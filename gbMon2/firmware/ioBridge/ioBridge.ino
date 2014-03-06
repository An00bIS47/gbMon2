//
//  ioBridge.ino
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
//
// Pin Layout:
//
// Arduino  ATmega	  Function
//	=====   =====	 ==========
//    0	      2		  --- (Reserved for Serial Communication)
//    1	      3		  --- (Reserved for Serial Communication)
//    2	      4		  CLK
//    3	      5		  DATA
//	  4		  6		  Button Up
//	  5		 11		  Button Down
//	  6		 12		  Button Back
//	  7		 13		  Button Ok
//	  8		 14		  Button Left
//	  9		 15		  Button Right
//	 10		 16		  --- (Reserved for PWM)
//	 11		 17		  --- (Reserved for PWM)
//   12		 18		  SoftwareSerial RX
//   13		 19		  SoftwareSerial TX
//  ----    ----     ---------
//   A0	     23		  LDR
//   A1      24		  EC
//   A2      25		  EC
//   A3      26		  EC
//   A4      27		  EC
//   A5		 28		  Raspberry Pi Interrupt
//
//
// The command for the Raspberry Pi:
// !! The command will be send reversed !!
//
// |-----------|-------------------------------------------------------|-----------|
// |   FRAME   |                            INFORMATION				   |   FRAME   |
// |-----------|-------------------------------------------------------|-----------|
// | 1110 0111 | 00000000 00000000 00000000 00000000 00000000 00000000 | 1110 0111 |
// |-----------|-------------------------------------------------------|-----------|
// |	       |  Buttons   LDR       EC0      EC1      EC2      EC3   |		   |
// |-----------|-------------------------------------------------------|-----------|
//
// Binary:	10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
// Hex:		800000000000000000
//
//
//
// |-------------------------------------------------------------------------|
// |							INFORMATION									 |
// |-------------------------------------------------------------------------|
// |																		 |
// |			 |					   Analog Values		     			 |
// |			 |															 |
// |   Buttons   |    LDR    |    EC0    |    EC1    |    EC2    |    EC3    |
// | 0000	0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 | 0000 0000 |
// | |||| ||||																 |
// | |||| ||||																 |
// | |||| |||Button 0														 |
// | |||| ||Button 1														 |
// | |||| |Button 2															 |
// | |||| Button 3															 |
// | |||Button 4															 |
// | ||Button 5																 |
// | |Short Click															 |
// | Long Click																 |
// |-------------------------------------------------------------------------|
//

//#include "DHT.h"
#include <SoftwareSerial.h>

#define rxPin 12
#define txPin 13

#define DEBOUNCE 10			// button debouncer, how many ms to debounce, 5+ ms is usually plenty
//#define LEDPIN 13
#define CLKPIN	3			// D2
#define DATAPIN	2			// D3
#define PIINTERRUPTPIN 5
#define NUMBERECSENSORS 4

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {9, 8, 7, 6, 5, 4}; // the analog 0-5 pins are also known as 14-19

// we will track if a button is just pressed, just released, or 'currently pressed'
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

//DHT dht(DHTPIN, DHTTYPE);

long time[NUMBUTTONS];
long lastUpdate=0;

int photocellPin = 0;						// the cell and 10K pulldown are connected to a0
byte photocellReading;						// the analog reading from the ldr

int ecPins[NUMBERECSENSORS] = {1,2,3,4};	// the ec level reading pins are connected to a1
int ecReading[NUMBERECSENSORS];				// the analog reading from the ec probes

int buttonValue	= 0;						// the button value
int frame = 0;								// the command for the raspberry pi

bool longClicked = false;


// set up a new serial port
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);
byte pinState = 0;

// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 100;


void setup() {
	byte i;
	
	// set up serial port
	Serial.begin(57600);
	Serial.println("gbMon2 ioBridge");
	
	
	Serial.print("Initialising Pin Modes ...");
	// define pin modes for tx, rx, led pins:
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	
	// pin13 LED
	//pinMode(LEDPIN, OUTPUT);
	
	// DATA AND CLK TO OUTPUT
	pinMode(CLKPIN, OUTPUT);
	pinMode(DATAPIN, OUTPUT);
	pinMode(PIINTERRUPTPIN, OUTPUT);
	Serial.println("OK");
	
	// set the data rate for the SoftwareSerial port
	Serial.print("Initialising SoftwareSerial ...");
	mySerial.begin(57600);
	Serial.println("OK");
	
	Serial.print("Initialising ");
	Serial.print(NUMBUTTONS, DEC);
	Serial.print(" Buttons ...");
	// Make input & enable pull-up resistors on switch pins
	for (i=0; i< NUMBUTTONS; i++) {
		pinMode(buttons[i], INPUT);
		digitalWrite(buttons[i], HIGH);
		time[i]=0;
	}
	Serial.println("OK");
	

	
	buildFrame();
	//dht.begin();
	
	// Run timer2 interrupt every 15 ms
	TCCR2A = 0;
	TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;
	
	//Timer2 Overflow Interrupt Enable
	TIMSK2 |= 1<<TOIE2;
	
}

SIGNAL(TIMER2_OVF_vect) {
	check_switches();
}


void printByte(int value){
	
	for (unsigned int mask = 0x80; mask; mask >>= 1) {
		if (mask & value) {
			Serial.print('1');
		}
		else {
			Serial.print('0');
		}
	}
	//Serial.println();
}

