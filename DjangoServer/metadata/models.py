from django.db import models
from django.core.validators import RegexValidator
import datetime


# Create your models here.
class Nodeowner(models.Model):
    ad_client_id = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)#Alternative Solution
    ad_org_id = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    isactive = models.CharField(max_length=1, default='Y',null=False,)
    owner_name = models.CharField(max_length=200, default="NA")

    #Table Tracing
    nodeowner_created_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    nodeowner_created_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)
    nodeowner_updated_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    nodeowner_updated_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)


    def __str__(self):
        return self.owner_name


class Pump(models.Model):
    #IDs
    nodeowner = models.ForeignKey(Nodeowner,on_delete=models.CASCADE)
    pump_name = models.CharField(max_length=200,default='')
    customer_equipment_id = models.IntegerField(default=0)

    #pump static metadata
    pump_serial_number = models.IntegerField(default=0)
    pump_manufacturer = models.CharField(max_length=200,default='')
    pump_model = models.CharField(max_length=200,default='')
    pump_size_x = models.IntegerField(default=0)
    pump_size_y = models.IntegerField(default=0)
    motor_hp = models.FloatField(default=0)
    rpm_rating = models.IntegerField(default=0)
    is_variable_speed = models.BooleanField(default=False)
    motor_voltage_rating = models.FloatField(default=0)
    motor_amperage_rating = models.FloatField(default=0)
    imp_size_inches = models.FloatField(default=0)
    shaft_OR_sleeve_OD = models.FloatField(default=0)
    stuffing_box_OD = models.FloatField(default=0)
    packing_ring_UOM = models.FloatField(default=0)
    lantern_flush_fluid_type = models.CharField(max_length=200,default='')
    packing_X_section_DIM1 = models.FloatField(default=0)
    packing_X_section_DIM2 = models.FloatField(default=0)
    packing_ring_shape = models.CharField(max_length=200,default='')
    packing_ring_material1 = models.CharField(max_length=200,default='')
    packing_ring_material2 = models.CharField(max_length=200,default='')
    packing_ring_material3 = models.CharField(max_length=200,default='')
    packing_ring_material4 = models.CharField(max_length=200,default='')
    packing_ring_material5 = models.CharField(max_length=200,default='')
    packing_ring_material6 = models.CharField(max_length=200,default='')
    has_lantern_ring = models.BooleanField(default=False)
    lantern_ring_position = models.CharField(max_length=200,default='')
    lantern_flush_volume_flow = models.FloatField(default=0)
    lantern_ring_fluid_pressure = models.FloatField(default=0)
    has_gasket_separator = models.BooleanField(default=False)
    has_throat_brushing = models.BooleanField(default=False)
    lantern_ring_width_inches = models.FloatField(default=0)
    number_of_packing_rings = models.IntegerField(default=0)
    packing_style_used = models.CharField(max_length=200,default='')
    pump_location_gps_latitude = models.FloatField(default=0)
    pump_location_gps_longitude = models.FloatField(default=0)
    pump_location_gps_altitude = models.FloatField(default=0)
    sleeve_material = models.CharField(max_length=200,default='')
    sleeve_install_date = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)
    sleeve_condition_at_start = models.IntegerField(default=0)
    is_from_bulk = models.BooleanField(default=False)
    is_precompressed = models.BooleanField(default=False)
    num_of_studs = models.IntegerField(default=0)
    nominal_flow_in_g_per_min = models.FloatField(default=0)
    nominal_pressure_in_psi = models.FloatField(default=0)
    nominal_pressure_suction_in_psi = models.FloatField(default=0)
    fluid_pumped = models.CharField(max_length=200,default='')
    nominal_solids_percentage = models.FloatField(default=0)
    does_media_change = models.BooleanField(default=False)
    fluid_specific_gravity = models.FloatField(default=0)
    nominal_fluid_temperature_celcius = models.FloatField(default=0)
    gearbox_lube_brand = models.CharField(max_length=200,default='')
    gearbox_lube_iso = models.CharField(max_length=200,default='')
    gearbox_lube_type = models.CharField(max_length=200,default='')
    is_gearbox_lube_synthetic = models.BooleanField(default=False)
    is_installer_trained = models.BooleanField(default=False)
    installer_training_program = models.CharField(max_length=200,default='')
    is_installer_witnessed = models.BooleanField(default=False)
    is_installation_proper = models.BooleanField(default=False)
    maintenance_crew_ID = models.IntegerField(default=0)
    past_maintenance_interval_days = models.IntegerField(default=0)
    target_maintenance_level = models.IntegerField(default=0)
    hourly_cost_of_downtime = models.FloatField(default=0)
    cost_of_water_per_1000_m3 = models.FloatField(default=0)
    cost_of_rebuild = models.FloatField(default=0)
    cost_of_sleeve = models.FloatField(default=0)
    cost_of_bearing = models.FloatField(default=0)
    cost_of_labour = models.FloatField(default=0)
    cost_of_lubs = models.FloatField(default=0)
    cost_of_other_rebuild = models.FloatField(default=0)
    cost_of_bearing_seal = models.FloatField(default=0)
    is_critical_application = models.BooleanField(default=False)
    sensor_configuration = models.CharField(max_length=200,default='')
    node_install_date = models.DateTimeField('Node Install Date',default=datetime.datetime.now,null=False)
    node_deploy_date = models.DateTimeField('Node Deploy Date',default=datetime.datetime.now,null=False)
    bearing_seal_type = models.CharField(max_length=200,default='')

    #Table tracing
    pump_created_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    pump_created_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)
    pump_updated_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    pump_updated_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)

    def __str__(self):
        return self.pump_name


