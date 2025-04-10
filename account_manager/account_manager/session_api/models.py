from datetime import datetime, UTC

from django.contrib.auth.models import User
from django.db import models
from django.db.models.deletion import CASCADE, SET_NULL


class NCYSession(models.Model):
    session_id = models.IntegerField(null=False)
    created = models.DateTimeField(auto_now_add=True)
    expires = models.DateTimeField()
    key = models.BinaryField()
    user = models.ForeignKey(User, CASCADE, null=False)

class NCYSessionUserData(models.Model):
    user = models.ForeignKey(User, CASCADE)
    session_expiration_min = models.IntegerField()
    last_session = models.ForeignKey(NCYSession, SET_NULL, null=True)
