from django.contrib import admin

from .models import Nodeowner,Node,TestDatapoint
# Register your models here.

admin.site.register(Nodeowner)
admin.site.register(Node)
admin.site.register(TestDatapoint)
