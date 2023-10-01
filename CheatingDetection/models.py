from django.db import models


class User(models.Model):
    name = models.CharField(max_length=200, unique=True, db_index=True)


class Submission(models.Model):
    score = models.IntegerField(default=0)
    sub_id = models.IntegerField(unique=True, db_index=True)
    language = models.CharField(max_length=20)
    source = models.CharField(max_length=100000)
    user = models.ForeignKey(User, on_delete=models.CASCADE, null=True)