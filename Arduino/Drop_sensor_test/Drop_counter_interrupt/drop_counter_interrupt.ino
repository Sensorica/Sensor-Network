int LEDpin = 13; // output, lights the IR LED
volatile int drops = 0; // define drop count variable
int previousDrops = 0; // define drop count variable


void setup() {
    Serial.begin(9600);        //  setup serial
    pinMode(LEDpin, OUTPUT);   // sets the digital pin as output
    attachInterrupt(digitalPinToInterrupt(2), event, RISING);
}

void loop() {
  
    if (previousDrops < drops) // if new drop
       { 
       Serial.print("drop count = "); // print cont on the screen
       Serial.println(drops); // print count on the screen
       attachInterrupt(digitalPinToInterrupt(2), event, RISING);
       }   
    previousDrops = drops;
    delay(10);
}

void event() {
   detachInterrupt(digitalPinToInterrupt(2));
   drops += 1; //increment number of drops
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

