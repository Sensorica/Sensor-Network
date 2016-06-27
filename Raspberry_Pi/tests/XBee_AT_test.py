"""    Written by Jim Anastassiou - Sensorica www.sensorica.co
Sample python script that sends message to the XBee over serial 
in AT mode and prints response. Tested with another XBee connected
to an Arduino. See link for sketch:
https://github.com/Sensorica/Sensor-Network/tree/master/Arduino/XBee_AT_test/
""" 
import serial

ser = serial.Serial('/dev/ttyUSB0', 9600)
string = 'Hello from Raspberry Pi'
print 'Sending "%s"' % string
ser.write('%s\n' % string)

while True:
    incoming = ser.readline().strip()
    print 'Received %s' % incoming
    ser.write('RPi Received: %s\n' % incoming)
