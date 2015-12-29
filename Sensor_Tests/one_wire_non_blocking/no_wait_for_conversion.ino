#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 4


OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

DeviceAddress tempDeviceAddress;

int  resolution = 12;
unsigned long lastTempRequest = 0;
int  delayInMillis = 0;
float temperature = 0.0;
int  idle = 0;

void setup(void)
{
  Serial.begin(115200);
  
  sensors.begin();
  sensors.getAddress(tempDeviceAddress, 0);
  sensors.setResolution(tempDeviceAddress, resolution);
  
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
    Serial.println(temperature, resolution - 8); 
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