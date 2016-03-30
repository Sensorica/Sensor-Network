from django.db import models


# Create your models here.
class Nodeowner(models.Model):
    node_owner_id = models.IntegerField()
    owner_name = models.CharField(max_length=200)

    def __str__(self):
        return self.owner_name


class Node(models.Model):
    nodeowner = models.ForeignKey(Nodeowner,on_delete=models.CASCADE)
    customer_id = models.IntegerField()
    node_name = models.CharField(max_length=200)  #just for fun and to make the process of installation more intuitive
    customer_equipment_id = models.IntegerField()

    # Sponsor Defined Static Metadata
    arduino_node_id = models.IntegerField()
    raspi_node_id = models.IntegerField()
    pump_serial_number = models.IntegerField()
    pump_manufacturer = models.CharField(max_length=200)
    pump_model = models.CharField(max_length=200)
    pump_size_x = models.IntegerField()
    pump_size_y = models.IntegerField()
    motor_hp = models.FloatField()
    rpm_rating = models.IntegerField()
    is_variable_speed = models.BooleanField()
    motor_voltage_rating = models.FloatField()
    motor_amperage_rating = models.FloatField()
    imp_size_inches = models.FloatField()
    shaft_OR_sleeve_OD = models.FloatField()
    stuffing_box_OD = models.FloatField()
    packing_ring_UOM = models.FloatField()
    lantern_flush_fluid_type = models.CharField(max_length=200)

    # Sponsor Defined Non-Static Metadata (or undefined)
    packing_X_section_DIM1 = models.FloatField()
    packing_X_section_DIM2 = models.FloatField()
    packing_ring_shape = models.CharField(max_length=200)
    packing_ring_material1 = models.CharField(max_length=200)
    packing_ring_material2 = models.CharField(max_length=200)
    packing_ring_material3 = models.CharField(max_length=200)
    packing_ring_material4 = models.CharField(max_length=200)
    packing_ring_material5 = models.CharField(max_length=200)
    packing_ring_material6 = models.CharField(max_length=200)
    has_lantern_ring = models.BooleanField()
    lantern_ring_position = models.CharField(max_length=200)
    lantern_flush_volume_flow = models.FloatField()
    lantern_ring_fluid_pressure = models.FloatField()
    has_gasket_separator = models.BooleanField()
    has_throat_brushing = models.BooleanField()
    lantern_ring_width_inches = models.FloatField()
    number_of_packing_rings = models.IntegerField()
    packing_style_used = models.CharField(max_length=200)
    pump_location_gps_latitude = models.FloatField()
    pump_location_gps_longitude = models.FloatField()
    pump_location_gps_altitude = models.FloatField()
    sleeve_material = models.CharField(max_length=200)
    sleeve_install_date = models.DateTimeField('Install Date')
    sleeve_condition_at_start = models.IntegerField()
    is_from_bulk = models.BooleanField()
    is_precompressed = models.BooleanField()
    num_of_studs = models.IntegerField()
    bearing_seal_type = models.CharField(max_length=200)
    nominal_flow_in_g_per_min = models.FloatField()
    nominal_pressure_in_psi = models.FloatField()
    nominal_pressure_suction_in_psi = models.FloatField()
    fluid_pumped = models.CharField(max_length=200)
    nominal_solids_percentage = models.FloatField()
    does_media_change = models.BooleanField()
    fluid_specific_gravity = models.FloatField()
    nominal_fluid_temperature_celcius = models.FloatField()
    gearbox_lube_brand = models.CharField(max_length=200)
    gearbox_lube_iso = models.CharField(max_length=200)
    gearbox_lube_type = models.CharField(max_length=200)
    is_gearbox_lube_synthetic = models.BooleanField()
    is_installer_trained = models.BooleanField()
    installer_training_program = models.CharField(max_length=200)
    is_installer_witnessed = models.BooleanField()
    is_installation_proper = models.BooleanField()
    maintenance_crew_ID = models.IntegerField()
    past_maintenance_interval_days = models.IntegerField()
    target_maintenance_level = models.IntegerField()
    hourly_cost_of_downtime = models.FloatField()
    cost_of_water_per_1000_m3 = models.FloatField()
    cost_of_rebuild = models.FloatField()
    cost_of_sleeve = models.FloatField()
    cost_of_bearing = models.FloatField()
    cost_of_labour = models.FloatField()
    cost_of_lubs = models.FloatField()
    cost_of_other_rebuild = models.FloatField()
    cost_of_bearing_seal = models.FloatField()
    is_critical_application = models.BooleanField()
    sensor_configuration = models.CharField(max_length=200)
    node_install_date = models.DateTimeField('Node Install Date')
    node_deploy_date = models.DateTimeField('Node Deploy Date')
    node_hardware_version = models.CharField(max_length=200)
    node_firmware_version = models.CharField(max_length=200)

    def __str__(self):
        return self.node_name


class TestDatapoint(models.Model):
    models.ForeignKey(Node,on_delete=models.CASCADE)
    data_id = models.IntegerField()
    data_name = models.CharField(max_length=200)
    temperature = models.FloatField()
    rpm = models.IntegerField()



