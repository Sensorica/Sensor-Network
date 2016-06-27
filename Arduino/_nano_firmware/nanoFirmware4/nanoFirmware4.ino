
/*
Code developed by Sensorica
Modified from openmusiclabs.com 8.18.12 example sketch 


We sample microphone and vibration sensor data and alternates between the two
We pass the data through an FFT
We searche for peaks in the FFT
We calculate the 3dB peak-power at the peaks
We output the data to the serial port
*/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

// do #defines BEFORE #includes
#define LOG_OUT 1 // use the log output function
#define FFT_N 128 // set to 128 point fft --> TODO: If you remove peak_power[] you will have enough memory to use a 256 point
#define WINDOW 1 // Enable windowing

#include <FFT.h> // include the library
//#include <prescaler.h>

int digitalSelectPin = 2;
int digitalSleepPin = 3;


// Define various ADC prescaler variables
const unsigned char PS_32_i = 0xe5; //Chosen Prescaler -> 32
const unsigned char PS_32_c = 0xf5; // Chosen Prescaler -> 32

//peak finding variables
const int number_of_peaks = 5;
int noise_floor = 100;
int biggest_peaks[number_of_peaks];
int peak_power[FFT_N/2];

//State Data
int mic_or_zxy_state = 0; //There are 4 possible states from 0-3: Microphone = 0; Vibration X = 1; Vibration Y = 2; Vibration Z = 3;
int down_sampling_counter = 0;
int down_sampling_rate = 1;

//Print Variables
String data_string = "";
String data_string_mic = "";
String data_string_x = "";
String data_string_y = "";
String data_string_z = "";
String data_request = "";

