/******** start code ********/
/*
 *    created   	 2013-07-29
 *    by   	 lisper (leyapin@gmail.com)
 *    function    test GP2Y0A41SK0F
 *
 */

//connect gp2d120x to A1
#define pin A1

void setup () {
    	Serial.begin (9600);
    	pinMode(pin, INPUT);
}

void loop () {
    	uint16_t value = analogRead (pin);
    	double distance = get_IR (value); //Convert the analog voltage to the distance
    	Serial.println (value);             	//Print the data to the arduino serial monitor
    	Serial.print (distance);
    	Serial.println (" cm");
    	Serial.println ();
    	delay (500);                        	//Delay 0.5s
}

//return distance (cm)
double get_IR (uint16_t value) {
    	if (value < 16)  value = 16;
    	return 2076.0 / (value - 11.0);
}

/******** end code ********/

