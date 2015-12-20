/*Written by Jim Anastassiou - Sensorica www.sensorica.co
  Sample sketch that receives message over serial from XBee  
  in AT mode and returns response. Tested with another XBee
  connected to a Raspberry Pi. See link for python script:
  https://github.com/Sensorica/Sensor-Network/tree/master/Raspberry_Pi/XBee_AT_test.py
*/   
String a;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  }

void loop() {
  while(Serial.available()) {
  a= Serial.readString(); // read the incoming data as string
  Serial.println(a);
  }
}

