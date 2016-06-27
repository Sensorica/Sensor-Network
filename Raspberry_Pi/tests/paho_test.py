#!/usr/bin/python3

#required libraries
import sys                                 
import ssl
import paho.mqtt.client as mqtt
import time

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
mqttc = mqtt.Client(client_id="leaknet_poc")

mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe
mqttc.on_message = on_message

#Configure network encryption and authentication options. Enables SSL/TLS support.
#adding client-side certificates and enabling tlsv1.2 support as required by aws-iot service
mqttc.tls_set("VeriSign-Class 3-Public-Primary-Certification-Authority-G5.pem", certfile="5c8a6b4fd4-certificate.pem.crt", keyfile="5c8a6b4fd4-private.pem.key", tls_version=ssl.PROTOCOL_TLSv1_2, ciphers=None)

#connecting to aws-account-specific-iot-endpoint
mqttc.connect("AAU8NJF5D2XBZ.iot.us-west-2.amazonaws.com", port=8883) #AWS IoT service hostname and portno

#automatically handles reconnecting
mqttc.loop_start()

while True:
    payload = '{"state":{"reported":{"ambientTemp":24,"glandTemp":22,"throatTemp":23}}}'
    #the topic to publish to
    mqttc.publish("$aws/things/leaknet_poc/shadow/update",payload,qos=0)
    time.sleep(2)
