const byte LED = 13;
const byte SENSOR = 2;
volatile boolean drop_occured = false;
int drop_count = 0;

// Interrupt Service Routine (ISR)
void drop ()
{
  drop_occured = true;
}  

void setup ()
{
  Serial.begin(115200);
  pinMode (LED, OUTPUT);  // so we can update the LED
  digitalWrite (SENSOR, HIGH);  // internal pull-up resistor built in :-)
  attachInterrupt (0, drop, RISING);  // attach interrupt handler
}  // end of setup

void loop ()
{
  if (drop_occured) {
    drop_count += 1;
    drop_occured = false;
    Serial.println (drop_count);
  }    
} 
