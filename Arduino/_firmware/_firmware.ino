//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           SENSOR NETWORK FIRMWARE
//
// This is the code for the sensor node. It integrates the following SENSORS an prints data to the serial monitor. 
// 1. Temperature Sensors.
//      At various locations around the pump
// 2. Shaft Temperature Sensor
// 3. Tachometer
//      Measuring the rpms of the motor shaft
// 4. Load Washers 1 & 2
// 5. Level Sensor

//The following are driven based on the sensor values:
// 5. Solenoid Drain Valve
// 6. The Serial Monitor
//      The data from each sensor is printed on the serial monitor. The data on the serial monitor will be picked up
//      by the Zigbee module and automatically transmitted to the base node. (Remember to toggle switch on shield to SERIAL) 



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            1. TEMPERATURE SENSORS
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 4                 // Data wire is plugged into port 2 on the Arduino
#define TEMPERATURE_PRECISION 9        // 9 bits or one decimal point

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);        

DallasTemperature sensors(&oneWire);                  // Pass our oneWire reference to Dallas Temperature.
DeviceAddress casingTemperature, ambientTemperature, glandTemperature;   // OneWire variables
unsigned long lastTempRequest = 0;
int  delayInMillis = 0;
float ambient_temperature = 0.0;
float casing_temperature = 0.0;
float gland_temperature = 0.0;
int  idle = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           2. SHAFT TEMPERATURE SENSOR
//
// This device uses a 2KHz SPI protocol and has no memory scratchpad to read from so data must be read sequentially
// according to its clock speed It takes approximately 470ms for a reading and the data is five bytes long.

#define  ClockPin  6  // clock in
#define  DataPin   7  //data in  
#define  TestPin   8  //driver pin  set pin low to start deliver data

float shaftTemp = 0.0;

