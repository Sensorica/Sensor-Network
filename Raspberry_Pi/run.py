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
    while tokenizeData1[0] != "DATA":
	
        incomingData1 = ser.readline().strip()
        tokenizedData1 = incomingData.split(",")
		
    incomingData2 = ser.readline().strip()
    incomingData3 = ser.readline().strip()
    incomingData4 = ser.readline().strip()
    incomingData5 = ser.readline().strip()
    
    #splits comma seperated data into an array
    tokenizedData2 = incomingData.split(",")
    tokenizedData3 = incomingData.split(",")
    tokenizedData4 = incomingData.split(",")
    tokenizedData5 = incomingData.split(",")
    
    #assigns array to variables
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

    micFreq1 = tokenizeData2[1]
    micPkPower1 = tokenizeData2[2]
    micFreq2 = tokenizeData2[3]
    micPkPower2 = tokenizeData2[4]
    micFreq3 = tokenizeData2[5]
    micPkPower3 = tokenizeData2[6]
    micFreq4 = tokenizeData2[7]
    micPkPower4 = tokenizeData2[8]
    micFreq5 = tokenizeData2[9]
    micPkPower5 = tokenizeData2[10]

    vibXFreq1 = tokenizeData3[1]
    vibXPkPower1 = tokenizeData3[2]
    vibXFreq2 = tokenizeData3[3]
    vibXPkPower2 = tokenizeData3[4]
    vibXFreq3 = tokenizeData3[5]
    vibXPkPower3 = tokenizeData3[6]
    vibXFreq4 = tokenizeData3[7]
    vibXPkPower4 = tokenizeData3[8]
    vibXFreq5 = tokenizeData3[9]
    vibXPkPower5 = tokenizeData3[10]

    vibYFreq1 = tokenizeData4[1]
    vibYPkPower1 = tokenizeData4[2]
    vibYFreq2 = tokenizeData4[3]
    vibYPkPower2 = tokenizeData4[4]
    vibYFreq3 = tokenizeData4[5]
    vibYPkPower3 = tokenizeData4[6]
    vibYFreq4 = tokenizeData4[7]
    vibYPkPower4 = tokenizeData4[8]
    vibYFreq5 = tokenizeData4[9]
    vibYPkPower5 = tokenizeData4[10]

    vibZFreq1 = tokenizeData5[1]
    vibZPkPower1 = tokenizeData5[2]
    vibZFreq2 = tokenizeData5[3]
    vibZPkPower2 = tokenizeData5[4]
    vibZFreq3 = tokenizeData5[5]
    vibZPkPower3 = tokenizeData5[6]
    vibZFreq4 = tokenizeData5[7]
    vibZPkPower4 = tokenizeData5[8]
    vibZFreq5 = tokenizeData5[9]
    vibZPkPower5 = tokenizeData5[10]

    
    #json format  
    payload = '{"state":{"reported":{"nodeID":%s,"shaftTemp":%s,"ambientTemp":%s,"casingTemp":%s,"glandTemp":%s,"rpm":%s,"loadWasher1":%s,"loadWasher2":%s,"leakageCcsPerSecond":%s,"flushFlowRate":%s, "displacement1":%s, "displacement2":%s, "micFreq1":%s, "micPkPower1":%s, "micFreq2":%s, "micPkPower2":%s, "micFreq3":%s, "micPkPower3":%s, "micFreq4":%s, "micPkPower4":%s, "micFreq5":%s, "micPkPower5":%s, "vibXFreq1":%s, "vibXPkPower1":%s, "vibXFreq2":%s, "vibXPkPower2":%s, "vibXFreq3":%s, "vibXPkPower3":%s, "vibXFreq4":%s, "vibXPkPower4":%s, "vibXFreq5":%s, "vibXPkPower5":%s, "vibYFreq1":%s, "vibYPkPower1":%s, "vibYFreq2":%s, "vibYPkPower2":%s, "vibYFreq3":%s, "vibYPkPower3":%s, "vibYFreq4":%s, "vibYPkPower4":%s, "vibYFreq5":%s, "vibYPkPower5":%s, "vibZFreq1":%s, "vibZPkPower1":%s, "vibZFreq2":%s, "vibZPkPower2":%s, "vibZFreq3":%s, "vibZPkPower3":%s, "vibZFreq4":%s, "vibZPkPower4":%s, "vibZFreq5":%s, "vibZPkPower5":%s}}}' % (nodeID,shaftTemp,ambientTemp,casingTemp,glandTemp,rpm,loadWasher1,loadWasher2,leakageCcsPerSecond,flushFlowRate,displacement1,displacement2,micFreq1,micPkPower1,micFreq2,micPkPower2,micFreq3,micPkPower3,micFreq4,micPkPower4,micFreq5,micPkPower5,vibXFreq1,vibXPkPower1,vibXFreq2,vibXPkPower2,vibXFreq3,vibXPkPower3,vibXFreq4,vibXPkPower4,vibXFreq5,vibXPkPower5,vibYFreq1,vibYPkPower1,vibYFreq2,vibYPkPower2,vibYFreq3,vibYPkPower3,vibYFreq4,vibYPkPower4,vibYFreq5,vibYPkPower5,vibZFreq1,vibZPkPower1,vibZFreq2,vibZPkPower2,vibZFreq3,vibZPkPower3,vibZFreq4,vibZPkPower4,vibZFreq5,vibZPkPower5)
	
	#prints payload to console
    print (payload)
    
    #publishes payload to the topic to AWS endpoint
    mqttc.publish("$aws/things/testbench/shadow/update",payload,qos=0)
    json_file = open('/home/pi/webapp/json', 'w')
    json_file.write (payload)   
