//
//  lcdBridge.ino
//  gbMon2
//
//  Created by michael on 25.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <glcd.h>

#include "fonts/allFonts.h"				// system and arial14 fonts are used
#include "bitmaps/allBitmaps.h"			// all images in the bitmap dir

#define BUFFERSIZE 1024

int pbIn = 0;							// Interrupt 0 is on DIGITAL PIN 2!
int PINDATA = 3;						// DATA Pin - DIGITAL PIN 3
int counter;

volatile int state = LOW;				// The input state toggle
uint8_t ringbuffer[BUFFERSIZE];

char bits[8];
uint8_t bitsCounter;

uint8_t sum=0;

uint8_t curColumn;
uint8_t curRow;
uint8_t curPos;


volatile uint8_t recv = 0;
//volatile uint8_t bit = 0;
volatile uint8_t read = 0;
volatile uint32_t bytesReceived;

/*
 * Declare Icons
 */
Image_t bootIcon;





void countdown(int count){
    while(count--){  // do countdown
        delay(500);
    }
}



void addToBuffer(uint8_t bit){
	
	//ringbuffer[recv]=bit;
	bits[bitsCounter]=(char)(((int)'0')+bit);
	
	//Serial.println("Add to Buffer: bit: %d - recv: %d\n", bit, recv);
	recv++;
	bitsCounter++;
	
	if (bitsCounter==8) {
		//Serial.println(fromBinary(bits), DEC);
		ringbuffer[bytesReceived]=fromBinary(bits);
		bytesReceived++;
		bitsCounter=0;
	}
	
	if (recv==BUFFERSIZE) {
		recv=0;
	}
	
	//ringbuffer[bytesReceived]=bits;
}



void introScreen(){
    int i;
    
    if(GLCD.Height >= 64)
        bootIcon = raspberry64x64;  // the 64 pixel high icon
    GLCD.DrawBitmap(bootIcon, 32,0); //draw the bitmap at the given x,y position
    countdown(3);
    
    GLCD.ClearScreen();
    GLCD.SelectFont(Arial_14); // you can also make your own fonts, see playground for details
    GLCD.CursorToXY(GLCD.Width/2 - 44, 3);
    GLCD.print("GLCD version ");
    GLCD.print(GLCD_VERSION, DEC);
    
    // Booting Screen
    for (i=0; i<3; i++) {
        GLCD.BarGraph(10, 40, 128, 8, 2, i);
        countdown(1);
    }

}

int fromBinary(char *s) {
	return (int) strtol(s, NULL, 2);
}

void setup() {
    Serial.begin(57600);
    Serial.print ("Starting gbMon2 lcdBridge...\n");
	
	Serial.print ("Setting PINS ...");
    // Set up the digital pin 2 to an Interrupt and Pin 4 to an Output
    pinMode(PINDATA, INPUT);
	Serial.print ("OK\n");
	
    Serial.print ("Initialising gLCD ...");
    GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen
	Serial.print ("OK\n");
	
    bytesReceived = 0;
    counter=0;
	bitsCounter=0;
	curPos=0;
	
	Serial.print ("Display IntroScreen ...");
    introScreen();
	Serial.print ("OK\n");
	GLCD.ClearScreen();
	
    //Attach the interrupt to the input pin and monitor for ANY Change
    attachInterrupt(pbIn, stateChange, RISING);
}

void loop(){
	
	
	
    if (read != bytesReceived){
		
		
		//bits[curPos] = (char)(((int)'0')+ringbuffer[read]);
		//Serial.println(ringbuffer[read], BIN);
		
		read++;
		curPos++;
		
		if (curPos==8){
			curPos=0;
		}
		
		if (read==BUFFERSIZE) {
			read=0;
		}
		
        
        if (bytesReceived==1024){
			uint8_t x=0;
			uint8_t y=0;
			GLCD.DrawArray(ringbuffer);
            Serial.println("Transmission complete!");
            Serial.println("Received all 1024 Bytes");
            bytesReceived=0;
			counter=0;
        }
	}
}

void stateChange(){

    addToBuffer(digitalRead(PINDATA));
}