void data_read(int *p)
{
  int i,j,PinState,tempData;   
  unsigned long time_check=millis();
  digitalWrite(TestPin, LOW);              //start device transmission
  for(i=0;i<5;i++) {                       //read 5 bytes
    for(j=0;j<8;j++){                      //read 8 bits
      do{
        PinState = digitalRead(ClockPin);
      } while(PinState);                   //sync to clock by waiting for change to LOW 
      delayMicroseconds(100); //buffer
      PinState = digitalRead(DataPin);     //read data bit
      if(1 == PinState)                    
        tempData = (tempData<<1 & 0xfe)+1; //add 1
      else
        tempData = (tempData<<1 & 0xfe);   //add 0
      do{
        PinState = digitalRead(ClockPin);  //get stuck until clock changes
        
        if ((millis() - time_check) >= 2000){//Haha, let's NOT get stuck if the thing is unplugged
//          Serial.println("break at 20ms");
          break;
        }
      }while(PinState != 1);
    }
  *p++ = tempData;                         //pass byte to array pointer address 
  }
  digitalWrite(TestPin, HIGH);             //stop device transmission
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  3. TACHOMETER
const byte TACH = 2;
double duration = 1;
volatile boolean revolution_occured = false;   
double revolution_count = 0;
unsigned int current_time = 0;
unsigned int previous_time = 0;
double rpm = 0;

// Tachometer Interrupt Service Routine (ISR)

void revolution (){
  revolution_occured = true;
  }  

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                 4. LOAD WASHER 1 & 2

int loadWasher1Pin = A0;
int loadWasher2Pin = A1;

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//We assume that the two load washers have the same calibration curve
//X is the applied weight in lbs
//A is our linear rate in Arduino ADC levels / lbs
//B is our reference level as applied by the amplifyer circuit
double A_linear_rate = 0.023;
double B_reference_level1 = 357; // Reference level for load1
double B_reference_level2 = 352; // Reference level for load2

//Initialize variables
const int load_sample_window = 70;
double load1_samples[load_sample_window];
double load2_samples[load_sample_window];
int index_load=0;
int index_water=0;
double load1_value = 0;
double load2_value = 0;
double load1_in_lbs = 0;
double load2_in_lbs = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                5. LEVEL SENSOR

int levelSensorPin = A2;

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//X is the water depth in cm
//A is the calibration slope in levels / cm
//B is the reference level
double A_water_rate = 12.125;
double B_water_reference_level = 789.25;
double vesicle_diameter_cm = 11.43;//4.5 inch x 2.54 cm / inch
double vesicle_area_cm_2 = (3.14159) * (vesicle_diameter_cm / 2) * (vesicle_diameter_cm / 2);

//Initialize variables
boolean initialize = true;
int analog_water_level = 0;
const int water_level_sample_window = 30;
double water_level_samples[water_level_sample_window];
double water_level_cm = 0;
double flow_calc_water_level_cm = 0;
unsigned long calc_time_ms = 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                6. SOLENOID DRAIN VALVE


int solenoidValvePin = 5; //Pin controlling the valve
int drain_at_cm_level = 12; //5 inches 
int close_valve_at_cm_level = 5.08; //2 inches

boolean solenoid_valve_open = false;
float flow_rate_cc_per_sec = 0;
float water_level_buffer=1; //The water level buffer is how much the water level should increase before calculating a new flow rate



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                7. SERIAL MONITOR
unsigned int long PRINT_DELAY = 2000; //in milliseconds
unsigned int long last_print_time = 0;
String data; 



///////////////////////////////////////////////////////////////////////////////////////////////////////////// 

void setup()
{
  
  Serial.begin(9600);        //Enable serial at high speed 
  analogReference(EXTERNAL); //Important or we send 5V into the 3V3 pin!
  
  // Tachometer setup
  pinMode(solenoidValvePin,OUTPUT);
  digitalWrite (TACH, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (digitalPinToInterrupt(2), revolution, RISING);  // attach interrupt handler for tachometer
  
  // OneWire sensor setup
  sensors.begin();
  if (!sensors.getAddress(ambientTemperature, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(casingTemperature, 1)) Serial.println("Unable to find address for Device 1"); 
  if (!sensors.getAddress(glandTemperature, 2)) Serial.println ("Unable to find address for Device 2");
  sensors.setResolution(ambientTemperature, TEMPERATURE_PRECISION);
  sensors.setResolution(casingTemperature, TEMPERATURE_PRECISION);
  sensors.setResolution(glandTemperature, TEMPERATURE_PRECISION);
  sensors.setWaitForConversion(false);      //Important undocumented feature that is only needed for "parasitic mode"
  sensors.requestTemperatures();
  delayInMillis = 750 / (1 << (3)); 
  lastTempRequest = millis(); 


  //Initialize load washer samples array
  for (int i = 0; i<load_sample_window; i++){
    load1_samples[i]=0;
    load2_samples[i]=0;
  }

  //Initialize the water level samples array
  for (int i = 0; i<water_level_sample_window; i++){
    water_level_samples[i]=0;
  }
	
	
  //Initialize SEN0093 pins (shaft temperature)
  pinMode(DataPin,INPUT);
  pinMode(ClockPin,INPUT);
  pinMode(TestPin,OUTPUT);
  digitalWrite(TestPin, HIGH);
  delay(100);
  
} 


void loop()
{

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////// ZERO DELAY LOOP ////////////////////////////////////////////////////
  // NOTE: DO NOT ADD FUNCTIONS THAT ADD DELAYS OR BLOCK INTERRUPTS HERE
  
   ////////////////////////////////////////////////////////Tachometer 
   // Check if tach was triggered
   if (revolution_occured) {
     revolution_count ++;
     revolution_occured = !revolution_occured;
   }   
 
  
  //////////////////////////////////////////////////////Load Washer

 //Take the samples each loop - Averaging done in the interrupt
 load1_samples[index_load] = analogRead(loadWasher1Pin);
 load2_samples[index_load] = analogRead(loadWasher2Pin);
 index_load = (index_load + 1)%load_sample_window;

 
  //////////////////////////////////////////////////////Water Level Sensor
 

 analog_water_level = analogRead(levelSensorPin);

// Stores samples
 water_level_samples[index_water] = ( analog_water_level - B_water_reference_level ) / A_water_rate;

////take the average over the window
//if(index_water==0){
 for (int i = 0 ; i< water_level_sample_window; i++){
  water_level_cm+= water_level_samples[index_water];
 }
 water_level_cm = water_level_cm / water_level_sample_window;
//}
 
 index_water = index_water%water_level_sample_window;

////Set the reference water level for a calculation (when appropriate)
if(initialize==true){
  Serial.println("Re-initializing water states");
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


//Calculate the flow rate
 if (solenoid_valve_open == false){ 
  if (water_level_cm >= (flow_calc_water_level_cm + water_level_buffer)){
    Serial.println("CALCULATION TRIGGERED!");
   double water_level_delta = water_level_cm - flow_calc_water_level_cm;
   double time_delta = millis() - calc_time_ms;
   Serial.println(water_level_delta);
   Serial.println(time_delta/1000);
   flow_rate_cc_per_sec = ( water_level_delta ) / (time_delta/1000);

   //set the last calculation references
   flow_calc_water_level_cm = water_level_cm;
   calc_time_ms = millis();
  }
 }

  if (solenoid_valve_open == true && water_level_cm <= close_valve_at_cm_level){
  digitalWrite(solenoidValvePin,LOW);
  solenoid_valve_open = false;
  initialize=true;
 }



//END OF ZERO DELAY LOOP
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////DELAY LOOPS  //////////////////////////////////////////////////////////
  // Note: ALL CODE THAT HAS A DELAY SHOULD GO HERE!!!
  // Every PRINT_DELAY, disable ISR and do calculations which otherwise block the flow of the main loop,
  // including printing to serial that sends data to relay node.
  

  if (millis() - last_print_time >= PRINT_DELAY){
    detachInterrupt(0);
     duration = millis() - last_print_time;
     rpm = revolution_count / (duration/1000) * 60 ;
     revolution_count = -1;


 /////////////////////////////////////////////////////////One-Wire Temperature Sensors
  
  if (millis() - lastTempRequest >= delayInMillis) {    // waited long enough??
    ambient_temperature = sensors.getTempCByIndex(0);
    casing_temperature = sensors.getTempCByIndex(1);
  gland_temperature = sensors.getTempCByIndex (2);
    sensors.requestTemperatures(); 
    delayInMillis = 750 / (1 << (3));
    lastTempRequest = millis(); 
  }
     
	
	//////////////////////////////////////////////////////Calculate load washer averages - ??? ms
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
 
    load1_in_lbs = ( load1_value - B_reference_level1 ) / A_linear_rate;
    load2_in_lbs = ( load2_value - B_reference_level2 ) / A_linear_rate;
		
    //////////////////////////////////////////////////////Shaft Temperature - 460 ms
    int data_buf[5] = {0};
    int tempData = 0; 
    data_read(data_buf);
    tempData = data_buf[1]*256 + data_buf[2];
    shaftTemp = (float)tempData/16-273.15;
	
	
	//////////////////////////////////////////////////////Printing 	  
	  data = "";
    data += shaftTemp;
    data += ",";
	  data += ambient_temperature;
    data += ",";
    data += casing_temperature;
    data += ",";
	  data += gland_temperature;
	  data += ",";
    data += rpm;
    data += ",";
    data += load1_in_lbs;
    data += ",";
    data += load2_in_lbs;
    data += ",";
    data += water_level_cm;
    data += ",";
    data += flow_rate_cc_per_sec;    
    
    Serial.println (data);

    //////////////////////////////////////////////////////Go back to Zero Delay Loop
	  attachInterrupt (0, revolution, RISING);  // re-attach interrupt handler for tachometer
    last_print_time = millis();
  }
}







