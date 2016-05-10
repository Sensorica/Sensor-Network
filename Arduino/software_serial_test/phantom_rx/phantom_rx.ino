
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 11); // RX, TX

void setup() {
  analogReference(EXTERNAL);
  
  Serial.begin(4800);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
  Serial.println("Receiving!");
  delay(1000);
  mySerial.begin(4800);
  
}

void loop() { 

  if (mySerial.overflow()){
    Serial.println("Software Serial overflow");
  }

//// This always prints
//  if(mySerial.isListening()){
//    Serial.println("mySerial is listening");
//  }
  
  if (mySerial.available()) {
    Serial.write(mySerial.read());
//    Serial.write("w");
  }
}

