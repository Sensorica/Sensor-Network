from django.shortcuts import render

# Create your views here.
from django.http import HttpResponse


def index(request):
    context = {
        "title": "Hello there!"
    }
    return render(request, "base.html",context)