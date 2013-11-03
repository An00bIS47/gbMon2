//
//  interruptTest.ino
//  gbMon2
//
//  Created by michael on 25.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <glcd.h>

#include "fonts/allFonts.h"         // system and arial14 fonts are used
#include "bitmaps/allBitmaps.h"       // all images in the bitmap dir

#define BUFFERSIZE 1024

int pbIn = 0;                  // Interrupt 0 is on DIGITAL PIN 2!
int PINDATA = 3;                // DATA Pin - DIGITAL PIN 3
int counter;

volatile int state = LOW;      // The input state toggle
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


void drawStatusLight(int x, int y, int on){
    GLCD.SetDot( x+3, y, WHITE);GLCD.SetDot( x+4, y, WHITE);
    GLCD.SetDot( x+2, y+1, WHITE);GLCD.SetDot( x+5, y+1, WHITE);
    GLCD.SetDot( x+1, y+2, WHITE);GLCD.SetDot( x+6, y+2, WHITE);
    GLCD.SetDot( x+1, y+3, WHITE);GLCD.SetDot( x+6, y+3, WHITE);
    GLCD.SetDot( x+2, y+4, WHITE);GLCD.SetDot( x+5, y+4, WHITE);
    GLCD.SetDot( x+2, y+5, WHITE);GLCD.SetDot( x+3, y+5, WHITE); GLCD.SetDot( x+4, y+5, WHITE);     GLCD.SetDot( x+5, y+5, WHITE);
    GLCD.SetDot( x+3, y+6, WHITE);GLCD.SetDot( x+4, y+6, WHITE);
    GLCD.SetDot( x+3, y+7, WHITE);GLCD.SetDot( x+4, y+7, WHITE);
    
    if (on==1){
        GLCD.SetDot( x+3, y+1, WHITE); GLCD.SetDot( x+4, y+1, WHITE);
        GLCD.SetDot( x+2, y+2, WHITE); GLCD.SetDot( x+3, y+2, WHITE); GLCD.SetDot( x+4, y+2, WHITE); GLCD.SetDot( x+5, y+2, WHITE);
        GLCD.SetDot( x+2, y+3, WHITE); GLCD.SetDot( x+3, y+3, WHITE); GLCD.SetDot( x+4, y+3, WHITE); GLCD.SetDot( x+5, y+3, WHITE);
        GLCD.SetDot( x+3, y+4, WHITE); GLCD.SetDot( x+4, y+4, WHITE);
    } else {
        GLCD.SetDot( x+3, y+1, BLACK); GLCD.SetDot( x+4, y+1, BLACK);
        GLCD.SetDot( x+2, y+2, BLACK); GLCD.SetDot( x+3, y+2, BLACK); GLCD.SetDot( x+4, y+2, BLACK); GLCD.SetDot( x+5, y+2, BLACK);
        GLCD.SetDot( x+2, y+3, BLACK); GLCD.SetDot( x+3, y+3, BLACK); GLCD.SetDot( x+4, y+3, BLACK); GLCD.SetDot( x+5, y+3, BLACK);
        GLCD.SetDot( x+3, y+4, BLACK); GLCD.SetDot( x+4, y+4, BLACK);
    }
    
}

