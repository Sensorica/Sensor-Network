Here is any code that is to be used by the Raspberry Pi base station. 

*labtemp.py 
Sample python script that publishes the temperature at Sensorica lab from a Waterproof DS18B20 Digital temperature sensor connected to a Raspberry Pi to a MQTT broker running at test.mosquitto.org under the topic temp/random.
Uses the open source Mosquitto command line tools : http://mosquitto.org
See the temperature at the lab : http://test.mosquitto.org/gauge

*snaplabs.py
Sample python script that pushes data to Snaplabs from a Waterproof DS18B20 Digital temperature sensor connected to a Raspberry Pi at Robco using method POST x-www-form-urlencoded body: temp=VALUE.
