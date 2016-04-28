/*
fft_adc.pde
guest openmusiclabs.com 8.18.12
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

// do #defines BEFORE #includes
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft
#define WINDOW 1 // Enable windowing

#include <FFT.h> // include the library
//#include <prescaler.h>

int  delayInMillis = 2000;
unsigned long lastPrintTime = 0;

//DEFINE PINS
int sensorZ_pin = A0;
int sensorY_pin = A1;
int sensorX_pin = A2;
int sample = 0;

int sensorZ = 0;
int sensorY = 0;
int sensorX = 0;
int digitalSelectPin = 2;
int digitalSleepPin = 3;
String data;

int sampling_freq_hz = 200;
int down_sampling_rate = 20;
int down_sampling_counter = 0;
int millisecond_delay = 16; //This should be the same as 1000 / 200;
int microsecond_delay = millisecond_delay * 1000;

// Define various ADC prescalers
//Initialization flags
const unsigned char PS_2_i = 0xe1;
const unsigned char PS_4_i = 0xe2;
const unsigned char PS_8_i = 0xe3;
const unsigned char PS_16_i = 0xe4;
const unsigned char PS_32_i = 0xe5; //Chosen Prescaler
const unsigned char PS_64_i = 0xe6;
const unsigned char PS_128_i = 0xe7;

//Code flags -> should use the same prescaler as the initialization
const unsigned char PS_2_c = 0xf1;
const unsigned char PS_4_c = 0xf2;
const unsigned char PS_8_c = 0xf3;
const unsigned char PS_16_c = 0xf4;
const unsigned char PS_32_c = 0xf5; // Chosen Prescaler
const unsigned char PS_64_c = 0xf6;
const unsigned char PS_128_c = 0xf7;

void setup() {
    // declare the ledPin as an OUTPUT:
  pinMode(sensorZ, INPUT);
  pinMode(sensorY, INPUT);
  pinMode(sensorX, INPUT);
  pinMode(digitalSelectPin, OUTPUT);
  digitalWrite(digitalSelectPin, HIGH); //Set to HIGH for 6G sensitivity, OR LOW for 1.5G sensitivity
  digitalWrite(digitalSleepPin, HIGH);
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; //Reduces jitter
  ADCSRA = PS_32_i; //HERE!!!!!!
  
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
  while(1) { // reduces jitter
      // read the value from the sensor:
//  sensorZ = analogRead(sensorZ_pin);
//  sensorY = analogRead(sensorY_pin);
//  sensorX = analogRead(sensorX_pin);
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 2 * FFT_N; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready -> Checking bit 4
      
      ADCSRA = PS_32_c; // restart adc // HERE!!!!!!
      
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int

      if(down_sampling_counter % down_sampling_rate == 0)
      {
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
      } else{
        i-=2;
      }
      down_sampling_counter++;
//      delay(1);
    }
    
    // window data, then reorder, then run, then take output
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei(); // turn interrupts back on
    
    Serial.println("Fourier Transform:");
    for(int i = 0 ; i < FFT_N/2; i++){
      Serial.print(fft_log_out[i]);
      Serial.print(",");
    }
    Serial.println("");

//delay(2000);

//When TIMSK0 is on: 
delay(120000);

//    delay(500000); 
  }
//  if (millis() - lastPrintTime >= delayInMillis) {
//  data = "";
//  data += sensorZ;
//  data += ",";
//  data += sensorY;
//  data += ",";
//  data += sensorX;
//
//  Serial.println(data);
//  lastPrintTime = millis();
//  }
}