void printCommand(){
	
	// SEND TO INTERRUPT PIN
	digitalWrite(PIINTERRUPTPIN, HIGH);
	delay(3);
	digitalWrite(PIINTERRUPTPIN, LOW);
	
	Serial.println("|==========|=======================================================|==========| ");
    Serial.print("| ");
	printByte(frame);
	sendCommand(frame);
    Serial.print(" | ");
	
	printByte(buttonValue);
	sendCommand(buttonValue);
	Serial.print(" ");
	
	printByte(photocellReading);
	sendCommand(photocellReading);
	Serial.print(" ");
	
	for (int i=0; i<NUMBERECSENSORS; i++) {
		printByte(ecReading[i]);
		sendCommand(ecReading[i]);
		Serial.print(" ");
	}
    Serial.print("| ");
	printByte(frame);
	sendCommand(frame);
	Serial.print(" |");
	Serial.println();
	Serial.println("|==========|=======================================================|==========| ");
}


void sendCommand(unsigned int data){
	/*
	Serial.print("Data: ");
	Serial.print(data, DEC);
	Serial.print(" : ");
	printByte(data);
	Serial.print(" - ");
	*/
	for (unsigned int mask = 0x80; mask; mask >>= 1) {
		digitalWrite(CLKPIN, HIGH);
		if (mask & data) {
			//Serial.print('1');
			digitalWrite(DATAPIN, HIGH);
		}
		else {
			//Serial.print('0');
			digitalWrite(DATAPIN, LOW);
		}
		digitalWrite(CLKPIN, HIGH);
		delay(3);
	}
	
	//Serial.println();
}

void buildFrame(){
	// Frame:	11100111
	// Binary:	10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
	// Decimal: 604462909807314587353088
	// Binary:	11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111
	// Decimal: 1208925819614629174706175
	// Binary:	11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111
	//			63    56 55	   48 47	40 39	 32 31	  24 23    16 15	 8 7	  0
	// Hex:		800000000000000000

	frame |= (1 << 7);
	frame |= (1 << 6);
	frame |= (1 << 5);
	frame |= (1 << 2);
	frame |= (1 << 1);
	frame |= (1 << 0);
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
	
	buttonValue = 0;
	buttonValue |= (1 << Button);
	buttonValue |= (1 << 6);	// for short click
	Serial.print(Button, DEC);
	Serial.print(" pressed - Value: ");
	
	
	// 0x8000 = 10000000 0000000
	// 0x80   = 10000000
	for (unsigned int mask = 0x80; mask; mask >>= 1) {
		if (mask & buttonValue) {
			Serial.print('1');
		}
		else {
			Serial.print('0');
		}
	}
	Serial.println();
	
	
	// Print the command
	printCommand();
	
	buttonValue = 0;
}

void longClick(int Button){
	buttonValue = 0;
	buttonValue |= (1 << Button);
	buttonValue |= (1 << 7);	// for long click
	Serial.print(Button, DEC);
	Serial.print(" pressed LONG - Value: ");
	
	
	// 0x8000 = 10000000 0000000
	// 0x80   = 10000000
	for (unsigned int mask = 0x80; mask; mask >>= 1) {
		if (mask & buttonValue) {
			Serial.print('1');
		}
		else {
			Serial.print('0');
		}
	}
	Serial.println();
	
	// Print the command
	printCommand();
	
	buttonValue = 0;
}


void loop() {
	
	
	photocellReading = analogRead(photocellPin);

	for (int j=0; j < NUMBERECSENSORS; j++) {
		ecReading[j] = analogRead(ecPins[j]);
	}
	
	
	if (lastUpdate == 0 ){
		
		/*
		for (int j=0; j < NUMBERECSENSORS; j++) {
			
			
			Serial.print("ecLevel ");
			Serial.print(j);
			Serial.print(" reading = ");
			Serial.print(ecReading[j]);
			Serial.print(" : ");
			
			// 0x8000 = 10000000 0000000
			// 0x80   = 10000000
			for (unsigned int mask = 0x80; mask; mask >>= 1) {
				if (mask & ecReading[j]) {
					Serial.print('1');
				}
				else {
					Serial.print('0');
				}
			}
			Serial.println();
			 
		}
		*/
		/*
		Serial.print("LDR reading = ");
		Serial.print(photocellReading);

		
		
		// We'll have a few threshholds, qualitatively determined
		if (photocellReading < 30) {
			Serial.print(" - Dark");
		} else if (photocellReading < 150) {
			Serial.print(" - Dim");
		} else if (photocellReading < 350) {
			Serial.print(" - Light");
		} else if (photocellReading < 550) {
			Serial.print(" - Bright");
		} else {
			Serial.print(" - Very bright");
		}
		
		Serial.print(" : ");
		
		// 0x8000 = 10000000 0000000
		// 0x80   = 10000000
		for (unsigned int mask = 0x80; mask; mask >>= 1) {
			if (mask & photocellReading) {
				Serial.print('1');
			}
			else {
				Serial.print('0');
			}
		}
		Serial.println();
		*/
		
		/*
		 * Print AND Send the command
		 */
		printCommand();
		
		
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
			//digitalWrite(LEDPIN, HIGH);
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
				//digitalWrite(LEDPIN, LOW);
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
	
	
	String content = "";
	char character;
	
	while(mySerial.available()) {
		character = mySerial.read();
		content.concat(character);
	}
	
	if (content != "") {
		Serial.print("SOFTSERIAL: ");
		Serial.print(content);
		Serial.println();
	}
}