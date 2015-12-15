/*************************************************** 
 
 The MLX90614 Digital IR thermometer uses the SMBus™ Communication Protocol. This is similar enough to the I2C™ bus to enable the device to work with the Arduino Wire library with few functional exceptions. This sensor can coexist on the I2C™ bus with many other sensors.
Defining a class enables multiple instances of the device to be created in software dramatically simplifying the programming task compared to older non object oriented approaches.
Initial release February 2015: A repository for the most recent version was created at GitHub which can be found here: MLX90614 Library https://github.com/jfitter/MLX90614
To install the library download the RAR Archive from GitHub and extract it to the libraries. Try also the Sparkfun GitHub library https://github.com/sparkfun/SparkFun_MLX90614_Arduino_Library 
see instructions https://learn.adafruit.com/using-melexis-mlx90614-non-contact-sensors/wiring-and-test 
or the Adafruit GitHub library https://github.com/adafruit/Adafruit-MLX90614-Library
see instructions https://www.sparkfun.com/products/9570

  This is a library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);

  Serial.println("Adafruit MLX90614 test");  

  mlx.begin();  
}

void loop() {
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  Serial.println();
  delay(500);
}


