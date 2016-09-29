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
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

//Analog sensors
//+5V Position Sensor
//+5 Film sensor
//+3.3V Fluid Leve
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Analog smoothing
const int number_of_readings = 30; //Increases the size of global variables big time. Can be be managed in case we run into memory issues
int analog_index = 0;

float smoothing(const int* numb_readings, float* total_sum,
                float* array_item, int* analog_pin){

  *total_sum -= *array_item;
  *array_item = analogRead(*analog_pin);
  *total_sum += *array_item;
  float result = *total_sum / *numb_readings;
  return result;
}

float smoothing_Vin(const int* numb_readings, float* total_sum,
                float* array_item){

  *total_sum -= *array_item;
  *array_item = emon1.readVcc()*0.001;
  *total_sum += *array_item;
  float result = *total_sum / *numb_readings;
  return result;
}

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

int position1_pin = A0; //J9 Connector
int position2_pin= A1;  //J10 Connector
float position1_average = 0.0;
float position2_average = 0.0;
float position1_array[number_of_readings];
float position2_array[number_of_readings];
float position1_total = 0.0;
float position2_total = 0.0;
float position1_mm = 0.0;
float position2_mm = 0.0;


void position_sensors(){
  // ADD AUTO VCC function!!!!
  // Accodingly to datasheet function!!!
  // add circuitry??
  position1_average = smoothing(&number_of_readings, &position1_total,
                                &position1_array[analog_index], &position1_pin);

  position2_average = smoothing(&number_of_readings, &position2_total,
                                &position2_array[analog_index], &position2_pin);

  //////////////////// For debugging
  // Serial.println(analogRead(position1_pin));
  // Serial.println(analogRead(position2_pin));
  // Serial.println(position1_average);
  // Serial.println(position2_average);


  position1_mm = position1_average * 12.7 / 1023.0;
  position2_mm = position2_average * 12.7 / 1023.0;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            3. FLUID LEVEL SENSOR
int fluidLevel_pin = A3;  //J6 Connector


//Insert Calibration Data Here: Level = AX + B format (rounded to the nearest integer N)
//X is the water depth in cm
//A is the calibration slope in levels / cm
//B is the reference level
float A_water_rate = 4.5; // difference in bits per cm
float B_water_reference_level = 328.0;
float vesicle_diameter_cm = 11.43;//4.5 inch x 2.54 cm / inch
float vesicle_area_cm_2 = (3.14159) * (vesicle_diameter_cm / 2.0) * (vesicle_diameter_cm / 2.0);

//Initialize variables
boolean initialize = true;

float fluidLevel_average = 0;
float fluidLevel_array[number_of_readings];
float fluidLevel_total = 0.0;

float water_level_cm = 0.0;
float flow_calc_water_level_cm = 0.0;
unsigned long calc_time_ms = 0;

void fluid_level_sensor(){

  fluidLevel_average = smoothing(&number_of_readings, &fluidLevel_total,
                                  &fluidLevel_array[analog_index], &fluidLevel_pin);

  water_level_cm = (fluidLevel_average - B_water_reference_level) / A_water_rate;

  //////////////////// For debugging
  // Serial.println(analogRead(fluidLevel_pin));
  // Serial.println(fluidLevel_average);
  // Serial.println(water_level_cm);

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
float flow_rate_cc_per_sec = 0.0;
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
     float water_level_delta = water_level_cm - flow_calc_water_level_cm;
     float time_delta = millis() - calc_time_ms;
     //Serial.println(water_level_delta);
    // Serial.println(time_delta/1000);
     flow_rate_cc_per_sec = ( water_level_delta ) / (time_delta/1000.0);
  
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
float duration = 1;
volatile boolean revolution_occured = false;   
float revolution_count = 0.0;
unsigned int current_time = 0;
unsigned int previous_time = 0;
float rpm = 0.0;

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
  rpm = revolution_count / (duration/1000.0) * 60.0;
  revolution_count = 0.0;
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
float flowrate = 0.0;
// Interrupt is called once a millisecond, looks for any pulses from the sensor!
float liters = 0.0;
bool flowTrigger = false;



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
  flowrate = (pulses - last_pulses) / (duration/1000.0) ; //Hz
  flowrate = (flowrate + 3.0)/8.1; // L/min
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
        if ((millis() - time_check) >= 2000){//Haha, let's NOT get stuck if the thing is unplugged
          Serial.println("ir_cl_sync_error");
          break;
        }
        //Serial.println("hh");
        
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
          Serial.println("ir_cl_sync_error");
          break;
        }
      }while(PinState != 1);
    }
  *p++ = tempData;                         //pass byte to array pointer address 
  }
  digitalWrite(TestPin, HIGH);             //stop device transmission
}

