# -*- coding: utf-8 -*-
# Generated by Django 1.9.4 on 2016-05-17 22:11
from __future__ import unicode_literals

import datetime
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('metadata', '0006_auto_20160413_2229'),
    ]

    operations = [
        migrations.CreateModel(
            name='MaintenanceLog',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('activity', models.TextField(default='')),
                ('created_by', models.DecimalField(decimal_places=0, default=1000000, max_digits=10)),
                ('created_timestamp', models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date')),
            ],
        ),
        migrations.CreateModel(
            name='PumpConfiguration',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('test_case_id', models.IntegerField(default=0)),
                ('packing_name', models.CharField(default='', max_length=200)),
                ('start_time', models.DateTimeField(default=datetime.datetime.now, verbose_name='Test Start Time')),
                ('packing_inner_diameter_inches', models.FloatField(default=0)),
                ('packing_outer_diameter_inches', models.FloatField(default=0)),
                ('packing_thickness_inches', models.FloatField(default=0)),
                ('surface_finish_rating', models.IntegerField(default=0)),
                ('fluid_type', models.CharField(default='', max_length=200)),
                ('initial_node_voltage', models.FloatField(default=0)),
                ('initial_temperature', models.FloatField(default=0)),
                ('end_time', models.DateTimeField(default=datetime.datetime.now, verbose_name='Test End Time')),
                ('config_created_by', models.DecimalField(decimal_places=0, default=1000000, max_digits=10)),
                ('config_created_timestamp', models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date')),
                ('config_updated_by', models.DecimalField(decimal_places=0, default=1000000, max_digits=10)),
                ('config_updated_timestamp', models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date')),
            ],
        ),
        migrations.RemoveField(
            model_name='testcase',
            name='pump',
        ),
        migrations.RemoveField(
            model_name='nodeowner',
            name='node_owner_id',
        ),
        migrations.RemoveField(
            model_name='pump',
            name='customer_id',
        ),
        migrations.AddField(
            model_name='manualdatapoint',
            name='data_created_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='manualdatapoint',
            name='data_created_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='manualdatapoint',
            name='data_updated_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='manualdatapoint',
            name='data_updated_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='node',
            name='node_created_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='node',
            name='node_created_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='node',
            name='node_updated_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='node',
            name='node_updated_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='nodeowner',
            name='ad_client_id',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='nodeowner',
            name='ad_org_id',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='nodeowner',
            name='isactive',
            field=models.CharField(default='Y', max_length=1),
        ),
        migrations.AddField(
            model_name='nodeowner',
            name='nodeowner_created_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='nodeowner',
            name='nodeowner_created_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='nodeowner',
            name='nodeowner_updated_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='nodeowner',
            name='nodeowner_updated_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='pump',
            name='pump_created_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='pump',
            name='pump_created_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='pump',
            name='pump_updated_by',
            field=models.DecimalField(decimal_places=0, default=1000000, max_digits=10),
        ),
        migrations.AddField(
            model_name='pump',
            name='pump_updated_timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Install Date'),
        ),
        migrations.AddField(
            model_name='sampleautomaticdatapoint',
            name='timestamp',
            field=models.DateTimeField(default=datetime.datetime.now, verbose_name='Timestamp'),
        ),
        migrations.DeleteModel(
            name='TestCase',
        ),
        migrations.AddField(
            model_name='pumpconfiguration',
            name='pump',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='metadata.Pump'),
        ),
    ]
