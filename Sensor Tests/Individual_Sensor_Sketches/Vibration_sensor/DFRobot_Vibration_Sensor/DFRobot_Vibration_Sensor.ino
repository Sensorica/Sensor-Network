// Connect only the analogue pin and ground it.


void setup()
{
  Serial.begin(9600); //
}
void loop()
{
  	int val;
  	val=analogRead(0);//Connect the sensor to analog pin 0
  	Serial.println(val,DEC);//
  	delay(100);
}