class Node(models.Model):
    nodeowner = models.ForeignKey(Nodeowner,on_delete=models.CASCADE)
    current_equipment_id = models.IntegerField(default=0) #This defines which pump the node it currently attached to

    #Node metadata
    node_id = models.IntegerField(default=0)
    node_name = models.CharField(max_length=200,default='')  #just for fun and to make the process of installation more intuitive
    associated_raspi_node_id = models.IntegerField(default=0)
    node_hardware_version = models.CharField(max_length=200,default='')
    node_firmware_version = models.CharField(max_length=200,default='')

    #Table tracing
    node_created_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    node_created_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)
    node_updated_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    node_updated_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)

    def __str__(self):
        return self.node_name


class PumpConfiguration(models.Model):
    pump = models.ForeignKey(Pump,on_delete=models.CASCADE)
    test_case_id = models.IntegerField(default=0)
    packing_name = models.CharField(max_length=200,default='')
    start_time = models.DateTimeField('Test Start Time',default=datetime.datetime.now,null=False)
    packing_inner_diameter_inches = models.FloatField(default=0)
    packing_outer_diameter_inches = models.FloatField(default=0)
    packing_thickness_inches = models.FloatField(default=0)
    surface_finish_rating = models.IntegerField(default=0)
    fluid_type = models.CharField(max_length=200,default='')
    initial_node_voltage = models.FloatField(default=0)
    initial_temperature = models.FloatField(default=0)
    end_time = models.DateTimeField('Test End Time', default=datetime.datetime.now,null=False)

    #Table tracing
    config_created_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    config_created_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)
    config_updated_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    config_updated_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)

    def __str__(self):
        return "Test #" + self.test_case_id


class ManualDatapoint(models.Model):
    models.ForeignKey(PumpConfiguration,on_delete=models.CASCADE)
    datapoint_id = models.IntegerField(default=0)
    timestamp = models.DateTimeField('Timestamp', default=datetime.datetime.now,null=False)
    time_to_failure = models.FloatField(default=-1) #set to -1 if you don't know yet
    out_pressure = models.FloatField(default=0)
    in_pressure = models.FloatField(default=0)

    #Table tracing
    data_created_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    data_created_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)
    data_updated_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    data_updated_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)

    def __str__(self):
        return "Datapoint at #" + self.datapoint_id


class SampleAutomaticDatapoint(models.Model):
    models.ForeignKey(Node,on_delete=models.CASCADE)
    node_id = models.IntegerField(default=0)
    datapoint_id = models.IntegerField(default=0)
    timestamp = models.DateTimeField('Timestamp', default=datetime.datetime.now,null=False)
    load_washer_1_lbs = models.FloatField(default=0)
    load_washer_2_lbs = models.FloatField(default=0)
    gland_follower_position_inches = models.FloatField(default=0)
    force_on_gland_follower_lbs = models.FloatField(default=0)
    equipment_id = models.IntegerField(default=0) #This defines which pump the node is currently attached to
    flush_water_flow_rate = models.FloatField(default=0)
    leakage_rate_cc_per_s = models.FloatField(default=0)

    ambient_temp_C = models.FloatField(default=0)
    shaft_temp_C = models.FloatField(default=0)
    casing_temp_C = models.FloatField(default=0)
    gland_temp_C = models.FloatField(default=0)
    rpm = models.IntegerField(default=0)

    def __str__(self):
        return "Datapoint at #" + self.datapoint_id


class MaintenanceLog(models.Model):
    models.ForeignKey(PumpConfiguration,on_delete=models.CASCADE)
    activity = models.TextField(default='Tightening Gland Follower')

    #Table tracing
    created_by = models.DecimalField(max_digits=10,decimal_places=0,null=False,default=1000000)
    created_timestamp = models.DateTimeField('Install Date',default=datetime.datetime.now,null=False)












