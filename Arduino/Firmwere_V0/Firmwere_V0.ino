/* First number is the pin, second is time to poll for and third is polling frequency, both in milliseconds.
Still need to add reporting to serial and some error checking (is sensor connected?) */
 
//AnalogSensor shaftTemp(10, 1, 500);
//AnalogSensor ambientTemp(11, 1, 1000);
//AnalogSensor packingTemp(12, 1, 100);
DigitalSensor dripSensor(14, 1, 10);

void setup()
{
} 
void loop()
{
 //shaftTemp.Update();
 //ambientTemp.Update();
 //packingTemp.Update();
 dripSensor.Update();
}


class DripSensor
{
  // Class Member Variables
  // These are initialized at startup
  int sensorPin;      // the number of the sensor pin
  long DelayTime;
 
  // These maintain the current state
  int previousValue; 
  int currentValue;// sensorData used to store the sensor data
  unsigned long previousMillis;  // will store last time sensorData was updated
  int drip;
  
public:
            DripSensor(int pin, long frequency)
                 {
            sensorPin = pin;
            DelayTime = frequency;
            previousValue = 0;
            previousMillis = 0;
            drip=1;       
                 }
void Update()
             {
            // check to see if it's time to read the analog sensor
            unsigned long currentMillis = millis();
   
            if (previousMillis >= DelayTime)
                {
            pinMode(LEDpin, OUTPUT);        // sets the digital pin as output
            attachInterrupt(digitalPinToInterrupt(2), eventR, RISING); // IMPORTANT: reattach pin to interrupt for the next event 
            eventsR = LOW;  
            eventsF = LOW;     
            
            if (eventsR == HIGH)              // if new drop leading edge
                { 
              detachInterrupt(digitalPinToInterrupt(2));
             if (eventsF == HIGH)          // if new drop falling edge
               {
             // detachInterrupt(digitalPinToInterrupt(2)); // IMPORTANT - detach interrupt and reattach it in the main look, to avoid false triggers          
             // currentTime = millis();           // timestamp this event
             //Serial.print("drop count = ");     // print cont on the screen
             Serial.println(drops); // print count on the screen
             // Serial.print(" ; duration = ");   // print cont on the screen
             // Serial.println(duration);         // print count on the screen
             //previousDrops = drops;  
            eventsR = LOW;
           eventsF = LOW;
           //duration = currentTime-previousTime;  // calculate duration
           // previousTime = currentTime;          // reset previousTime for next duration
           delay(20);
           //attachInterrupt(digitalPinToInterrupt(2), event, RISING); // IMPORTANT: reattach pin to interrupt for the next event 
          attachInterrupt(digitalPinToInterrupt(2), eventR, RISING);
              }
             else
              {
          delay(20);
          attachInterrupt(digitalPinToInterrupt(2), eventF, FALLING);
              } 
                }
                
          void eventR() {
          //detachInterrupt(digitalPinToInterrupt(2)); // IMPORTANT - detach interrupt and reattach it in the main look, to avoid false triggers
          eventsR = HIGH;
          }

          void eventF() {
          //detachInterrupt(digitalPinToInterrupt(2)); // IMPORTANT - detach interrupt and reattach it in the main look, to avoid false triggers
          drops += 1; //increment number of drops
          eventsF = HIGH;
          }
              }
                previousMillis = currentMillis - previousMillis;  // Remember the time
             }
            
};


DripSensor dripSensor(3,5);

void setup()
{
   Serial.begin(9600);
} 
void loop()
{
  dripSensor.Update();
}


