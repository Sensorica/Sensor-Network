
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           OneWire temperature sensor 
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
// OneWire variables
unsigned long lastTempRequest = 0;
int  delayInMillis = 0;
float ambient_temperature = 0.0;
float casing_temperature = 0.0;
int  idle = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Tachometer 
const byte TACH = 2;
const byte DURATION = 1;
volatile boolean revolution_occured = false;
unsigned int revolution_count = 0;
unsigned int current_time = 0;
unsigned int previous_time = 0;
unsigned int rpm = 0;

// Tachometer Interrupt Service Routine (ISR)
void revolution ()
{
  revolution_occured = true;
}  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  Drip Sensor
const byte LED = 13;
const byte SENSOR = 3;
volatile boolean drop_occured = false;
int drop_count = 0;

// Drip sensor Interrupt Service Routine (ISR)
void drop ()
{
  drop_occured = true;
} 
///////////////////////////////////////////////////////////////////////////////////////////////////////////// 


void setup()
{
  Serial.begin(115200);   //Enable serial at high speed 
  
  // Tachometer setup
  digitalWrite (TACH, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (digitalPinToInterrupt(2), revolution, RISING);  // attach interrupt handler for tachometer
  
  // OneWire sensor setup
  sensors.begin();
  if (!sensors.getAddress(ambientTemperature, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(casingTemperature, 1)) Serial.println("Unable to find address for Device 1"); 
  sensors.setResolution(ambientTemperature, TEMPERATURE_PRECISION);
  sensors.setResolution(casingTemperature, TEMPERATURE_PRECISION);
  sensors.setWaitForConversion(false);
  
  // Drip sensor setup
  digitalWrite (SENSOR, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (0, drop, RISING);  // attach interrupt handler
  
  
} 


void loop()
{
  dripSensor.Update();
}


