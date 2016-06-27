"""    Written by Jim Anastassiou - Sensorica www.sensorica.co
Sample python script that publishes the temperature at Sensorica lab
from a Waterproof DS18B20 Digital temperature sensor connected to a Raspberry Pi
to a MQTT broker running at test.mosquitto.org under the topic temp/random
Run script and visit http://test.mosquitto.org/gauge/
""" 
import os
import glob
import time

os.system('modprobe w1-gpio')
os.system('modprobe w1-therm')

base_dir = '/sys/bus/w1/devices/'
device_folder = glob.glob(base_dir + '28*')[0]
device_file = device_folder + '/w1_slave'

def read_temp_raw():
    f = open(device_file, 'r')
    lines = f.readlines()
    f.close()
    return lines

def read_temp():
    lines = read_temp_raw()
    while lines[0].strip()[-3:] != 'YES':
        time.sleep(0.2)
        lines = read_temp_raw()
    equals_pos = lines[1].find('t=')
    if equals_pos != -1:
        temp_string = lines[1][equals_pos+2:]
        temp_c = float(temp_string) / 1000.0
        #temp_f = temp_c * 9.0 / 5.0 + 32.0 put this here for farenheit
        return temp_c

while True:
        current_temp = read_temp()
        bashCommand = "mosquitto_pub -h test.mosquitto.org -t temp/random -m %s" % current_temp
        os.system(bashCommand)
        print(current_temp)
        time.sleep(1)
