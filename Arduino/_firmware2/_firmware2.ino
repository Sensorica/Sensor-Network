//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           SENSOR NETWORK FIRMWARE
//
// This is the code for the sensor node. It integrates the following SENSORS/ACTUATORS 
//  and prints data to the serial monitor. 
// 1. Temperature Sensors.
//      At various locations around the pump
// 2. Shaft Temperature Sensor
//      This uses an IR temperature sensor and is measured with an interrupt on the rising edge of 
//      the sensor's waveform.
// 3. Tachometer
//      Measuring the rpms of the motor shaft. It has an optical sensor that measures the black mark
//      on the rotating shaft to determine each revolution. It also triggers an interrupt on the rising 
//      edge of the sensor's waveform.
// 4. Load Washers 1 & 2
// 5. Level Sensor
// 6. Solenoid Drain Valve
// 7. The Serial Monitor
// 8. Flow Sensor. 
//      The data from each sensor is printed on the serial monitor. The data on the serial monitor will be picked up
//      by the Zigbee module and automatically transmitted to the base node. (Remember to toggle switch on shield to SERIAL)
// 9. Software Serial to FFT
//10. Node Metadata



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            1. TEMPERATURE SENSORS
#include <SoftwareSerial.h>
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

int loadWasher1Pin = A4;
int loadWasher2Pin = A5;

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//We assume that the two load washers have the same calibration curve
//X is the applied weight in lbs
//A is our linear rate in Arduino ADC levels / lbs
//B is our reference level as applied by the amplifyer circuit
double A_linear_rate = 0.023;
double B_reference_level1 = 508; // Reference level for load1
double B_reference_level2 = 504; // Reference level for load2

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

int levelSensorPin = A3;

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
//                                              8. FLOW SENSOR


// which pin to use for reading the sensor? can use any pin!
#define FLOWSENSORPIN 3

// count how many pulses!
volatile uint16_t pulses = 0;
long int last_pulses = 0;
// track the state of the pulse pin
volatile uint8_t lastflowpinstate;
// you can try to keep time of how long it is between pulses
volatile uint32_t lastflowratetimer = 0;
// and use that to calculate a flow rate
volatile float flowrate;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
float liters = 0;
bool flowTrigger = false;

