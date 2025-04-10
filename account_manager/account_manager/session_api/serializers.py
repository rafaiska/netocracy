from django.contrib.auth.models import Group, User
from rest_framework import serializers

from account_manager.session_api.models import NCYSession


class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ['url', 'username', 'email', 'groups']


class GroupSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Group
        fields = ['url', 'name']

class NCYSessionSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = NCYSession
        fields = ['session_id', 'created', 'expires', 'key', 'user_id']