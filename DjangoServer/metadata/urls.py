__author__ = 'scott'

from django.conf.urls import url
from django.conf import settings
from django.conf.urls.static import static
from django.views.generic import TemplateView

from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),

    #This one "seems" to be correct, except for that it doesn't load
    # url(r'^freeboard/$', TemplateView.as_view(template_name="../../static_git/static_root/freeboard/index.html"), name='freeboard'),
    url(r'^freeboard/$', TemplateView.as_view(template_name="../templates/freeboard/index.html"), name='freeboard'),

    # This
    # url(r'^freeboard/$', TemplateView.as_view(template_name="../static/../templates/metadata/../static/freeboard/index.html"), name='freeboard'),
] + static(settings.STATIC_URL, document_root=settings.STATIC_ROOT) #ONLY FOR DEVELOPMENT

#ONLY FOR DEVELOPMENT
if settings.DEBUG:
    urlpatterns += static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)
    urlpatterns += static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)