void drawStatusFan(int x, int y, int on){
    
    if (on==1) {
        GLCD.SetDot( x+4, y, WHITE);    GLCD.SetDot( x+5, y, WHITE);
        GLCD.SetDot( x+1, y+1, WHITE);GLCD.SetDot( x+4, y+1, WHITE);GLCD.SetDot( x+5, y+1, WHITE); GLCD.SetDot( x+6, y+1, WHITE);
        GLCD.SetDot( x, y+2, WHITE);        GLCD.SetDot( x+2, y+2, WHITE);        GLCD.SetDot( x+5, y+2, WHITE);
        GLCD.SetDot( x, y+3, WHITE);    GLCD.SetDot( x+1, y+3, WHITE);    GLCD.SetDot( x+2, y+3, WHITE);    GLCD.SetDot( x+3, y+3, WHITE);    GLCD.SetDot( x+4, y+3, WHITE);
        GLCD.SetDot( x+3, y+4, WHITE);    GLCD.SetDot( x+4, y+4, WHITE);    GLCD.SetDot( x+5, y+4, WHITE);    GLCD.SetDot( x+6, y+4, WHITE);    GLCD.SetDot( x+7, y+4, WHITE);
        GLCD.SetDot( x+2, y+5, WHITE);    GLCD.SetDot( x+3, y+5, WHITE);    GLCD.SetDot( x+5, y+5, WHITE);        GLCD.SetDot( x+7, y+5, WHITE);
        GLCD.SetDot( x+1, y+6, WHITE);       GLCD.SetDot( x+3, y+6, WHITE);    GLCD.SetDot( x+6, y+6, WHITE);
        GLCD.SetDot( x+2, y+7, WHITE);    GLCD.SetDot( x+3, y+7, WHITE);
        
        GLCD.SetDot( x+4, y+2, WHITE);
        GLCD.SetDot( x+1, y+3, WHITE);
        GLCD.SetDot( x+6, y+5, WHITE);
        GLCD.SetDot( x+2, y+6, WHITE);
    } else {
        GLCD.SetDot( x+4, y, BLACK);    GLCD.SetDot( x+5, y, BLACK);
        GLCD.SetDot( x+1, y+1, BLACK);GLCD.SetDot( x+4, y+1, BLACK);GLCD.SetDot( x+5, y+1, BLACK); GLCD.SetDot( x+6, y+1, BLACK);
        GLCD.SetDot( x, y+2, BLACK);        GLCD.SetDot( x+2, y+2, BLACK);        GLCD.SetDot( x+5, y+2, BLACK);
        GLCD.SetDot( x, y+3, BLACK);    GLCD.SetDot( x+1, y+3, BLACK);    GLCD.SetDot( x+2, y+3, BLACK);    GLCD.SetDot( x+3, y+3, BLACK);    GLCD.SetDot( x+4, y+3, BLACK);
        GLCD.SetDot( x+3, y+4, BLACK);    GLCD.SetDot( x+4, y+4, BLACK);    GLCD.SetDot( x+5, y+4, BLACK);    GLCD.SetDot( x+6, y+4, BLACK);    GLCD.SetDot( x+7, y+4, BLACK);
        GLCD.SetDot( x+2, y+5, BLACK);    GLCD.SetDot( x+3, y+5, BLACK);    GLCD.SetDot( x+5, y+5, BLACK);        GLCD.SetDot( x+7, y+5, BLACK);
        GLCD.SetDot( x+1, y+6, BLACK);       GLCD.SetDot( x+3, y+6, BLACK);    GLCD.SetDot( x+6, y+6, BLACK);
        GLCD.SetDot( x+2, y+7, BLACK);    GLCD.SetDot( x+3, y+7, BLACK);
        
        GLCD.SetDot( x+4, y+2, BLACK);
        GLCD.SetDot( x+1, y+3, BLACK);
        GLCD.SetDot( x+6, y+5, BLACK);
        GLCD.SetDot( x+2, y+6, BLACK);
    }
	
    
}

void drawStatusWifi(int x, int y, int strength){
    if (strength > 19 ) {
        GLCD.DrawVLine( x+1, y+6, 1, WHITE);
    } else {
        GLCD.DrawVLine( x+1, y+6, 1, BLACK);
    }
    if (strength > 39 ) {
        GLCD.DrawVLine( x+3, y+4, 3, WHITE);
    } else {
        GLCD.DrawVLine( x+3, y+4, 3, BLACK);
    }
    if (strength > 59 ) {
        GLCD.DrawVLine( x+5, y+2, 5, WHITE);
    } else {
        GLCD.DrawVLine( x+5, y+2, 5, BLACK);
    }
    if (strength > 79 ) {
        GLCD.DrawVLine( x+7, y,   7, WHITE);
    } else {
        GLCD.DrawVLine( x+7, y,   7, BLACK);
    }
}


void countdown(int count){
    while(count--){  // do countdown
        delay(500);
    }
}

/**
 * drawStatusNetwork - draws the network online icon;
 *  Starts at top left
 *  x       =   x start coordinate
 *  y       =   y start coordinate
 *  online  =   1 >> online
 *              0 >> offline
 */
