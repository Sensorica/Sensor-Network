
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  analogReference(EXTERNAL);
  
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Receiving!");
  delay(1000);
  mySerial.begin(4800);
  
}

void loop() { 
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

