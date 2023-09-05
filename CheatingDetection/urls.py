from django.contrib import admin
from django.urls import path
from . import views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', views.home_view, name='home'),
    path('submissions', views.result_view, name='submissions'),
    path('submission/<str:file_name>/', views.submission_view, name='submission'),
]
