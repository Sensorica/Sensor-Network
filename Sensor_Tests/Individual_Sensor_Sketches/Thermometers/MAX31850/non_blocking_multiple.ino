//
// Sample of using Async reading of Dallas Temperature Sensors with two sensors
// 
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

DeviceAddress casingTemperature, ambientTemperature;

unsigned long lastTempRequest = 0;
int  delayInMillis = 0;
float ambient_temperature = 0.0;
float casing_temperature = 0.0;
int  idle = 0;
//
// SETUP
//
void setup(void)
{
  Serial.begin(115200);
  Serial.println("Dallas Temperature Control Library - Async Demo with two sensors");
  Serial.print("Library Version: ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.println("\n");

  sensors.begin();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  
  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  if (!sensors.getAddress(ambientTemperature, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(casingTemperature, 1)) Serial.println("Unable to find address for Device 1"); 
  
  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(ambientTemperature);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(casingTemperature);
  Serial.println();
  
  sensors.setResolution(ambientTemperature, TEMPERATURE_PRECISION);
  sensors.setResolution(casingTemperature, TEMPERATURE_PRECISION);
  
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(ambientTemperature), DEC); 
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(casingTemperature), DEC); 
  Serial.println();
  
  sensors.setWaitForConversion(false);
  
  sensors.requestTemperatures();
  delayInMillis = 750 / (1 << (3)); 
  lastTempRequest = millis(); 
  
  pinMode(13, OUTPUT); 
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();    
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void loop(void)
{ 
  
  if (millis() - lastTempRequest >= delayInMillis) // waited long enough??
  {
    digitalWrite(13, LOW);	
    Serial.print("Ambient temperature: ");
    ambient_temperature = sensors.getTempCByIndex(0);
    Serial.println(ambient_temperature, 1); 
	
    Serial.print("Casing temperature: ");
    casing_temperature = sensors.getTempCByIndex(1);
    Serial.println(casing_temperature, 1); 
    Serial.print("Idle counter: ");
	
    Serial.println(idle);     
    Serial.println();     
    idle = 0;     
    sensors.requestTemperatures(); 
    delayInMillis = 750 / (1 << (3));
    lastTempRequest = millis(); 
  }
  
  digitalWrite(13, HIGH);
  // we can do usefull things here 
  // for the demo we just count the idle time in millis
  delay(1);
  idle++;
}
