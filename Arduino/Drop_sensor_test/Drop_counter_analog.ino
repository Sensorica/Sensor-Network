/*
Demonstrate IR interrupt opto switch with analog in, for threshold adjustment
Author: T!b!
Date: 2015-11-25
*/
int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
                       // outside leads to ground and +5V
int previousValue;
int currentValue;     // variable to store the value read
int LEDPin = 13; // output
int drip=1;
void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);          //  setup serial
  pinMode(LEDPin, OUTPUT);      // sets the digital pin as output
}

void loop()                     // run over and over again
{
   currentValue = analogRead(analogPin);    // read the input pin
   
   if ((previousValue < 150) && (currentValue > 250))
       { 
       Serial.print("drip count = ");
       Serial.println(drip);
       drip += 1; 
       }   
    previousValue = currentValue; 
    delay(5);
}
       
    
 
     
  
