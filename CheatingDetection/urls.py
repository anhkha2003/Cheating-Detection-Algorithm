from django.contrib import admin
from django.urls import path
from . import views

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', views.home_view, name='home'),
    path('submissions', views.result_view, name='submissions'),
    path('process', views.process_view, name='process'),
    path('submission/<int:sub_id>/', views.submission_view, name='submission'),
    path('about', views.about_view, name='about'),
]
