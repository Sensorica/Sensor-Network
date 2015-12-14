/*
This program tests the rotation sensor using a QRD 114 IR (optical) proximity sensor. 
The QRD 114 IR (optical) proximity sensor. is made of an LED and a light detector. The LED sends an IR signal, which reflects on on object in front of it and comes bacl to the sensor. The light detector measures the intensity of the reflected light. The further the object the lower the intensity. 
An absorbant material is places on a rotating shaft, or any object, on one half of its circumference.
The proximity sensor is placed in proximity with the rotating shaft, facing it. 
Whenever the absorbant material is in front of the proximity sensor the reflected signal is low. 
Whenever the bare object is in front of the proximity sensor the reflected signal is high. 
As the shaft rotates, the intensity measured by the sensor is half of the time low and half of the time high. 
The duration between two consecutive transitions from low to high is a periode of rotation, which translates into rotations per minut, or RPM.  

NOTE: the RPM is an average of 3 measures over revolutions of the shaft. Therefore, the speed is a moving average over 3 consecutive revolutions. 

Program wrote by T!b! and Jim, SENSORICA affiliates - www.sensorica.co
Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0) 
*/
int LED = 13; // Define vatiable LED as pin 13, to drive the LED of the optical sensor. 
int sensorPin = 0; // use analog in pin A0 pin 0 to read the PD of the optical sensor
int previousValue; // sensorData used to store the sensor data
int currentValue; // sensorData used to store the sensor data
unsigned long previousTime;  // will store last time sensorData was updated
unsigned long currentTime;  // will store current time sensorData was updated
unsigned long periods[3]; // array of 3 elements, containing the period of time between different events, i.e. change in state from low to high of the sensor data
double RPM = 0; // rotation speed
unsigned long timeToprint = 0;

/* 
NOTE, we can parametrize the periodes array size, by creating a valueble "size" and passing it to the array at initialization. 
*/

void setup() {
  // put your setup code here, to run once:
  
Serial.begin(9600);  //se serial communication
pinMode(LED, OUTPUT); // define the digital pin out 13 as Output to drive the LED
digitalWrite(LED, HIGH); // put the LED ON
}

void loop() {
  // put your main code here, to run repeatedly:
currentValue = analogRead(sensorPin);    // read the input pin

/*
 Serial.print("currentValue=");
 Serial.println(currentValue);
 Serial.print("previousValue=");
 Serial.println(previousValue);
*/

if ((previousValue < 500) && (currentValue > 600))
   { 
   currentTime = millis(); // get current time when a transition event occurs
   periods[0] = periods[1]; //shift elements in array
   periods[1] = periods[2]; //shift elements in array
   periods[2] = currentTime - previousTime; //put new delay time into the array
   RPM = 60000 / ((periods[0] + periods[1] + periods[2])/3); //calculate RPM
   previousTime = currentTime; // set previous Time
   }
   
   previousValue = currentValue; // set previous value                
 
   delay (1);
   
   if ( currentTime - timeToprint > 2000 )
  
  {
    Serial.print("RPM = ");
    Serial.println(RPM);
  
   // Serial.print("timeToprint = ");
   // Serial.println(timeToprint);
   // Serial.print("currentTime = ");
   // Serial.println(currentTime);
    
    timeToprint = currentTime;
  }
   
/*   
   delay (1000);
   Serial.print("Periods =");
   Serial.print(periods[0]);
   Serial.print(" ;");
   Serial.print(periods[1]);
   Serial.print(" ;");
   Serial.println(periods[2]);
*/
   
   }

