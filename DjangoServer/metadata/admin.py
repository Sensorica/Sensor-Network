from django.contrib import admin

from .models import Nodeowner,Node, Pump, ManualDatapoint, SampleAutomaticDatapoint,PumpConfiguration, MaintenanceLog
# Register your models here.

admin.site.register(Nodeowner)
admin.site.register(Node)
admin.site.register(Pump)
admin.site.register(PumpConfiguration)
admin.site.register(ManualDatapoint)
admin.site.register(SampleAutomaticDatapoint)
admin.site.register(MaintenanceLog)
