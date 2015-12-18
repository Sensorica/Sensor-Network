/*
Demonstrate drop counting using an limit switch, with analog in
Author: T!b!
Date: 2015-11-25
*/
int analogPin = 3;     // potentiometer wiper (middle terminal) connected to analog pin 3
                       // outside leads to ground and +5V
int previousValue;
int currentValue;     // variable to store the value read
int LEDPin = 13; // output, lights the IR LED
int drops=1; // define drop count variable


void setup()                    // run once, when the sketch starts
{
  Serial.begin(9600);          //  setup serial
  pinMode(LEDPin, OUTPUT);      // sets the digital pin as output
}

void loop()                     // run over and over again
{
   currentValue = analogRead(analogPin);    // read the input pin
   
   if ((previousValue < 700) && (currentValue > 900)) // if it goes from LOW to HIGH, [adjust the threshoold with a bandgap]
       { 
       drops += 1; //increment number of drops
       Serial.print("drip count = "); // print cont on the screen
       Serial.println(drops); // print count on the screen
       
       }   
    previousValue = currentValue; 
    delay(10); // for some reason it works better with 10ms delay. 
}


/********************************************************
THIS IS A SIMPLER PROGRAM, USED FOR THRESHOLD ADJUSTMENT
Disable all code above and enable this one
*********************************************************/
/*
Demonstrate IR limit switch with analog in, for threshold adjustment
Author: T!b!
Date: 2015-11-25
*/

/***delete this if you want to test**
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
***Delete this***/

     
  
