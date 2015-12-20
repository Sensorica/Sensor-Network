/***************************************************************
Instruction:
 Connection:
 1.Plugable Terminal Sensor Adapter & Waterproof DS18B20 Digital Temperature Sensor 
                 A   ---->     Blue(DATA SIGNAL)
                 B   ---->     RED   (VIN) 
                 C   ---->     Black (GND) 
   
 2.Waterproof DS18B20 Digital Temperature Sensor & Arduino board
              1(A)   ---->     Digital Pin2
              2(B)   ---->     5V/3.3V  
              3(C)   ---->     GND      
  
 Setting for the Pull-up Register/Pull-down Register Selection Jumpers
     When connect DS18B20 with the adapter,please choose to use the 
     Pull-up Register Jumper
 ***************************************************************/
#include <OneWire.h>
   
int DS18B20_Pin = 2; //DS18B20 Signal pin on digital 2
   
//Temperature chip i/o
OneWire ds(DS18B20_Pin);  // on digital pin 2
   
void setup(void) {
  Serial.begin(9600);
}
   
void loop(void) {
  float temperature = getTemp();
  Serial.println(temperature);
     
  delay(2000); //just here to slow down the output so it is easier to read
     
}
   
float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
   
  byte data[12];
  byte addr[8];
   
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      Serial.println("no more sensors on chain, reset search!");
      ds.reset_search();
      return -1000;
  }
   
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }
   
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }
   
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
   
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
   
     
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
     
  ds.reset_search();
     
  byte MSB = data[1];
  byte LSB = data[0];
   
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
     
  return TemperatureSum;
     
}

