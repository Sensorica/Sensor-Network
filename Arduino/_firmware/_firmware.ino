unsigned int long PRINT_DELAY = 2000; //in milliseconds
unsigned int long last_print_time = 0;
String data; 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           OneWire temperature sensor 
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 4
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
  Serial.begin(9600);   //Enable serial at high speed 
  
  // Tachometer setup
  digitalWrite (TACH, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (0, revolution, RISING);  // attach interrupt handler for tachometer
  
  // OneWire sensor setup
  sensors.begin();
  if (!sensors.getAddress(ambientTemperature, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(casingTemperature, 1)) Serial.println("Unable to find address for Device 1"); 
  sensors.setResolution(ambientTemperature, TEMPERATURE_PRECISION);
  sensors.setResolution(casingTemperature, TEMPERATURE_PRECISION);
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
  delayInMillis = 750 / (1 << (3)); 
  lastTempRequest = millis(); 
  
  // Drip sensor setup
  digitalWrite (SENSOR, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (1, drop, RISING);  // attach interrupt handler  
} 


void loop()
{
   ////////////////////////////////////////////////////////Tachometer 
   // Check if tach was triggered
   if (revolution_occured) {
   revolution_count ++;
   revolution_occured = !revolution_occured;
   }   
 
 // Calculate RPM
 current_time = millis();
   if (current_time - previous_time >= 1000) {
     rpm = (revolution_count / DURATION * 60);
     previous_time = current_time;
     revolution_count = 0;
   } 
  
  /////////////////////////////////////////////////////////Temperature
  
  if (millis() - lastTempRequest >= delayInMillis) // waited long enough??
  {
    ambient_temperature = sensors.getTempCByIndex(0);
    casing_temperature = sensors.getTempCByIndex(1);
    sensors.requestTemperatures(); 
    delayInMillis = 750 / (1 << (3));
    lastTempRequest = millis(); 
  }
  
  ///////////////////////////////////////////////////////Drops
  
  if (drop_occured) {
    drop_count += 1;
    drop_occured = false;
    }    
  
  //////////////////////////////////////////////////////Printing
  
  if (millis() - last_print_time >= PRINT_DELAY){
    data = "";
    data += ambient_temperature;
    data += ",";
    data += casing_temperature;
    data += ",";
    data += drop_count;
    data += ",";
    data += rpm;
    
    Serial.println (data);
    
    last_print_time = millis();
  }
}


