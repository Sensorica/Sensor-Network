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
//                                                 LOAD WASHER

int loadWasherPin = A0;
int analog_load_value = 0;
double load_in_lbs = 0;

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//X is the applied weight in lbs
//A is our linear rate in Arduino ADC levels / lbs
//B is our reference level as applied by the amplifyer circuit
double A_linear_rate = 0.01;
double B_reference_level = 483.854;

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                LEVEL SENSOR

boolean initialize = true;
int levelSensorPin = A1;
int analog_water_level = 0;
double water_level_cm = 0;
double flow_calc_water_level_cm = 0;
unsigned long calc_time_ms = 0;

double vesicle_diameter_cm = 11.43;//4.5 inch x 2.54 cm / inch
double vesicle_area_cm_2 = (3.14159) * (vesicle_diameter_cm / 2) * (vesicle_diameter_cm / 2);

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
double A_water_rate = 11.96;
double B_water_reference_level = 327.45;

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                SOLENOID DRAIN VALVE


int solenoidValvePin = 14; //Pin controlling the valve
int drain_at_cm_level = 5; 
int close_valve_at_cm_level = 1;

boolean solenoid_valve_open = false;
double flow_rate_cc_per_min = 0;
double water_level_buffer; //The water level buffer is how much the water level should increase before calculating a new flow rate

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void setup()
{
  Serial.begin(9600);   //Enable serial at high speed 
  
  // Tachometer setup
  pinMode(solenoidValvePin,OUTPUT);
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


//////////////////////////////////////////////////////Load Washer
 analog_load_value = analogRead(loadWasherPin);
 load_in_lbs = ( analog_load_value - B_reference_level ) / A_linear_rate;

 //TODO: Send this value to the cloud for storage and analytics


 //////////////////////////////////////////////////////Water Level Sensor
 
 analog_water_level = analogRead(levelSensorPin);
 water_level_cm = ( analog_water_level - B_water_reference_level ) / A_water_rate;

if(initialize==true){
  flow_calc_water_level_cm = water_level_cm;
  calc_time_ms = millis();
  initialize==false;
}

 

 //////////////////////////////////////////////////////Solenoid Drain Valve

 //Start the drain if the water level is too high
 if(water_level_cm >= drain_at_cm_level){
  //TODO: If water level is way too high, then water is accumulating so send the operator a warning message
  digitalWrite(solenoidValvePin,HIGH);
  solenoid_valve_open = true;
 }

 if (solenoid_valve_open == false){
  if (water_level_cm >= (flow_calc_water_level_cm + water_level_buffer)){
   flow_rate_cc_per_min = ( analog_water_level - flow_calc_water_level_cm ) / (millis() - calc_time_ms);
   flow_calc_water_level_cm = analog_water_level;
  }
 }

  if (water_level_cm <= close_valve_at_cm_level){
  digitalWrite(solenoidValvePin,LOW);
  solenoid_valve_open = false;
  initialize=true;
 }



  //////////////////////////////////////////////////////Printing
  
  if (millis() - last_print_time >= PRINT_DELAY){
    data = "";
    data += ambient_temperature;
    data += ",";
    data += casing_temperature;
    data += ",";
    data += rpm;
    data += ",";
    data += load_in_lbs;
    data += ",";
    data += water_level_cm;
    data += ",";
    data += flow_rate_cc_per_min;
    
    Serial.println (data);
    
    last_print_time = millis();
  }
}


