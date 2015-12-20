/*
Demonstrate IR limit switch with analog in, for threshold adjustment
Author: T!b!
Date: 2015-11-25
*/
int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
                       // outside leads to ground and +5V
int val = 0;     // variable to store the value read
int LEDPin = 13; // output

void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);          //  setup serial
  pinMode(LEDPin, OUTPUT);      // sets the digital pin as output
}

void loop()                     // run over and over again
{
   val = analogRead(analogPin);    // read the input pin
   Serial.println(val);       // debug value
   delay(200);
}