void flowCalc(){
  flowTrigger = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               9. SOFTWARE SERIAL TO FFT


SoftwareSerial mySerial(9, 11); // RX, TX


///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                              10. NODE METADATA

long int NODE_ID = 1;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            11. POSITION SENSOR

int positionSignal1 = A2;
int positionSignal2= A1;
double position1 = 0;
double position2 = 0;
double position1_mm = 0;
double position2_mm = 0;



void setup()
{
  
  Serial.begin(9600);        //Enable serial at high speed 
  analogReference(EXTERNAL); //Important or we send 5V into the 3V3 pin!
  
  // Tachometer setup
  pinMode(solenoidValvePin,OUTPUT);
  digitalWrite (TACH, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (digitalPinToInterrupt(2), revolution, RISING);  // attach interrupt handler for tachometer
  attachInterrupt (digitalPinToInterrupt(3), flowCalc, RISING);  // attach interrupt handler for flow meter
  
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
  



  //Liquid Flow Sensor Setup
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
//   useInterrupt(true);

   delay(100);
   
   // Software Serial to FFT micro
 mySerial.begin(4800);

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
// Serial.print (load1_samples[index_load]);
// Serial.print (" ");
// Serial.println (load2_samples[index_load]);
 
 index_load = (index_load + 1)%load_sample_window;
 
 
  //////////////////////////////////////////////////////Water Level Sensor //////////////////////////////////////
 

 analog_water_level = analogRead(levelSensorPin);
// Serial.print ("Water level: ");
// Serial.println (analog_water_level);
// delay (250);
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
  //Serial.println("Re-initializing water states");
  flow_calc_water_level_cm = water_level_cm;
  calc_time_ms = millis();
  initialize=false;
}

 //////////////////////////////////////////////////////Solenoid Drain Valve ////////////////////////////////////

 //Start the drain if the water level is too high
 if(water_level_cm >= drain_at_cm_level){
  //TODO: If water level is way too high, then water is accumulating so send the operator a warning message
  digitalWrite(solenoidValvePin,HIGH);
  solenoid_valve_open = true;
 }


//Calculate the flow rate
 if (solenoid_valve_open == false){ 
  if (water_level_cm >= (flow_calc_water_level_cm + water_level_buffer)){
    //Serial.println("CALCULATION TRIGGERED!");
   double water_level_delta = water_level_cm - flow_calc_water_level_cm;
   double time_delta = millis() - calc_time_ms;
   //Serial.println(water_level_delta);
  // Serial.println(time_delta/1000);
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

 //////////////////////////////////////////////////////Flow Rate Sensor //////////////////////////////////////


//   Serial.print("Freq: "); Serial.println(flowrate);
//  Serial.print("Pulses: "); Serial.println(pulses, DEC);
  
  // if a plastic sensor use the following calculation
  // Sensor Frequency (Hz) = 7.5 * Q (Liters/min)
  // Liters = Q * time elapsed (seconds) / 60 (seconds/minute)
  // Liters = (Frequency (Pulses/second) / 7.5) * time elapsed (seconds) / 60
  // Liters = Pulses / (7.5 * 60)

if (flowTrigger){
  pulses++;
  flowTrigger = !flowTrigger;
}

  
  liters = pulses;
  liters /= 7.5;
  liters /= 60.0;



///////////////////////////////////////////////////Position Sensor ////////////////////////////////////////////


 position1 = analogRead(positionSignal1);
 position2 = analogRead(positionSignal2);

 //TODO calculation to get position in cm
  position1_mm = position1 * 12.7 / 1023 ;//12.7mm is the max range of the displacement sensor
  position2_mm = position2 * 12.7 / 1023; // 1023 is the highest adc number

//END OF ZERO DELAY LOOP
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////DELAY LOOPS  //////////////////////////////////////////////////////////
  // Note: ALL CODE THAT HAS A DELAY SHOULD GO HERE!!!
  // Every PRINT_DELAY, disable ISR and do calculations which otherwise block the flow of the main loop,
  // including printing to serial that sends data to relay node.
  

  if (millis() - last_print_time >= PRINT_DELAY){
    detachInterrupt(0);
    detachInterrupt(1);
     duration = millis() - last_print_time;
     rpm = revolution_count / (duration/1000) * 60 ;
     revolution_count = 0;

//  flow rate code
    flowrate = (pulses - last_pulses) / (duration/1000) ; //Hz
    flowrate = (flowrate + 3)/8.1; // L/min
    last_pulses = pulses;
    


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
    data += NODE_ID;
    data += ",";
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
    data += load1_in_lbs; //Load Washers
    data += ",";
    data += load2_in_lbs; //Load Washers
    data += ",";
    data += flow_rate_cc_per_sec; //This is for the level sensor rate
    data += ",";
    data += flowrate; // This is for the flow sensor
    data += ",";
    data += position1_mm; //Displacement sensor1
    data += ",";
    data += position2_mm; //Displacement sensor2

    //Signal the FFT to take measurements
    mySerial.write("!");

    long int counter =0;
    //Wait for the response from the FFT
    while(!mySerial.available()){ 
      if(counter>=100000){
        break;   //Break after a while to avoid deadlock
      }
      counter++;
    }

  Serial.print(data);

  //This loop prints the data from the FFT. The waits 
for (int i = 0; i < 50; i++){
    while(mySerial.available()){
      Serial.write(mySerial.read());
    }
    delay(5); //This delay help wait until the mySerial is available (not sure what the optimal value is here)
}
   
    Serial.println("");
    
    //////////////////////////////////////////////////////Go back to Zero Delay Loop/////////////////////////////////////////////
    attachInterrupt (0, revolution, RISING);  // re-attach interrupt handler for tachometer
    attachInterrupt (1, flowCalc, RISING);  // re-attach interrupt handler for flush flow rate sensor
    last_print_time = millis();
    revolution_occured = false;
    //pulses = 0;
    lastflowratetimer = 0;
  }
}







