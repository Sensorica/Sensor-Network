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
  shaftTemp.Update();
  ambientTemp.Update();
  packingTemp.Update();
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
            currentValue = analogRead(sensorPin);    // read the input pin
   
                 if ((previousValue < 150) && (currentValue > 250))
                     { 
                     Serial.print("drip count = ");
                     Serial.println(drip);
                     drip += 1; 
                     }
                  previousValue = currentValue; 
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


