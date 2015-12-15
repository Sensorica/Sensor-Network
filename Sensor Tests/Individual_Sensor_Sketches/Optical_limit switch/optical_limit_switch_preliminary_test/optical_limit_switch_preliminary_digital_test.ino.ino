/*
Demonstrate IR interrupt opto switch, digital
Author: Darin White
Date: 2009-05-14
*/

int swPin = 8;     // input
int LEDPin = 13; // output

void setup()                    // run once, when the sketch starts
{
  pinMode(LEDPin, OUTPUT);      // sets the digital pin as output
}

void loop()                     // run over and over again
{
  if (digitalRead(swPin)) {
     digitalWrite(LEDPin, HIGH); 
     delay(300);
  }
  else {
     digitalWrite(LEDPin, LOW); 
  }
}

