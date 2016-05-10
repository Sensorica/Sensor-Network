
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
 // while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
 // }


  Serial.println("Who are you?!!");
  delay(1000);

  // set the data rate for the SoftwareSerial port
//  mySerial.begin(4800);
//  mySerial.println("Hi, I'm the arduino that will be running the FFT.");
//  delay(3000);
//  mySerial.println("I'll be sending you data over a software emulated serial connection");
//  delay(3000);
//  mySerial.println("If you look at my connection, how am I getting powered?!");
//  delay(3000);
//  mySerial.println("Mmmmmwahahahha!!!");
//  delay(3000);
//  for (int a = 1; a < 1000; a++) {
//    mySerial.println("All your pins are belong to me!!!");
//  }
}

void loop() { // run over and over
    mySerial.println("Hi, I'm the arduino that will be running the FFT.");
  delay(3000);
  mySerial.println("I'll be sending you data over a software emulated serial connection");
  delay(3000);
  mySerial.println("If you look at my connection, how am I getting powered?!");
  delay(3000);
  mySerial.println("Mmmmmwahahahha!!!");
  delay(3000);
  for (int a = 1; a < 1000; a++) {
    mySerial.println("All your pins are belong to me!!!");
  }


  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

