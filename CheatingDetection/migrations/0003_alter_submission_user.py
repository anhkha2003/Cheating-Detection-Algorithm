# Generated by Django 4.2.4 on 2023-09-30 23:21

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('CheatingDetection', '0002_alter_submission_score'),
    ]

    operations = [
        migrations.AlterField(
            model_name='submission',
            name='user',
            field=models.ForeignKey(null=True, on_delete=django.db.models.deletion.CASCADE, to='CheatingDetection.user'),
        ),
    ]
