from django.contrib import admin

from .models import Nodeowner,Node, Pump, TestCase, ManualDatapoint, SampleAutomaticDatapoint
# Register your models here.

admin.site.register(Nodeowner)
admin.site.register(Node)
admin.site.register(Pump)
admin.site.register(TestCase)
admin.site.register(ManualDatapoint)
admin.site.register(SampleAutomaticDatapoint)