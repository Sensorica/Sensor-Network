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
    incomingData = ser.readline().strip()
    
    #splits comma seperated data into an array
    tokenizedData = incomingData.split(",")
    
    #assigns array to variables
    nodeID = tokenizedData[0]
    shaftTemp = tokenizedData[1]
    ambientTemp = tokenizedData[2]
    casingTemp = tokenizedData[3]
    glandTemp = tokenizedData[4]
    rpm = tokenizedData[5]
    loadWasher1 = tokenizedData[6]
    loadWasher2 = tokenizedData[7]
    leakageCcsPerSecond = tokenizedData[8]
    flushFlow = tokenizedData[9]
    displacement1 = tokenizedData[10]
    displacement2 = tokenizedData[11]
    micFreq1 = tokenizeData[12]
    micPkPower1 = tokenizeData[13]
    micFreq2 = tokenizeData[14]
    micPkPower2 = tokenizeData[15]
    micFreq3 = tokenizeData[16]
    micPkPower3 = tokenizeData[17]
    micFreq4 = tokenizeData[18]
    micPkPower4 = tokenizeData[19]
    micFreq5 = tokenizeData[20]
    micPkPower5 = tokenizeData[21]
    vibXFreq1 = tokenizeData[22]
    vibXPkPower1 = tokenizeData[23]
    vibXFreq2 = tokenizeData[24]
    vibXPkPower2 = tokenizeData[25]
    vibXFreq3 = tokenizeData[26]
    vibXPkPower3 = tokenizeData[27]
    vibXFreq4 = tokenizeData[28]
    vibXPkPower4 = tokenizeData[29]
    vibXFreq5 = tokenizeData[30]
    vibXPkPower5 = tokenizeData[31]
    vibYFreq1 = tokenizeData[32]
    vibYPkPower1 = tokenizeData[33]
    vibYFreq2 = tokenizeData[34]
    vibYPkPower2 = tokenizeData[35]
    vibYFreq3 = tokenizeData[36]
    vibYPkPower3 = tokenizeData[37]
    vibYFreq4 = tokenizeData[38]
    vibYPkPower4 = tokenizeData[39]
    vibYFreq5 = tokenizeData[40]
    vibYPkPower5 = tokenizeData[41]
    vibZFreq1 = tokenizeData[42]
    vibZPkPower1 = tokenizeData[43]
    vibZFreq2 = tokenizeData[44]
    vibZPkPower2 = tokenizeData[45]
    vibZFreq3 = tokenizeData[46]
    vibZPkPower3 = tokenizeData[47]
    vibZFreq4 = tokenizeData[48]
    vibZPkPower4 = tokenizeData[49]
    vibZFreq5 = tokenizeData[50]
    vibZPkPower5 = tokenizeData[51]

    
    #json format  
    payload = '{"state":
                   {"reported":
                       {"nodeID":%s,
                        "shaftTemp":%s,
                        "ambientTemp":%s,
                        "casingTemp":%s,
                        "glandTemp":%s,
                        "rpm":%s,
                        "loadWasher1":%s,
                        "loadWasher2":%s,
                        "leakageCcsPerSecond":%s,
                        "flushFlowRate":%s}}}' % 
                        (nodeID,shaftTemp,ambientTemp,casingTemp,glandTemp,rpm,loadWasher1,loadWasher2,leakageCcsPerSecond,flushFlowRate)
  
    #prints payload to console
    print (payload)
    
    #publishes payload to the topic to AWS endpoint
    mqttc.publish("$aws/things/testbench/shadow/update",payload,qos=0)
    json_file = open('/home/pi/webapp/json', 'w')
    json_file.write (payload)   
