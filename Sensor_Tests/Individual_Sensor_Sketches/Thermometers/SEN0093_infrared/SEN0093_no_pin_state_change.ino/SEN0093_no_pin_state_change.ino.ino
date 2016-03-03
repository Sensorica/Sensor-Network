
 
////////////////Arduino例子程序///////////////////////////////////////

#define  ClockPin  6  // clock in
#define  DataPin   7  //data in  
#define  TestPin   8 //driver pin  set pin low to start deliver data
 
void setup() {
  int i = 0;
  int j = 0;
  Serial.begin(9600);   
  pinMode(DataPin,INPUT);
  pinMode(ClockPin,INPUT);
  pinMode(TestPin,OUTPUT);
  digitalWrite(TestPin, LOW);
  delay(100);
}
 
void data_read(int *p)
{
  int i,j,PinState,tempData;
   
 
  for(i=0;i<5;i++)
  {
	for(j=0;j<8;j++)
	{
  	//do{
    	//PinState = digitalRead(ClockPin);
  	//}while(PinState);
  	
  	PinState = digitalRead(DataPin);
  	if(1 == PinState)
     	tempData = (tempData<<1 & 0xfe)+1;
  	else
     	tempData = (tempData<<1 & 0xfe);
   	//do{
    	//PinState = digitalRead(ClockPin);
  	//}while(PinState != 1);
	}
	*p++ = tempData;
  }
  
}
 
void loop()
{
  int data_buf[5] = {0};
 int tempData = 0;
unsigned int long previousTime = 0;
unsigned int long elapsedTime = 0; 
  
  
  while(1)
  {
  previousTime = micros(); 
  data_read(data_buf);
  //elapsedTime = millis() - previousTime;
  tempData = data_buf[1]*256 + data_buf[2];
  float realTemp = (float)tempData/16-273.15;
  elapsedTime = micros() - previousTime;
  
  
  Serial.println(elapsedTime);
  Serial.print("Temp[C]=");
  Serial.println(realTemp);
  delay(1000);  
  }
}	


