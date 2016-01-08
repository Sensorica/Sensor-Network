const byte TACH = 2;
volatile boolean revolution_occured = false;
int revolution_count = 0;
int current_time = 0
int previous_time = 0;
int rpm = 0;
int duration = 1; // in seconds

// Interrupt Service Routine (ISR)
void revolution ()
{
  revolution_occured = true;
}  

void setup ()
{
  Serial.begin(115200);
  pinMode (LED, OUTPUT);  // so we can update the LED
  digitalWrite (TACH, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (0, revolution, RISING);  // attach interrupt handler
}  // end of setup

void loop ()
{
  if (revolution) {
    revolution_count += 1;
    revolution_occured = false;
  }   
  
  current_time = millis();
  if (current_time - previous_time >= 1000) {
    rpm = (revolution_count / duration * 60);
	Serial.println (rpm);
	revolution_count = 0;
	previous_time = current_time;
  }  
}
 
