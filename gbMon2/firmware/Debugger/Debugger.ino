//
//	Debugger.ino
//	gbMon2
//
//	Created by michael on 19.11.13.
//	Copyright (c) 2013 michael. All rights reserved.
//
//	Software serial multple serial test
//
//	Receives from the hardware serial, sends to software serial.
//	Receives from software serial, sends to hardware serial.
//
//	The circuit:
//	RX is digital pin 2 (connect to TX of other device)
//	TX is digital pin 3 (connect to RX of other device)
//

#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3
#define ledPin 13

// set up a new serial port
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);
byte pinState = 0;

// how much serial data we expect before a newline
const unsigned int MAX_INPUT = 50;

void setup ()
{
	// define pin modes for tx, rx, led pins:
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	//pinMode(ledPin, OUTPUT);
	
	// set the data rate for the SoftwareSerial port
	mySerial.begin(9600);
	Serial.begin(9600);
	Serial.println("SoftwareSerial Test stared ...");
} // end of setup

void loop()
{
	
	 
	 String content = "";
	 char character;
	 
	 while(mySerial.available()) {
	 character = mySerial.read();
	 content.concat(character);
	 }
	 
	 if (content != "") {
	 Serial.println(content);
	 }

	
}  // end of loop
