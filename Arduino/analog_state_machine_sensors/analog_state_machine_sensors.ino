class DripSensor
{
	// Class Member Variables
	// These are initialized at startup
	int sensorPin;      // the number of the sensor pin
	long DelayTime;
 
	// These maintain the current state
	int previousValue; // sensorData used to store the sensor data
        int currentValue; // sensorData used to store the sensor data
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
  long a;
  long b;
  a = millis();
  
	dripSensor.Update();
  b = millis();
  Serial.println(b-a);
  delay(200);
}

