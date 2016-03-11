int loadWasher;


void setup() {
 
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void loop() {
 
    loadWasher = analogRead(A0);  
 
  
  Serial.println(loadWasher);
 
  delay(500);

}
