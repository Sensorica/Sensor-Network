int sensor = 0; // use A0 pin 0 to read the PD of the optical sensor
int value = 0; // define variable "value" and set to 0
int LED = 13; // Define vatiable LED as pin 13, to drive the LED of the optical sensor. 

void setup() {
Serial.begin(9600);  //se serial communication
pinMode(LED, OUTPUT); // define the digital pin out 13 as Output to drive the LED
digitalWrite(LED, HIGH); // put the LED ON
}

void loop() {
value = analogRead(sensor);  //read the value of the PD 
Serial.println(value); //send this data through serial port in human readable ASCI
delay(500); //wait. 
}

