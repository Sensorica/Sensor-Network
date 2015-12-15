int pin = 13;
volatile int state = LOW;

void setup() {
    Serial.begin(9600);          //  setup serial
    pinMode(pin, OUTPUT);
    attachInterrupt(digitalPinToInterrupt(2), blink, FALLING);
}

void loop() {
  
    digitalWrite(pin, state);
    delay(10);
}

void blink() {
    state = !state;
}
