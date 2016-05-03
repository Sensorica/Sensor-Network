/*
Code developed by Sensorica
Modified from openmusiclabs.com 8.18.12 example sketch 

The code takes in 
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

int down_sampling_counter = 0;

// Define various ADC prescalers
//Initialization flags
//const unsigned char PS_2_i = 0xe1;
//const unsigned char PS_4_i = 0xe2;
//const unsigned char PS_8_i = 0xe3;
//const unsigned char PS_16_i = 0xe4;
const unsigned char PS_32_i = 0xe5; //Chosen Prescaler -> 32
//const unsigned char PS_64_i = 0xe6;
//const unsigned char PS_128_i = 0xe7;

//Code flags -> should use the same prescaler as the initialization
//const unsigned char PS_2_c = 0xf1;
//const unsigned char PS_4_c = 0xf2;
//const unsigned char PS_8_c = 0xf3;
//const unsigned char PS_16_c = 0xf4;
const unsigned char PS_32_c = 0xf5; // Chosen Prescaler -> 32
//const unsigned char PS_64_c = 0xf6;
//const unsigned char PS_128_c = 0xf7;

//peak finding variables
const int number_of_peaks = 5;
const int noise_floor = 50;
int biggest_peaks[number_of_peaks];
int dB3[FFT_N/2];

//State Data
boolean is_microphone = true;
int down_sampling_rate = 5;

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
  ADCSRA = PS_32_i; //Set Prescaler Value
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
      
      ADCSRA = PS_32_c; // restart adc prescaler value
      
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
    }
    
    // window data, then reorder, then run, then take output
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei(); // turn interrupts back on
    
    find_peaks();
    print_fft(); 
    print_peaks();
    print_max_peaks();
    
    //SEND DATA TO ARDUINO-ARDUINO INTERFACE
    //NEED INPUT FROM JIM HERE

      is_microphone = true; //set to microphone mode
      down_sampling_rate =20;// microphone mode 

//    //TIME DUPLEX BETWEEN MICROPHONE AND VIBRATION SENSOR
//    if (is_microphone == true){
//      is_microphone = false; //set to vibration mode
//      down_sampling_rate =20;//vibration mode uses downsampling factor 20
//    }else{
//      is_microphone = true; //set to microphone mode
//      down_sampling_rate =5;// microphone mode 
//    }



//delay(2000);

//When TIMSK0 is on: 
delay(120000);

//    delay(500000); 
  }
}

void print_fft(){
  
    Serial.println("Fourier Transform:");
    for(int i = 0 ; i < FFT_N/2; i++){
      Serial.print(fft_log_out[i]);
      Serial.print(",");
    }
    Serial.println("");
  
}

void print_peaks(){ 
    Serial.println("3dB Bandwidth:");
    for(int i = 0 ; i < FFT_N/2; i++){
      Serial.print(dB3[i]);
      Serial.print(",");
    }
    Serial.println("");
}

void print_max_peaks(){
 int scaling_factor = 1; //pos * BW_per_pos
  if(is_microphone==true){
    Serial.println("Microphone Peaks At:");
    scaling_factor = 247;
  }else{
    Serial.println("Vibration Sensor Peaks At:");
    scaling_factor = 7;
  }
 
    for(int i = 0 ; i < number_of_peaks; i++){
      Serial.print(biggest_peaks[i]*scaling_factor);
      Serial.print(",");
      Serial.print(dB3[biggest_peaks[i]]);
      Serial.print(",");
    }
    Serial.println("");
}

void find_peaks(){

  //Initialize peak arrays
    for (int i = 0; i<FFT_N/2;i++){
    dB3[i] = 0;
  }
  
  for (int i = 1; i < FFT_N/2-1; i++){
    //Check that it's above the noise floor
    //Check if it's a peak
    if (fft_log_out[i] > noise_floor &&
        fft_log_out[i] >= fft_log_out[i-1] &&
        fft_log_out[i] >= fft_log_out[i+1]){
          int j=i;          
          while(fft_log_out[j] > fft_log_out[i]/2){
            if(j>=FFT_N/2-1){
              break;
            }
            j++;
          }
//         dB3[i] = 2*(j-i-1)+1; //Algorithm to find the 3dB point
        dB3[i] = fft_log_out[i]*(2*(j-i-1)+1); //This actually find the power
    }
  }
  
//initialize biggest peaks
for (int j = 0; j<number_of_peaks;j++){
    biggest_peaks[j] = 0;
  }
  
  //extract the biggest peaks
  int smallest_max_peak = 0;
  for (int i = 0; i<FFT_N/2;i++){
    if (dB3[i] > smallest_max_peak){ 

      //Determine the index of the current smallest peak
      int smallest_peak_index=0;
      int smallest_peak=20000;
      for (int j = 0; j<number_of_peaks;j++){
           if(dB3[biggest_peaks[j]]<smallest_peak){
            smallest_peak = dB3[biggest_peaks[j]];
            smallest_peak_index = j;
           }
      }
      
      //Replace it with the newest peak index
      biggest_peaks[smallest_peak_index] = i;

      //Determine what is the newest smallest peak and update the smallest_max_peak variable
      smallest_peak=20000;
      smallest_peak_index=0;
      for (int j = 0; j<number_of_peaks;j++){
           if(dB3[biggest_peaks[j]]<smallest_peak){
            smallest_peak = dB3[biggest_peaks[j]];
            smallest_peak_index = j;
           }
      }
      smallest_max_peak = dB3[biggest_peaks[smallest_peak_index]];  
    }
  }
}