void shaft_temp(){
    int i;
    unsigned long time_check=millis();

    int data_buf[5] = {0};
    int tempData = 0;
    do
    {
      data_read(data_buf);
      delay(1);
      if ((millis() - time_check) >= 2000){//Haha, let's NOT get stuck if the thing is unplugged
        Serial.println("checksum_error");
        break;
        }

    }while(((data_buf[0] + data_buf[1] + data_buf[2])& 0xff) != data_buf[3]);

    tempData = data_buf[1]*256 + data_buf[2];
    shaftTemp = (float)tempData/16-273.15;

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
float ambient_temperature = 0.0;
float casing_temperature = 0.0;
float gland_temperature = 0.0;
int  idle = 0;

void one_wire_temps(){
  // OneWire sensor setup
      // waited long enough??
    ambient_temperature = sensors.getTempCByIndex(0);
    casing_temperature = sensors.getTempCByIndex(1);
    gland_temperature = sensors.getTempCByIndex (2);
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

int load1_pin = A4;
int load2_pin = A5;
float load1_average = 0.0;
float load2_average = 0.0;
float load1_array[number_of_readings];
float load2_array[number_of_readings];
float load1_total = 0.0;
float load2_total = 0.0;

float v_in = 5.04;// Measure and correct the V in.
float v_in_total = 0.0;
float v_in_array[number_of_readings];

float load1_v_out = 0.0;
float load2_v_out = 0.0;
float res_1 = 50350.0; // Measure the value of R1 in voltage divider circuit.
float res_2 = 0.0;

float load_cap = 10000000.0;
float load1_res2 = 0.0;
float load2_res2 = 0.0;



void film_sensor(){

  load1_average = smoothing(&number_of_readings, &load1_total,
                            &load1_array[analog_index], &load1_pin);
  load2_average = smoothing(&number_of_readings, &load2_total,
                            &load2_array[analog_index], &load2_pin);
  v_in = smoothing_Vin(&number_of_readings, &v_in_total, &v_in_array[analog_index]);
  //////////////////// For debugging
  // Serial.println(analogRead(load1_pin));
  // Serial.println(analogRead(load2_pin));
  // Serial.println(load2_average);
  // Serial.println(load2_average);
  //Serial.println(v_in);

  //Convert analog signal to voltage
  load1_v_out = load1_average / 1023 * v_in;
  load2_v_out = load2_average / 1023 * v_in;

  //Calculate the resistance value of the film sensor
  load1_res2 = res_1 / (v_in / load1_v_out - 1.0);
  load2_res2 = res_1 / (v_in / load2_v_out - 1.0);

  //If value is higher than the cap, it sets to the cap value, because the value can reach INF
  if(load1_res2 > load_cap){
    load1_res2 = 10000000.0;
  }
  if(load2_res2 > load_cap){
    load2_res2 = 10000000.0;
  }

  
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                            11. Current sensor

float Irms = 0.0;

////////////////////////////////////////////END of Sensors

void add_data(String to_add){
  data += ",";
  data += to_add;
}

void add_data_floats(float* address_1){
    data += ",";
    data += *address_1;

}



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
  
  //Initialize the fuild level, load film & position samples arrays
  for (int i = 0; i<number_of_readings; i++){
    fluidLevel_array[i] = 0;
    position1_array[i] = 0;
    position2_array[i] = 0;
    load1_array[i] = 0;
    load2_array[i] = 0;
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

  emon1.current(A2, 30);             // Current: input pin, calibration.

}

float* sensors_list[] = {&shaftTemp, &ambient_temperature, &casing_temperature, &gland_temperature,
&rpm, &load1_res2, &load2_res2, &flow_rate_cc_per_sec, &flowrate, &position1_mm, &position2_mm, &Irms};


int i;
void loop() {
  /////////////////////////////////////////////////////////////////
  //                            Analog readings
  position_sensors();
  fluid_level_sensor();
  film_sensor();

  analog_index++;
  if (analog_index == number_of_readings) analog_index = 0;
  ////////////////////////////////////////////////////////////////

  solenoid_drain_valve();
  tachometer_sensor();
  flow_rate_sensor();

  //Serial.println(emon1.readVcc());
  
  if (millis() - last_print_time >= PRINT_DELAY){

    rpm_calculation();
    flow_rate();
    //shaft_temp();
    one_wire_temps();
    Irms = emon1.calcIrms(1480);  // Calculate Irms only


    data = "D";
    add_data(NODE_ID);
    for(i=0; i<12; i++){
        add_data_floats(sensors_list[i]);
    }
      /* 1. NodeID
   * 2. shaftTemp
   * 3. ambient_temperature
   * 4. casing_temperature
   * 5. gland_temperature
   * 6. rpm
   * 7. load1_res2
   * 8. load2_res2
   * 9. flow_rate_cc_per_sec
   * 10.flowrate
   * 11. posistion1_mm
   * 12. position2_mm
   * */
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
