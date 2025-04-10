import os
from datetime import timedelta, datetime, UTC

from django.contrib.auth.models import Group, User
from django.db.models import QuerySet
from django.http import JsonResponse
from rest_framework import permissions, viewsets
from rest_framework.decorators import api_view

from account_manager.session_api.models import NCYSession, NCYSessionUserData
from account_manager.session_api.serializers import GroupSerializer, UserSerializer, NCYSessionSerializer


class UserViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows users to be viewed or edited.
    """
    queryset = User.objects.all().order_by('-date_joined')
    serializer_class = UserSerializer
    permission_classes = [permissions.IsAuthenticated]


class GroupViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows groups to be viewed or edited.
    """
    queryset = Group.objects.all().order_by('name')
    serializer_class = GroupSerializer
    permission_classes = [permissions.IsAuthenticated]


def get_or_create_udata(user):
    query_ret: QuerySet = NCYSessionUserData.objects.get(user=user)
    if query_ret is None:
        new_data = NCYSessionUserData(user)
        new_data.user = user
        new_data.session_expiration_min = 30
        new_data.save()
        return new_data
    return query_ret


def expired(u_session):
    return datetime.now(UTC) > u_session.expires


@api_view(['GET'])
def get_current_session(request, username):
    user = User.objects.get_by_natural_key(username)
    udata = get_or_create_udata(user)
    if udata.last_session is None or expired(udata.last_session):
        new_session = NCYSession()
        new_session.session_id = udata.last_session.session_id + 1 if udata.last_session is not None else 0
        new_session.key = os.urandom(16)
        new_session.created = datetime.now()
        new_session.expires = datetime.now() + timedelta(minutes=udata.session_expiration_min)
        new_session.user = user
        new_session.save()
        udata.last_session = new_session
        udata.save()

    if request.method == 'GET':
        serializer = NCYSessionSerializer(udata.last_session, context={'request': request})
        return JsonResponse(serializer.data)
