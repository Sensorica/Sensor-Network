/* This sketch uses a digital pin out to drive the LED of an optical limit switch and an interrupt pin from the Arduino 
 *  board (2 in this case) that is connected to the photodetector of the same optical limit switch.  
 * This setup works well with an input voltage to the interrupt pin between 4.1 and 4.3 V. Lower than that it misses triggers
 * higher than that it fires multiple times. So the input voltage to the interrupt needs to be conditioned with a Schmitt trigger.
 * 
 * Tibi, www.SENSORICA.ca January 2015
 */


int LEDpin = 13;                    // output, lights the IR LED
volatile int drops = 0;             // define drop count variable
volatile boolean event = LOW;
unsigned int currentTime = 0;       // define variable for timestamping
unsigned int previousTime = 0;      // define variable for timestamping
unsigned int duration = 0;          // define variable for timestamping
#define CNT 5
int durationArray[CNT]; // define array of durations between drop events
float avrgPeriode = 0; // is the periode or the inverse of the droprate. 
      
void setup() {
    Serial.begin(9600);             //  setup serial
    pinMode(LEDpin, OUTPUT);        // sets the digital pin as output
    attachInterrupt(digitalPinToInterrupt(2), drop, RISING); // IMPORTANT: reattach pin to interrupt for the next event 
}

void loop() {
   
  if (event == HIGH){                      // if new drop leading edge
    
      detachInterrupt(digitalPinToInterrupt(2));
      currentTime = millis();              // timestamp this event
      duration = currentTime-previousTime;  // calculate duration
              
      // Rolling average of durations update
      durationArray[drops-1] = duration; // put the last duration into durationsArray
      
      /*  *** Printing stuff for verrification ***
      Serial.print("array elements : "); // print count on the screen
      Serial.print(durationArray[0]); // print count on the screen
      Serial.print("; "); // print count on the screen
      Serial.print(durationArray[1]); // print count on the screen
      Serial.print("; "); // print count on the screen
      Serial.print(durationArray[2]); // print count on the screen
      Serial.print("; "); // print count on the screen
      Serial.print(durationArray[3]); // print count on the screen
      Serial.print("; "); // print count on the screen
      Serial.println(durationArray[4]); // print count on the screen
      */  
        
      //sum durationArray
      avrgPeriode = 0;
      for (int x = 0; x < CNT; x++){
      avrgPeriode = avrgPeriode + durationArray[x];  
       }
      //calculate the average
      avrgPeriode = avrgPeriode / CNT;

      if (drops >= CNT){
       drops = 0;
      }
        
      previousTime = currentTime;           // reset previousTime for next duration
      event = LOW;
          
      // testing prints...  
      Serial.print("avrgPeriode = ");     // print cont on the screen
      Serial.println(avrgPeriode); // print count on the screen
      //Serial.print("drop count = ");     // print cont on the screen
      //Serial.println(drops); // print count on the screen
         
     
      attachInterrupt(digitalPinToInterrupt(2), drop, RISING);
      } //  if loop
               
    } // Void loop
 


void drop() {
  
   //detachInterrupt(digitalPinToInterrupt(2)); // IMPORTANT - detach interrupt and reattach it in the main look, to avoid false triggers
   drops += 1; //increment number of drops
   event = HIGH;
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