///////////////////////////////////////////////////SETUP////////////////////////////////////////////////////////
void setup() {
  mySerial.begin(4800);
  pinMode(digitalSelectPin, OUTPUT);
  digitalWrite(digitalSelectPin, LOW); //Set to HIGH for 6G sensitivity, OR LOW for 1.5G sensitivity
  digitalWrite(digitalSleepPin, HIGH);
  Serial.begin(9600); // use the serial port
  TIMSK0 = 0; //Reduces jitter
  ADCSRA = PS_32_i; //Set Prescaler Value
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

//////////////////////////////////////////////////LOOP//////////////////////////////////////////////////////////

void loop() {

// TODO:
// 2. Then run the mic and vibration again, store the data, and wait for the signal
// 3. Refresh if I haven't seen anything in 10 second
  
  while(1) { // reduces jitter

  //DEBUG
//  if(mic_or_zxy_state==0){
////    Serial.print("Microphone running");
//  }else if(mic_or_zxy_state==1) {
////    Serial.print("VibrationX running");
//  }else if(mic_or_zxy_state==2){
////    Serial.print("VibrationY running");
//  }else{
////    Serial.print("VibrationZ running");
//  }
  
    
    cli();  // UDRE interrupt slows this way down on arduino1.0

    //Collect FFT_N samples
    for (int i = 0 ; i < 2 * FFT_N; i += 2) {
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
    //print_fft(); 
    //print_peaks();
    //print_max_peaks();
    soft_serial_max_peaks();

    //ALTERNATE BETWEEN MICROPHONE AND VIBRATION SENSOR
    if (mic_or_zxy_state == 0){
      //SWITCH BACK TO VIBRATION X SENSOR
      mic_or_zxy_state = 1; //set to vibration mode
      down_sampling_rate =20;//vibration mode uses downsampling factor 20
      ADMUX = 0x41; // change it to adc1
      noise_floor = 50;
    }else if (mic_or_zxy_state == 1){
      //SWITCH BACK TO VIBRATION X SENSOR
      mic_or_zxy_state = 2; //set to vibration mode
      down_sampling_rate =20;//vibration mode uses downsampling factor 20
      ADMUX = 0x42; // change it to adc1
      noise_floor = 50;

    }else if (mic_or_zxy_state == 2){
      //SWITCH BACK TO VIBRATION X SENSOR
      mic_or_zxy_state = 3; //set to vibration mode
      down_sampling_rate =20;//vibration mode uses downsampling factor 20
      ADMUX = 0x43; // change it to adc1
      noise_floor = 50;
      
    }else if (mic_or_zxy_state == 3){
      //SWITCH BACK TO MICROPHONE
      mic_or_zxy_state = 0; //set to microphone mode
      down_sampling_rate =1;// microphone mode 
      ADMUX = 0x40; // use adc0
      noise_floor = 100;
    }
  }
}


//////////////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////
//void print_fft(){
//    Serial.println("Fourier Transform:");
//    for(int i = 0 ; i < FFT_N/2; i++){
//      Serial.print(fft_log_out[i]);
//      Serial.print(",");
//    }
//    Serial.println("");
//}
//
//void print_peaks(){ 
//    Serial.println("3dB Bandwidth:");
//    for(int i = 0 ; i < FFT_N/2; i++){
//      Serial.print(peak_power[i]);
//      Serial.print(",");
//    }
//    Serial.println("");
//}
//
//void print_max_peaks(){
// int scaling_factor = 1; //pos * BW_per_pos
//  if(mic_or_zxy_state==0){
//    Serial.println("Microphone Peaks At:");
//    scaling_factor = 133;
//  }else if(mic_or_zxy_state==1){
//    Serial.println("VibrationX Sensor Peaks At:");
//    scaling_factor = 7;
//  }else if(mic_or_zxy_state==2){
//    Serial.println("VibrationY Sensor Peaks At:");
//    scaling_factor = 7;
//  }else{
//    Serial.println("VibrationZ Sensor Peaks At:");
//    scaling_factor = 7;
//  }
// 
//    for(int i = 0 ; i < number_of_peaks; i++){
//      if(biggest_peaks[i]!=0){
//      Serial.print((biggest_peaks[i]+1)*scaling_factor); //Print out the frequency
//      }else{
//          Serial.print(0);
//      }
//      Serial.print(",");
//      Serial.print(peak_power[biggest_peaks[i]]); //Print out the power at that peak
//      Serial.print(",");
//    }
//    Serial.println("");
//}

void find_peaks(){
  //Initialize peak arrays
    for (int i = 0; i<FFT_N/2;i++){
    peak_power[i] = 0;
  }
  
  for (int i = 1; i < FFT_N/2-1; i++){
    //Check that it's above the noise floor
    //Check if it's a peak
    if (fft_log_out[i] > noise_floor &&
        fft_log_out[i] >= fft_log_out[i-1] &&
        fft_log_out[i] >= fft_log_out[i+1]){
          int j=i;

          //Determine the BW of the peak
          while(fft_log_out[j] > fft_log_out[i]/2){
            if(j>=FFT_N/2-1){
              break;
            }
            j++;
          }

//         dB3[i] = 2*(j-i-1)+1; //Algorithm to find the right hand 3dB point
        peak_power[i] = fft_log_out[i]*(2*(j-i-1)+1); //Calculate the power of this peak
    }
  }
  
//initialize biggest peaks
for (int j = 0; j<number_of_peaks;j++){
    biggest_peaks[j] = 0;
  }
  
  //extract the biggest peaks
  int smallest_max_peak = 0;
  for (int i = 0; i<FFT_N/2;i++){
    if (peak_power[i] > smallest_max_peak){ 

      //Determine the index of the current smallest peak
      int smallest_peak_index=0;
      int smallest_peak=20000;
      for (int j = 0; j<number_of_peaks;j++){
           if(peak_power[biggest_peaks[j]]<smallest_peak){
            smallest_peak = peak_power[biggest_peaks[j]];
            smallest_peak_index = j;
           }
      }
      
      //Replace it with the newest peak index
      biggest_peaks[smallest_peak_index] = i;

      //Determine what is the newest smallest peak and update the smallest_max_peak variable
      smallest_peak=20000;
      smallest_peak_index=0;
      for (int j = 0; j<number_of_peaks;j++){
           if(peak_power[biggest_peaks[j]]<smallest_peak){
            smallest_peak = peak_power[biggest_peaks[j]];
            smallest_peak_index = j;
           }
      }
      smallest_max_peak = peak_power[biggest_peaks[smallest_peak_index]];  
    }
  }
}


void soft_serial_max_peaks(){
 //Set the scaling factor based on the state
 int scaling_factor = 1; //pos * BW_per_pos
  if(mic_or_zxy_state==0){
    
    scaling_factor = 133;
      //Build the printout string
   for(int i = 0; i < number_of_peaks; i++){

    //What we want:
    // 1. Print out the peaks,
    // 2. Print out the power levels
      
      //Print out the frequency
      if(biggest_peaks[i]!=0){ //In the case of zero don't print the scaling factor
        data_string_mic += ","; 
        data_string_mic += ((biggest_peaks[i]+1)*scaling_factor);
      }else{ 
        //In the case of zero don't print the scaling factor
         data_string_mic += ",";
         data_string_mic += "0";
      }
      
      //Print the power at that peak  
      data_string_mic += ",";
      data_string_mic += (peak_power[biggest_peaks[i]]);
    }
    
//    Serial.println(" Microphone Software Serial Test:");

  }else if (mic_or_zxy_state==1){
    scaling_factor = 7;
      //Build the printout string
   for(int i = 0; i < number_of_peaks; i++){

    //What we want:
    // 1. Print out the peaks,
    // 2. Print out the power levels
      
      //Print out the frequency
      if(biggest_peaks[i]!=0){ //In the case of zero don't print the scaling factor
        data_string_x += ","; 
        data_string_x += ((biggest_peaks[i]+1)*scaling_factor);
      }else{ 
        //In the case of zero don't print the scaling factor
         data_string_x += ",";
         data_string_x += "0";
      }
      
      //Print the power at that peak  
      data_string_x += ",";
      data_string_x += (peak_power[biggest_peaks[i]]);
    }
    
//    Serial.println(" Vibration SoftwareX Serial Test:");

  }else if(mic_or_zxy_state==2){
   
   scaling_factor = 7;
      //Build the printout string
   for(int i = 0; i < number_of_peaks; i++){

    //What we want:
    // 1. Print out the peaks,
    // 2. Print out the power levels
      
      //Print out the frequency
      if(biggest_peaks[i]!=0){ //In the case of zero don't print the scaling factor
        data_string_y += ","; 
        data_string_y += ((biggest_peaks[i]+1)*scaling_factor);
      }else{ 
        //In the case of zero don't print the scaling factor
         data_string_y += ",";
         data_string_y += "0";
      }
      
      //Print the power at that peak  
      data_string_y += ",";
      data_string_y += (peak_power[biggest_peaks[i]]);
    }
    
//    Serial.println(" Vibration SoftwareY Serial Test:");

  }else{
//    Serial.println(" Vibration SoftwareZ Serial Test:");
    scaling_factor = 7;

      //Build the printout string
   for(int i = 0; i < number_of_peaks; i++){

    //What we want:
    // 1. Print out the peaks,
    // 2. Print out the power levels
      
      //Print out the frequency
      if(biggest_peaks[i]!=0){ //In the case of zero don't print the scaling factor
        data_string_z += ","; 
        data_string_z += ((biggest_peaks[i]+1)*scaling_factor);
      }else{ 
        //In the case of zero don't print the scaling factor
         data_string_z += ",";
         data_string_z += "0";
      }
      
      //Print the power at that peak  
      data_string_z += ",";
      data_string_z += (peak_power[biggest_peaks[i]]);
    }
    
  }


//    Serial.println(data_string);

    //Print out to the node after the fourth iteration
    if(mic_or_zxy_state==3){

  int loool = 0;
    
    while(true){

      while(!mySerial.available()){
      }




    data_request = char(mySerial.read());

//    Serial.print(data_request);
//    Serial.print(" ");

      
      if(data_request == "M"){
        mySerial.print(data_string_mic);
        Serial.println("M is sent");
      }else if(data_request == "X"){
        mySerial.print(data_string_x);
        Serial.println("X is sent");
      }else if(data_request == "Y"){
        mySerial.print(data_string_y);
        Serial.println("Y is sent");
      }else if(data_request == "Z"){
        mySerial.print(data_string_z);
        Serial.println("Z is sent");
      }else if(data_request == "E"){
        Serial.println("E is received");
        break;
      }
      
    }

      data_string_mic = "";
      data_string_x = "";
      data_string_y = "";
      data_string_z = "";    



    
      //Here we have our data ready... wait for the signal to send it
//      while(!mySerial.available()){
//      }
//        
//      //Print out the ! to confirm that we received it
//      while(mySerial.available()){
//          Serial.write(mySerial.read());
//      }

    


      
 //     mySerial.print(data_string);
//      delay(100);
//      Serial.println (data_string); //If this doesn't print, then you may be having memory issues
//      Serial.println ("Data was sent!");
//      delay(100); //Give it some clearance to make sure everything printed (not sure if this is needed)
//      data_string = ""; //clear the data string once we've printed it

      
    }else{
//      Serial.println("Need to run another time");
    }
}




