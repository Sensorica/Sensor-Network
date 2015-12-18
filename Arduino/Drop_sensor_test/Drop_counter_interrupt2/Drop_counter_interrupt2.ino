int LEDpin = 13; // output, lights the IR LED
volatile int drops = 0; // define drop count variable
//unsigned long previousDrops = 0; // define drop count variable
volatile boolean events = LOW;
//unsigned int currentTime = 0; // define variable for timestamping
//unsigned int previousTime = 0; // define variable for timestamping
//unsigned int duration = 0; // define variable for timestamping
volatile unsigned long last_micros;
      
void setup() {
    Serial.begin(9600);        //  setup serial
    pinMode(LEDpin, OUTPUT);   // sets the digital pin as output
    attachInterrupt(digitalPinToInterrupt(2), debounce_event, CHANGE); // IMPORTANT: reattach pin to interrupt for the next event 
    events = LOW;  
}

void loop() {
  
  //currentTime = millis(); // timestamp this event
  if (events == HIGH) // if new drop
    { 
     // detachInterrupt(digitalPinToInterrupt(2)); // IMPORTANT - detach interrupt and reattach it in the main look, to avoid false triggers          
     // currentTime = millis(); // timestamp this event
      //Serial.print("drop count = "); // print cont on the screen
      Serial.println(drops); // print count on the screen
     // Serial.print(" ; duration = "); // print cont on the screen
     // Serial.println(duration); // print count on the screen
      //previousDrops = drops;  
      events = LOW;
      //duration = currentTime-previousTime; // calculate duration
     // previousTime = currentTime; // reset previousTime for next duration
     // delay(2000);
      //attachInterrupt(digitalPinToInterrupt(2), event, RISING); // IMPORTANT: reattach pin to interrupt for the next event 
     
    }   
     
}

void debounce_event() {
  if((long)(micros()-last_micros) >= 7000){
  event(); 
   last_micros = micros();
  }
} 
void event() {
   //detachInterrupt(digitalPinToInterrupt(2)); // IMPORTANT - detach interrupt and reattach it in the main look, to avoid false triggers
   drops += 1; //increment number of drops
   events = HIGH;
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

