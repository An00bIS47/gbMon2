
/*
 *   Pin	 Button		 Function
 *	=====	========	==========
 *	  7			0		  Down
 *	  6			1		  Up
 *	  5			2		  Left
 *	  4			3		  Right
 *
 */
 
#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty
#define LEDPIN 13

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {7, 6, 5, 4}; // the analog 0-5 pins are also known as 14-19
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed'
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

long time[NUMBUTTONS];
long lastUpdate=0;

int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the analog resistor divider

bool longClicked = false;

void setup() {
	byte i;
	
	// set up serial port
	Serial.begin(57600);
	Serial.println("gbMon2 ioBridge");
	Serial.print("Initializing ");
	Serial.print(NUMBUTTONS, DEC);
	Serial.println(" Buttons");
	
	// pin13 LED
	pinMode(LEDPIN, OUTPUT);
	
	// Make input & enable pull-up resistors on switch pins
	for (i=0; i< NUMBUTTONS; i++) {
		pinMode(buttons[i], INPUT);
		digitalWrite(buttons[i], HIGH);
		time[i]=0;
	}
	
	// Run timer2 interrupt every 15 ms
	TCCR2A = 0;
	TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;
	
	//Timer2 Overflow Interrupt Enable
	TIMSK2 |= 1<<TOIE2;
	
}

SIGNAL(TIMER2_OVF_vect) {
	check_switches();
}

void check_switches(){
	static byte previousstate[NUMBUTTONS];
	static byte currentstate[NUMBUTTONS];
	static long lasttime;
	byte index;
	
	if (millis() < lasttime) {
		// we wrapped around, lets just try again
		lasttime = millis();
	}
	
	if ((lasttime + DEBOUNCE) > millis()) {
		// not enough time has passed to debounce
		return;
	}
	// ok we have waited DEBOUNCE milliseconds, lets reset the timer
	lasttime = millis();
	
	for (index = 0; index < NUMBUTTONS; index++) {
		
		currentstate[index] = digitalRead(buttons[index]);   // read the button
		
		/*
		 Serial.print(index, DEC);
		 Serial.print(": cstate=");
		 Serial.print(currentstate[index], DEC);
		 Serial.print(", pstate=");
		 Serial.print(previousstate[index], DEC);
		 Serial.print(", press=");
		 */
		
		if (currentstate[index] == previousstate[index]) {
			if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
				// just pressed
				justpressed[index] = 1;
			}
			else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
				// just released
				justreleased[index] = 1;
			}
			pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
		}
		//Serial.println(pressed[index], DEC);
		previousstate[index] = currentstate[index];   // keep a running tally of the buttons
	}
}

void shortClick(int Button){
	Serial.print(Button, DEC);
	Serial.println(" pressed");
}

void longClick(int Button){
	Serial.print(Button, DEC);
	Serial.println(" pressed LONG");
}


void loop() {
	
	
	photocellReading = analogRead(photocellPin);
	if (lastUpdate == 0 ){
		Serial.print("Analog reading = ");
		Serial.print(photocellReading);     // the raw analog reading
		
		// We'll have a few threshholds, qualitatively determined
		if (photocellReading < 30) {
			Serial.println(" - Dark");
		} else if (photocellReading < 150) {
			Serial.println(" - Dim");
		} else if (photocellReading < 350) {
			Serial.println(" - Light");
		} else if (photocellReading < 550) {
			Serial.println(" - Bright");
		} else {
			Serial.println(" - Very bright");
		}
		lastUpdate=millis();
	}
	
	if (lastUpdate + 1000 < millis()) {
		lastUpdate=0;
	}

	for (byte i = 0; i < NUMBUTTONS; i++) {
		if (justpressed[i]) {
			justpressed[i] = 0;
			//Serial.print(i, DEC);
			//Serial.println(" Just pressed");
			digitalWrite(LEDPIN, HIGH);
			time[i]=millis();
			// remember, check_switches() will CLEAR the 'just pressed' flag
		}
		if (justreleased[i]) {
			justreleased[i] = 0;
			//Serial.print(i, DEC);
			time[i]=0;
			
			//Serial.println(" Just released");
			if (longClicked==true){
				longClick(i);
				longClicked=false;
			} else {
				digitalWrite(LEDPIN, LOW);
				shortClick(i);
			}
			// remember, check_switches() will CLEAR the 'just pressed' flag
		}
		if (pressed[i]) {
			//Serial.print(i, DEC);
			//Serial.println(" pressed");
			if((time[i] + 1000) < millis()){
				
				longClicked=true;
				
				
			}
			// is the button pressed down at this moment
		}
	}
}