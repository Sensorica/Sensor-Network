"""    Written by Jim Anastassiou - Sensorica www.sensorica.co
Sample python script that sends message to the XBee over serial 
in AT mode and prints response. Tested with another XBee connected
to an Arduino. See link for sketch:
https://github.com/Sensorica/Sensor-Network/tree/master/Arduino/XBee_AT_test/
""" 
import serial
import os
import glob
import time

ser = serial.Serial('/dev/ttyUSB0', 9600)
ser.flushInput()
ser.flushOutput()
while True:
    incomingData = ser.readline().strip()
    tokenizedData = incomingData.split(",")
    
    glandTemp = tokenizedData[0]
    ambientTemp = tokenizedData[1]
    dropCount = tokenizedData[2]
    shaftSpeed = tokenizedData[3]

    glandTempBashCommand = "curl --data \"reading=%s\" http://iot.snaplabs.io:8000/entity/12:0/record" % glandTemp
    ambientTempBashCommand = "curl --data \"reading=%s\" http://iot.snaplabs.io:8000/entity/12:1/record" % ambientTemp
    dropCounterBashCommand = "curl --data \"reading=%s\" http://iot.snaplabs.io:8000/entity/12:2/record" % dropCount
    shaftSpeedBashCommand = "curl --data \"reading=%s\" http://iot.snaplabs.io:8000/entity/12:3/record" % shaftSpeed
    
    os.system(glandTempBashCommand)
    os.system(ambientTempBashCommand)
    os.system(dropCounterBashCommand)
    os.system(shaftSpeedBashCommand)
#    print incomingData
#    print tokenizedData[0]

#    print tokenizedData[1]   
#    print tokenizedData[2]
#    print glandTemp

