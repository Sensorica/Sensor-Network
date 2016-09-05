""" 
Written by Jim Anastassiou - Sensorica www.sensorica.co
SENSORICA - sensor-network project - https://github.com/Sensorica/Sensor-Network 
"""
 
import sys                                 
import ssl
import paho.mqtt.client as mqtt
import serial
import os
import glob
import time


#sets up the serial port Note: will be changed to 115200
ser = serial.Serial('/dev/ttyUSB0', 9600)

#flushes serial input and output buffers
ser.flushInput()
ser.flushOutput()

#called while client tries to establish connection with the server 
def on_connect(mqttc, obj, flags, rc):
    if rc==0:
        print ("Subscriber Connection status code: "+str(rc)+" | Connection status: successful")
    elif rc==1:
        print ("Subscriber Connection status code: "+str(rc)+" | Connection status: Connection refused")

#called when a topic is successfully subscribed to
def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos)+"data"+str(obj))

#called when a message is received by a topic
def on_message(mqttc, obj, msg):
    print("Received message from topic: "+msg.topic+" | QoS: "+str(msg.qos)+" | Data Received: "+str(msg.payload))

#creating a client with client-id=mqtt-test
mqttc = mqtt.Client(client_id="testbench")

mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe
mqttc.on_message = on_message

#Configure network encryption and authentication options. Enables SSL/TLS support.
#adding client-side certificates and enabling tlsv1.2 support as required by aws-iot service
mqttc.tls_set("auth.pem", certfile="cert.pem", keyfile="privateKey.pem", tls_version=ssl.PROTOCOL_TLSv1_2, ciphers=None)

#connecting to aws-account-specific-iot-endpoint
mqttc.connect("AAU8NJF5D2XBZ.iot.us-east-1.amazonaws.com", port=8883) #AWS IoT service hostname and portno

#automatically handles reconnecting
mqttc.loop_start()

