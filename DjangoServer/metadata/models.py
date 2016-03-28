from django.db import models


# Create your models here.
class Nodeowner(models.Model):
    node_owner_id = models.IntegerField()
    owner_name = models.CharField(max_length=200)

    def __str__(self):
        return self.owner_name


class Node(models.Model):
    nodeowner = models.ForeignKey(Nodeowner,on_delete=models.CASCADE)
    node_id = models.IntegerField()
    node_name = models.CharField(max_length=200)
    node_version = models.IntegerField()

    pump_id = models.IntegerField()
    pump_type = models.CharField(max_length=200, default='Pump')
    fluid_type = models.CharField(max_length=200)
    packing_model = models.CharField(max_length=200)
    packing_location = models.CharField(max_length=200)

    entry_date_time = models.DateTimeField('Date Published')

    def __str__(self):
        return self.node_name


class TestDatapoint(models.Model):
    models.ForeignKey(Node,on_delete=models.CASCADE)
    data_id = models.IntegerField()
    data_name = models.CharField(max_length=200)
    temperature = models.FloatField()
    rpm = models.IntegerField()



