__author__ = 'scott'

from django.conf.urls import url
from django.views.generic import TemplateView

from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),
    url(r'^freeboard/$', TemplateView.as_view(template_name="../static/../templates/metadata/freeboard/index.html"), name='freeboard'),
]