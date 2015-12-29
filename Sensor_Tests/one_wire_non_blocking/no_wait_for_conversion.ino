#include <OneWire.h>
#include <DallasTemperature.h>

// Digital pin that sensor is connected to
#define ONE_WIRE_BUS 4


OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//We will need to change this to code that checks how many sensors on the chain
DeviceAddress tempDeviceAddress;

//Setting up variables
int  resolution = 12;              //resolution from 9-12 bits
unsigned long lastTempRequest = 0; //when the last read occured 
int  delayInMillis = 0;            //how long to wait for a read
float temperature = 0.0;           //the temperature
int  idle = 0;                     //how long we stay in the main loop
int stripped_temp = 0;

void setup(void)
{
  
  Serial.begin(115200);
  
  sensors.begin();
  sensors.getAddress(tempDeviceAddress, 0);
  sensors.setResolution(tempDeviceAddress, resolution);
  
  // This is the library function that disables needing to hold the pin state
  sensors.setWaitForConversion(false);
  
  sensors.requestTemperatures();
  delayInMillis = 750 / (1 << (12 - resolution)); 
  lastTempRequest = millis(); 
  
  pinMode(13, OUTPUT); 
}

void loop(void)
{ 
  
  if (millis() - lastTempRequest >= delayInMillis) // waited long enough??
  {
    digitalWrite(13, LOW);
    Serial.print(" Temperature: ");
    temperature = sensors.getTempCByIndex(0);
    stripped_temp = (int) temperature
	Serial.println(stripped_temp); 
    Serial.print("  Resolution: ");
    Serial.println(resolution); 
    Serial.print("Idle counter: ");
    Serial.println(idle);     
    Serial.println(); 
    
    idle = 0; 
        
    resolution++;
    if (resolution > 12) resolution = 9;
    
    sensors.setResolution(tempDeviceAddress, resolution);
    sensors.requestTemperatures(); 
    delayInMillis = 750 / (1 << (12 - resolution));
    lastTempRequest = millis(); 
  }
  
  digitalWrite(13, HIGH);
  // we can do usefull things here 
  // for the demo we just count the idle time in millis
  delay(1);
  idle++;
}