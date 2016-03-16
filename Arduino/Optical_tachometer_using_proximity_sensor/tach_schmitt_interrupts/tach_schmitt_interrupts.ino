const byte TACH = 2;
const byte DURATION = 1;
volatile boolean revolution_occured = false;
unsigned int revolution_count = 0;
unsigned int current_time = 0;
unsigned int previous_time = 0;
unsigned int rpm = 0;
//unsigned int duration = 1; // in seconds

// Interrupt Service Routine (ISR)
void revolution ()
{
  revolution_occured = true;
}  

void setup ()
{
  Serial.begin(115200);
  
  digitalWrite (TACH, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (digitalPinToInterrupt(2), revolution, RISING);  // attach interrupt handler
}  // end of setup

void loop ()
{
  if (revolution_occured) {
    revolution_count ++;
    revolution_occured = !revolution_occured;
  }   
  
  current_time = millis();
  if (current_time - previous_time >= 1000) {
    rpm = (revolution_count / DURATION * 60);
    Serial.print ("rpm = ");
    Serial.println (rpm);
    Serial.print ("counts = ");
    Serial.println (revolution_count); 
    previous_time = current_time;
    revolution_count = 0;
  }   
}