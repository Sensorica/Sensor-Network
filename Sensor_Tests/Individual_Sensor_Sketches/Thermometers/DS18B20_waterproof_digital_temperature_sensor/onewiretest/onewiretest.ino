#include <OneWire.h>
int DS18B20_Pin = 2;
OneWire ds(DS18B20_Pin);

class OneWireSensor
{
	// Class Member Variables
	// These are initialized at startup
	int sensorPin;      // the number of the LED pin
	long ResetDelay;     // milliseconds between OneWire Reset
	long ReadDelay;      // milliseconds between OneWire Read  
	// These maintain the current state
	int resetState;             		// current reset state
	unsigned long resetPreviousMillis;  	// will store last time reset was triggered
        unsigned long readPreviousMillis;  	// will store last time reset was triggered
  // Constructor - creates a OneWireSensor 
  // and initializes the member variables and state
  public:
  OneWireSensor(int pin, long resetDelay, long readDelay)
  {
	sensorPin = pin;
		  
	ResetDelay = resetDelay;
	ReadDelay = readDelay;
	
	resetState = LOW; 
	resetPreviousMillis = 0;
        
        readPreviousMillis = 0;
  }
 
  void Update()
  {
    // check to see if it's time to send the reset request
    unsigned long resetCurrentMillis = millis();
    unsigned long readCurrentMillis = millis();
     byte data[12];
  byte addr[8];
     
    if((resetState == HIGH) && (resetCurrentMillis - resetPreviousMillis >= ResetDelay))
    {
    	resetState = LOW;  // change state to wait
      resetPreviousMillis = resetCurrentMillis;  // Remember the time
        }
    else if ((resetState == LOW) && (resetCurrentMillis - resetPreviousMillis >= ResetDelay))
    {
      resetState = HIGH;  // change state to read
             
   
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      Serial.println("no more sensors on chain, reset search!");
      ds.reset_search();
     
  }
   
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      
  }
   
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
     
  }
   
  ds.reset();
  ds.select(addr);
  ds.write(0x44,0); // start conversion, with parasite power on at the end*/
      resetPreviousMillis = resetCurrentMillis;   // Remember the time
       
    }
    
    if (readCurrentMillis - readPreviousMillis >= ReadDelay)
    {
    	byte present = ds.reset();
        ds.select(addr);    
        ds.write(0xBE); // Read Scratchpad
   
     
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
    ds.reset_search();
     
  byte MSB = data[1];
  byte LSB = data[0];
   
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  Serial.print("temp");
  
      readPreviousMillis = readCurrentMillis;  // Remember the time
  }
     
  
    }
    else
    {
    	readPreviousMillis = readCurrentMillis;  // Remember the time
    }
  }
};


   
OneWireSensor temp(2,750,500);
   
void setup() {
  Serial.begin(9600);

}

void loop() {

temp.Update();
}