#main data loop
while True:
   
    #read serial port and trim spaces 
    incomingData1 = ser.readline().strip()
    tokenizedData1 = incomingData1.split(",")
    
    #print (incomingData1)
    #print (tokenizedData1)
    
    #synchronize with first data string by staying in loop
    while tokenizedData1[0] != "D":

        incomingData1 = ser.readline().strip()
        tokenizedData1 = incomingData1.split(",")
    
    #recieve remaining four data strings
    incomingData2 = ser.readline().strip()
    incomingData3 = ser.readline().strip()
    incomingData4 = ser.readline().strip()
    incomingData5 = ser.readline().strip()
    
    #splits comma seperated data into an array
    tokenizedData2 = incomingData2.split(",")
    tokenizedData3 = incomingData3.split(",")
    tokenizedData4 = incomingData4.split(",")
    tokenizedData5 = incomingData5.split(",")
    
    #assign array to variables
   
    #first string - sensors
    nodeID = tokenizedData1[1]
    shaftTemp = tokenizedData1[2]
    ambientTemp = tokenizedData1[3]
    casingTemp = tokenizedData1[4]
    glandTemp = tokenizedData1[5]
    rpm = tokenizedData1[6]
    loadWasher1 = tokenizedData1[7]
    loadWasher2 = tokenizedData1[8]
    leakageCcsPerSecond = tokenizedData1[9]
    flushFlowRate = tokenizedData1[10]
    displacement1 = tokenizedData1[11]
    displacement2 = tokenizedData1[12]
    
    #second string - mic data
    micFreq1 = tokenizedData2[1]
    micPkPower1 = tokenizedData2[2]
    micFreq2 = tokenizedData2[3]
    micPkPower2 = tokenizedData2[4]
    micFreq3 = tokenizedData2[5]
    micPkPower3 = tokenizedData2[6]
    micFreq4 = tokenizedData2[7]
    micPkPower4 = tokenizedData2[8]
    micFreq5 = tokenizedData2[9]
    micPkPower5 = tokenizedData2[10]
    
    #third string - vibration X axis
    vibXFreq1 = tokenizedData3[1]
    vibXPkPower1 = tokenizedData3[2]
    vibXFreq2 = tokenizedData3[3]
    vibXPkPower2 = tokenizedData3[4]
    vibXFreq3 = tokenizedData3[5]
    vibXPkPower3 = tokenizedData3[6]
    vibXFreq4 = tokenizedData3[7]
    vibXPkPower4 = tokenizedData3[8]
    vibXFreq5 = tokenizedData3[9]
    vibXPkPower5 = tokenizedData3[10]
    
    #fourth string - vibration Y axis
    vibYFreq1 = tokenizedData4[1]
    vibYPkPower1 = tokenizedData4[2]
    vibYFreq2 = tokenizedData4[3]
    vibYPkPower2 = tokenizedData4[4]
    vibYFreq3 = tokenizedData4[5]
    vibYPkPower3 = tokenizedData4[6]
    vibYFreq4 = tokenizedData4[7]
    vibYPkPower4 = tokenizedData4[8]
    vibYFreq5 = tokenizedData4[9]
    vibYPkPower5 = tokenizedData4[10]
    
    #fifth string - vibration Z axis
    vibZFreq1 = tokenizedData5[1]
    vibZPkPower1 = tokenizedData5[2]
    vibZFreq2 = tokenizedData5[3]
    vibZPkPower2 = tokenizedData5[4]
    vibZFreq3 = tokenizedData5[5]
    vibZPkPower3 = tokenizedData5[6]
    vibZFreq4 = tokenizedData5[7]
    vibZPkPower4 = tokenizedData5[8]
    vibZFreq5 = tokenizedData5[9]
    vibZPkPower5 = tokenizedData5[10]

    
    #json format  
    payload = '{"state":{"reported":{"nodeID":%s,"shaftTemp":%s,"ambientTemp":%s,"casingTemp":%s,"glandTemp":%s,"rpm":%s,"loadWasher1":%s,"loadWasher2":%s,"leakageCcsPerSecond":%s,"flushFlowRate":%s,"displacement1":%s, "displacement2":%s, "micFreq1":%s, "micPkPower1":%s, "micFreq2":%s, "micPkPower2":%s, "micFreq3":%s, "micPkPower3":%s, "micFreq4":%s, "micPkPower4":%s, "micFreq5":%s, "micPkPower5":%s, "vibXFreq1":%s, "vibXPkPower1":%s, "vibXFreq2":%s, "vibXPkPower2":%s, "vibXFreq3":%s, "vibXPkPower3":%s, "vibXFreq4":%s, "vibXPkPower4":%s, "vibXFreq5":%s, "vibXPkPower5":%s, "vibYFreq1":%s, "vibYPkPower1":%s, "vibYFreq2":%s, "vibYPkPower2":%s, "vibYFreq3":%s, "vibYPkPower3":%s, "vibYFreq4":%s, "vibYPkPower4":%s, "vibYFreq5":%s, "vibYPkPower5":%s, "vibZFreq1":%s, "vibZPkPower1":%s, "vibZFreq2":%s, "vibZPkPower2":%s, "vibZFreq3":%s, "vibZPkPower3":%s, "vibZFreq4":%s, "vibZPkPower4":%s, "vibZFreq5":%s, "vibZPkPower5":%s}}}' % (nodeID,shaftTemp,ambientTemp,casingTemp,glandTemp,rpm,loadWasher1,loadWasher2,leakageCcsPerSecond,flushFlowRate,displacement1,displacement2,micFreq1,micPkPower1,micFreq2,micPkPower2,micFreq3,micPkPower3,micFreq4,micPkPower4,micFreq5,micPkPower5,vibXFreq1,vibXPkPower1,vibXFreq2,vibXPkPower2,vibXFreq3,vibXPkPower3,vibXFreq4,vibXPkPower4,vibXFreq5,vibXPkPower5,vibYFreq1,vibYPkPower1,vibYFreq2,vibYPkPower2,vibYFreq3,vibYPkPower3,vibYFreq4,vibYPkPower4,vibYFreq5,vibYPkPower5,vibZFreq1,vibZPkPower1,vibZFreq2,vibZPkPower2,vibZFreq3,vibZPkPower3,vibZFreq4,vibZPkPower4,vibZFreq5,vibZPkPower5)
    #prints payload to console
    print (payload)
    #for x in range (0, len(tokenizedData1), 1):
    #    print (tokenizedData1[x])
    #publishes payload to the topic to AWS endpoint
    mqttc.publish("$aws/things/testbench/shadow/update",payload,qos=0)
    
    #write current payload to file 
    json_file = open('/home/pi/webapp/json', 'w')
    json_file.write (payload)   