void drawStatusNetwork(int x, int y, int online){
    // Rahmen
    GLCD.SetDot( x+2, y, WHITE);GLCD.SetDot( x+5, y, WHITE);
    GLCD.SetDot( x+2, y+1, WHITE);GLCD.SetDot( x+5, y+1, WHITE);
    GLCD.SetDot( x+1, y+2, WHITE);GLCD.SetDot( x+2, y+2, WHITE);GLCD.SetDot( x+3, y+2, WHITE);GLCD.SetDot( x+4, y+2, WHITE);GLCD.SetDot( x+5,y+2, WHITE);GLCD.SetDot( x+6, y+2, WHITE);
    GLCD.SetDot( x+1, y+3, WHITE);GLCD.SetDot( x+6, y+3, WHITE);
    GLCD.SetDot( x+1, y+4, WHITE);GLCD.SetDot( x+6, y+4, WHITE);
    GLCD.SetDot( x+2, y+5, WHITE);GLCD.SetDot( x+ 5, y+5, WHITE);
    GLCD.SetDot( x+3, y+6, WHITE);GLCD.SetDot( x+4, y+6, WHITE);
    GLCD.SetDot( x+3, y+7, WHITE);GLCD.SetDot( x+4, y+7, WHITE);
    
    
    if (online==1) {
        GLCD.SetDot( x+2, y+3, WHITE);GLCD.SetDot( x+3, y+3, WHITE);GLCD.SetDot( x+4, y+3, WHITE);GLCD.SetDot( x+5, y+3, WHITE);
        GLCD.SetDot( x+2, y+4, WHITE);GLCD.SetDot( x+3, y+4, WHITE);GLCD.SetDot( x+4, y+4, WHITE);GLCD.SetDot( x+5, y+4, WHITE);
        GLCD.SetDot( x+3, y+5, WHITE);GLCD.SetDot( x+4, y+5, WHITE);
    } else {
        GLCD.SetDot( x+2, y+3, BLACK);GLCD.SetDot( x+3, y+3, BLACK);GLCD.SetDot( x+4, y+3, BLACK);GLCD.SetDot( x+5, y+3, BLACK);
        GLCD.SetDot( x+2, y+4, BLACK);GLCD.SetDot( x+3, y+4, BLACK);GLCD.SetDot( x+4, y+4, BLACK);GLCD.SetDot( x+5, y+4, BLACK);
        GLCD.SetDot( x+3, y+5, BLACK);GLCD.SetDot( x+4, y+5, BLACK);
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



int readBuffer(){

}

void printScreenTemplate(){
    //GLCD.DrawHLine(0, 2, 128);
    //GLCD.DrawHLine(62, 2, 128);
    //GLCD.DrawVLine(0, 2, 128);
    
    
    // Header
    GLCD.ClearScreen();
    GLCD.InvertRect(0, 0, 127, 9 );
    //GLCD.DrawRect(0, 10, 127, 53);
    
    // Status Icons
    drawStatusWifi(2,1, 50);
    drawStatusNetwork(12, 1 , 0);
    drawStatusFan(107, 1, 0);
    drawStatusLight(117, 1, 1);
    
    //GLCD.DrawBitmap(wifi80, 1, 1, WHITE);
    //GLCD.DrawBitmap(networkOn, 10, 1, WHITE);
    //GLCD.DrawBitmap(fanOn, 111, 1, WHITE);
    //GLCD.DrawBitmap(lightOn, 119, 1, WHITE);
    
    /*
	 GLCD.CursorToXY((GLCD.Width-GLCD. StringWidth("Temperature 1h"))/2, 2);
	 GLCD.SelectFont(SystemFont5x7, WHITE);
	 GLCD.print("Temperature 1h");
	 GLCD.DrawBitmap(temperature, 1,1, WHITE);
	 */
    
    /*
	 GLCD.SelectFont(Arial14, BLACK);
	 GLCD.CursorToXY(30,12);
	 GLCD.print("29");
	 
	 GLCD.CursorToXY(70,12);
	 GLCD.print("28");
	 
	 GLCD.CursorToXY(90,12);
	 GLCD.print("20");
	 */
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
	
    
    //printScreenTemplate();
}

int fromBinary(char *s) {
	return (int) strtol(s, NULL, 2);
}

void setup() {
    Serial.begin(115200);
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
			/*
			Serial.print("  -  Bits: ");
			Serial.print(bits);
			Serial.print("  -  Dec: ");
			Serial.print(fromBinary(bits), DEC);
			Serial.println("");
			 */
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
    //Serial.println("INTERRUPT");
    //state = !state;
    //digitalWrite(ledOut, state);
    //int sensorValue = digitalRead(PINDATA);
    //Serial.println(sensorValue, DEC);
    //Serial.print(sensorValue, DEC);
    
    //received[counter]=digitalRead(PINDATA);
    addToBuffer(digitalRead(PINDATA));
}
