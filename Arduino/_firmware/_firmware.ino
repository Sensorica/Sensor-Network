//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           SENSOR NETWORK FIRMWARE

// This is the code for the sensor node. It integrates the following SENSORS an prints data to the serial monitor. 
// 1. Temperature Sensors.
//      At various locations around the pump
// 2. Tachometer
//      Measuring the rpms of the 
// 3. Load Washers 1 & 2
// 4. Level Sensor

//The following are driven based on the sensor values:
// 5. Solenoid Drain Valve
// 6. The Serial Monitor
//      The data from each sensor is printed on the serial monitor. The data on the serial monitor will be picked up
//      by the Zigbee module and will be available on the baseNode





//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           1. TEMPERATURE SENSOR
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
//                                                  2. TACHOMETER
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
//                                                 3. LOAD WASHER 1 & 2

int loadWasher1Pin = A0;
int loadWasher2Pin = A1;

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//We assume that the two load washers have the same calibration curve
//X is the applied weight in lbs
//A is our linear rate in Arduino ADC levels / lbs
//B is our reference level as applied by the amplifyer circuit
double A_linear_rate = 0.023;
double B_reference_level = 405;

//Initialize variables
const int load_sample_window = 100;
double load1_samples[load_sample_window];
double load2_samples[load_sample_window];
int index_i=0;
double load1_value = 0;
double load2_value = 0;
double load1_in_lbs = 0;
double load2_in_lbs = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                4. LEVEL SENSOR

int levelSensorPin = A2;

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//X is the water depth in cm
//A is the calibration slope in levels / cm
//B is the reference level
float A_water_rate = 12.125;
float B_water_reference_level = 789.25;
float vesicle_diameter_cm = 11.43;//4.5 inch x 2.54 cm / inch
float vesicle_area_cm_2 = (3.14159) * (vesicle_diameter_cm / 2) * (vesicle_diameter_cm / 2);

//Initialize variables
boolean initialize = true;
int analog_water_level = 0;
float water_level_cm = 0;
float flow_calc_water_level_cm = 0;
unsigned long calc_time_ms = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                5. SOLENOID DRAIN VALVE


int solenoidValvePin = 5; //Pin controlling the valve
int drain_at_cm_level = 12.7; //5 inches 
int close_valve_at_cm_level = 5.08; //2 inches

boolean solenoid_valve_open = false;
float flow_rate_cc_per_min = 0;
float water_level_buffer=1; //The water level buffer is how much the water level should increase before calculating a new flow rate



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           6. SERIAL MONITOR
unsigned int long PRINT_DELAY = 2000; //in milliseconds
unsigned int long last_print_time = 0;
String data; 



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           7. SHAFT TEMPERATURE SENSOR
#define  ClockPin  6  // clock in
#define  DataPin   7  //data in  
#define  TestPin   8 //driver pin  set pin low to start deliver data
///////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void setup()
{
  Serial.begin(9600);   //Enable serial at high speed 
  analogReference(EXTERNAL);
  
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


//Initialize load washer samples
for (int i = 0; i<load_sample_window; i++){
  load1_samples[i]=0;
  load2_samples[i]=0;


  pinMode(DataPin,INPUT);
  pinMode(ClockPin,INPUT);
  pinMode(TestPin,OUTPUT);
  digitalWrite(TestPin, HIGH);
  delay(100);
}
  
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
 
 load1_samples[index_i] = analogRead(loadWasher1Pin);
 load2_samples[index_i] = analogRead(loadWasher2Pin);
 index_i = (index_i + 1)%load_sample_window;

 load1_value = 0;
 for (int j = 0; j < load_sample_window; j++){
  load1_value+= load1_samples[j];
 }
 load1_value = load1_value / load_sample_window;


 load2_value = 0;
 for (int j = 0; j < load_sample_window; j++){
  load2_value+= load2_samples[j];
 }
 load2_value = load2_value / load_sample_window;




 
 load1_in_lbs = ( load1_value - B_reference_level ) / A_linear_rate;
 load2_in_lbs = ( load2_value - B_reference_level ) / A_linear_rate;

 //TODO: Send this value to the cloud for storage and analytics


 //////////////////////////////////////////////////////Water Level Sensor
 
 analog_water_level = analogRead(levelSensorPin);
 water_level_cm = ( analog_water_level - B_water_reference_level ) / A_water_rate;

//Set the reference water level for a calculation (when appropriate)

if(initialize==true){
  flow_calc_water_level_cm = water_level_cm;
  calc_time_ms = millis();
  initialize=false;
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


   float water_level_delta = analog_water_level - flow_calc_water_level_cm;
   unsigned long time_delta = millis() - calc_time_ms;
   flow_rate_cc_per_min = ( water_level_delta ) / (time_delta);
//   flow_calc_water_level_cm = analog_water_level;
  }
 }

  if (water_level_cm <= close_valve_at_cm_level){
  digitalWrite(solenoidValvePin,LOW);
  solenoid_valve_open = false;
  initialize=true;
 }


  //////////////////////////////////////////////////////Shaft Temperature 
  int data_buf[5] = {0};
  int tempData = 0;
 
  data_read(data_buf);
  tempData = data_buf[1]*256 + data_buf[2];
  float realTemp = (float)tempData/16-273.15;

  //////////////////////////////////////////////////////Printing
  
  if (millis() - last_print_time >= PRINT_DELAY){
    data = "";
//    data += ambient_temperature;
//    data += ",";
//    data += realTemp;
//    data += ",";
//    data += casing_temperature;
//    data += ",";
    data += rpm;
    data += ",";
    data += load1_in_lbs;
    data += ",";
    data += load2_in_lbs;
    data += ",";
    data += analog_water_level;
    data += ",";
    data += water_level_cm;
    data += ",";
    data += flow_rate_cc_per_min;
    
    Serial.println (data);
    
    last_print_time = millis();
  }
}

void data_read(int *p)
{
  int i,j,PinState,tempData;
   
  digitalWrite(TestPin, LOW);
  for(i=0;i<5;i++)
  {
  for(j=0;j<8;j++)
  {
    do{
      PinState = digitalRead(ClockPin);
    }while(PinState);
    delayMicroseconds(100);
    PinState = digitalRead(DataPin);
    if(1 == PinState)
      tempData = (tempData<<1 & 0xfe)+1;
    else
      tempData = (tempData<<1 & 0xfe);
    do{
      PinState = digitalRead(ClockPin);
    }while(PinState != 1);
  }
  *p++ = tempData;
  }
  digitalWrite(TestPin, HIGH);
}


