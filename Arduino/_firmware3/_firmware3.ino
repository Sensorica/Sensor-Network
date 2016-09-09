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
#include <SoftwareSerial.h>


//Analog sensors
//+5V Position Sensor
//+5 Film sensor
//+3.3V Fluid Leve
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            0. NODE ID
String NODE_ID = "1";
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            1. SERIAL MONITOR
unsigned int long PRINT_DELAY = 10000;
unsigned int long last_print_time = 0;
String data;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            2. POSITION SENSORs

int positionSignal1 = A0; //J9 Connector
int positionSignal2= A1;  //J10 Connector
double position1 = 0;
double position2 = 0;
String position1_mm = "0.0";
String position2_mm = "0.0";

void position_sensors(){
  position1 = analogRead(positionSignal1);
  position2 = analogRead(positionSignal2);
  position1_mm = String(position1 * 12.7 / 1023);
  position2_mm = String(position2 * 12.7 / 1023);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            3. FLUID LEVEL SENSOR
int levelSensorPin = A3;  //J6 Connector

int index_water=0;

//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//X is the water depth in cm
//A is the calibration slope in levels / cm
//B is the reference level
double A_water_rate = 5; // difference in bits per cm
double B_water_reference_level = 323;
double vesicle_diameter_cm = 11.43;//4.5 inch x 2.54 cm / inch
double vesicle_area_cm_2 = (3.14159) * (vesicle_diameter_cm / 2) * (vesicle_diameter_cm / 2);

//Initialize variables
boolean initialize = true;
int analog_water_level = 0;
const int water_level_sample_window = 30;
double water_level_samples[water_level_sample_window];
double water_level_total = 0;
double water_level_cm = 0;
double flow_calc_water_level_cm = 0;
unsigned long calc_time_ms = 0;

void fluid_level_sensor(){

  water_level_total -= water_level_samples[index_water];

  analog_water_level = analogRead(levelSensorPin);
  //Serial.println(analog_water_level);
  water_level_samples[index_water] = ( analog_water_level - B_water_reference_level ) / A_water_rate;
  water_level_total += water_level_samples[index_water];
  index_water++;
 
  if (index_water == water_level_sample_window) index_water = 0;
  water_level_cm = water_level_total / water_level_sample_window;

  //Serial.println(analog_water_level);
  //Serial.println(water_level_cm);


////take the average over the window
//if(index_water==0){
//for (int i = 0 ; i< water_level_sample_window; i++){
//  water_level_cm+= water_level_samples[index_water];
//  }
// water_level_cm = water_level_cm / water_level_sample_window;
////}
// index_water = index_water%water_level_sample_window;

////Set the reference water level for a calculation (when appropriate)
  if(initialize==true){
    //Serial.println("Re-initializing water states");
    flow_calc_water_level_cm = water_level_cm;
    calc_time_ms = millis();
    initialize=false;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                                4. SOLENOID DRAIN VALVE


int solenoidValvePin = 12; //Pin controlling the valve
int drain_at_cm_level = 15; //5 inches
int close_valve_at_cm_level = 5; //2 inches

boolean solenoid_valve_open = false;
String flow_rate_cc_per_sec = "0.0";
float water_level_buffer=1; //The water level buffer is how much the water level should increase before calculating a new flow rate

void solenoid_drain_valve(){
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
     flow_rate_cc_per_sec = String(( water_level_delta ) / (time_delta/1000));
  
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

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  5. TACHOMETER
const byte TACH = 2;
double duration = 1;
volatile boolean revolution_occured = false;   
double revolution_count = 0;
unsigned int current_time = 0;
unsigned int previous_time = 0;
String rpm = "0.0";

// Tachometer Interrupt Service Routine (ISR)

void revolution (){
  revolution_occured = true;
}

void tachometer_sensor(){
   // Check if tach was triggered
   if (revolution_occured) {
     revolution_count ++;
     revolution_occured = !revolution_occured;
   }   
}

void rpm_calculation(){
  detachInterrupt(0);
  detachInterrupt(1);
  duration = millis() - last_print_time;
  rpm = String(revolution_count / (duration/1000) * 60);
  revolution_count = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//                                              6. FLOW SENSOR


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

String flowrate_str = "0.0";

void flowCalc(){
  flowTrigger = true;
}

void flow_rate_sensor(){
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

}

void flow_rate(){
  //  flow rate code
  flowrate = (pulses - last_pulses) / (duration/1000) ; //Hz
  flowrate = (flowrate + 3)/8.1; // L/min
  flowrate_str = String(flowrate);
  last_pulses = pulses;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           7. SHAFT TEMPERATURE SENSOR
//
// This device uses a 2KHz SPI protocol and has no memory scratchpad to read from so data must be read sequentially
// according to its clock speed It takes approximately 470ms for a reading and the data is five bytes long.

#define  ClockPin  6  // clock in 
#define  DataPin   7  //data in  
#define  TestPin   8  //driver pin  set pin low to start deliver data

String shaftTemp = "";

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

void shaft_temp(){
    int data_buf[5] = {0};
    int tempData = 0;
    data_read(data_buf);
    tempData = data_buf[1]*256 + data_buf[2];
    shaftTemp = String((float)tempData/16-273.15);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            8. TEMPERATURE SENSORS
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
String ambient_temperature = "";
String casing_temperature = "";
String gland_temperature = "";
int  idle = 0;

void one_wire_temps(){
  // OneWire sensor setup
      // waited long enough??
    ambient_temperature = String(sensors.getTempCByIndex(0));
    casing_temperature = String(sensors.getTempCByIndex(1));
    gland_temperature = String(sensors.getTempCByIndex (2));
    sensors.requestTemperatures(); 
    delayInMillis = 750 / (1 << (3));
    lastTempRequest = millis(); 
 

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               9. SOFTWARE SERIAL TO FFT

SoftwareSerial mySerial(11, 9); // RX, TX

boolean data_received = false;
String data_mic = "";
String data_x = "";
String data_y = "";
String data_z = "";

void receive_FFT(){

  data_mic = "";
  data_x = "";
  data_y = "";
  data_z = "";

   //Serial.println("2.\t My serialwrite M");
   mySerial.write("M");
   for (int i = 0; i < 20; i++){
    //Serial.println("waiting");
      while(mySerial.available()){
        //Serial.println("available");
        data_mic += char(mySerial.read());
       }
       delay(5); //This delay help wait until the mySerial is available (not sure what the optimal value is here)
    }

   //Serial.println("2.\t My serialwrite x");

    mySerial.print("X");
    for (int i = 0; i < 20; i++){
      while(mySerial.available()){
        data_x += char(mySerial.read());
       }
       delay(5); //This delay help wait until the mySerial is available (not sure what the optimal value is here)
    }
    mySerial.print("Y");
   for (int i = 0; i < 20; i++){
      while(mySerial.available()){
        data_y += char(mySerial.read());
       }
       delay(5); //This delay help wait until the mySerial is available (not sure what the optimal value is here)
    }
    mySerial.print("Z");
   for (int i = 0; i < 20; i++){
      while(mySerial.available()){
        data_z += char(mySerial.read());
       }
       delay(5); //This delay help wait until the mySerial is available (not sure what the optimal value is here)
    }
    mySerial.print("E");
  
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                            10. LOAD FILM SENSOR
float v_in = 5.04;// Measure and correct the V in.
float load1_v_out = 0.0;
float load2_v_out = 0.0;
float res_1 = 50350.0; // Measure the value of R1 in voltage divider circuit.
float res_2 = 0.0;

const int loadSignal1 = A4;
const int loadSignal2 = A5;

float load1 = 0.0;
float load2 = 0.0;
float load1_val = 0.0;
float load2_val = 0.0;
float load_cap = 10000000.0;
String load1_res2 = "0.0";
String load2_res2 = "0.0";

const int numRead = 10;
float readings_load1[numRead];
float readings_load2[numRead];
int load_read_index = 0;
float total_load1 = 0.0;
float total_load2 = 0.0;
float ave_load1 = 0.0;
float ave_load2 = 0.0;



void film_sensor(){
  //Read analog signal
  load1 = float(analogRead(loadSignal1));
  load2 = float(analogRead(loadSignal2));
  //Convert analog signal to voltage
  load1_v_out = load1 / 1023 * v_in;
  load2_v_out = load2 / 1023 * v_in;
  //Calculate the resistance value of the film sensor
  load1_val = res_1 / (v_in / load1_v_out - 1.0);
  load2_val = res_1 / (v_in / load2_v_out - 1.0);
  //If value is higher than the cap, it sets to the cap value, because the value can reach INF
  if(load1_val > load_cap){
    load1_val = 10000000.0;
  }
  if(load2_val > load_cap){
    load2_val = 10000000.0;
  }

  /// Smoothing code starts
  total_load1 -= readings_load1[load_read_index];
  total_load2 -= readings_load2[load_read_index];

  readings_load1[load_read_index] = load1_val;
  readings_load2[load_read_index] = load2_val;

  total_load1 += readings_load1[load_read_index];
  total_load2 += readings_load2[load_read_index];

  load_read_index += 1;

  if (load_read_index >= numRead) load_read_index = 0;

  ave_load1 = total_load1 / float(numRead);
  ave_load2 = total_load2 / float(numRead);
  //Smoothing code ends
  
  load1_res2 = String(ave_load1);
  load2_res2 = String(ave_load2);
  
}


void add_data(String to_add){
  data += ",";
  data += to_add;
}

//void smoothing(double total, double readings[], double average){
//  total = total - readings[readIndex];
//}

void setup() {
  Serial.begin(9600);        //Enable serial at high speed 

     // Software Serial to FFT micro
  mySerial.begin(57600);

  //analogReference(EXTERNAL); //Important or we send 5V into the 3V3 pin!
  
  //Initialize SEN0093 pins (shaft temperature)
  pinMode(DataPin,INPUT);
  pinMode(ClockPin,INPUT);
  pinMode(TestPin,OUTPUT);
  digitalWrite(TestPin, HIGH);
  
  //Initialize the water level samples array
  for (int i = 0; i<water_level_sample_window; i++){
    water_level_samples[i]=0;
  }

  // Tachometer setup
  pinMode(solenoidValvePin,OUTPUT);\
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

    //Liquid Flow Sensor Setup
   pinMode(FLOWSENSORPIN, INPUT);
   digitalWrite(FLOWSENSORPIN, HIGH);
   lastflowpinstate = digitalRead(FLOWSENSORPIN);
//   useInterrupt(true);
//////////////////////////////////////////Initialise load film array
  for(int x = 0; x < numRead; x++){
    readings_load1[x] = 0.0;
    readings_load2[x] = 0.0;
  }
  //digitalWrite(solenoidValvePin,HIGH);

}

void loop() {
  position_sensors();
  fluid_level_sensor();
  solenoid_drain_valve();
  tachometer_sensor();
  flow_rate_sensor();
  film_sensor();


  if (millis() - last_print_time >= PRINT_DELAY){
    
    rpm_calculation();
    flow_rate();
    shaft_temp();
    one_wire_temps();

    data = "D";
    add_data(NODE_ID);
    add_data(shaftTemp);
    add_data(ambient_temperature);
    add_data(casing_temperature);
    add_data(gland_temperature);
    add_data(rpm);
    add_data(load1_res2);
    add_data(load2_res2);
    add_data(flow_rate_cc_per_sec);
    add_data(flowrate_str);
    add_data(position1_mm);
    add_data(position2_mm);
    
    receive_FFT();
    Serial.println(data);
    Serial.println("M" + data_mic);
    Serial.println("X" + data_x);
    Serial.println("Y" + data_y);
    Serial.println("Z" + data_z);  
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
