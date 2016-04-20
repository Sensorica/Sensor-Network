int sensorZ_pin = A0;
int sensorY_pin = A1;
int sensorX_pin = A2;

int sensorZ = 0;
int sensorY = 0;
int sensorX = 0;
int digitalSelectPin = 2;
int digitalSleepPin = 3;

int fft_window = 1000;
int fft_samples[fft_window];
int fft_output[fft_window];


String data;


void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(sensorZ, INPUT);
  pinMode(sensorY, INPUT);
  pinMode(sensorX, INPUT);
  pinMode(digitalSelectPin, OUTPUT);
  digitalWrite(digitalSelectPin, HIGH); //Set to HIGH for 6G sensitivity, OR LOW for 1.5G sensitivity
  digitalWrite(digitalSleepPin, HIGH);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorZ = analogRead(sensorZ_pin);
  sensorY = analogRead(sensorY_pin);
  sensorX = analogRead(sensorX_pin);

// NOTES about the readings:
// Remember that 1G (gravity) is always in effect towards the earth
// The baseline output should be around 337 points

  data = "";
  data += sensorZ;
  data += ",";
  data += sensorY;
  data += ",";
  data += sensorX;

  Serial.println(data);

  delay(100);
}
