//
//  screen-test.c
//  gbMon2
//
//  Created by michael on 25.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

// fonts
#include "../fonts.h"
#include "../lcd.h"

#define REPEAT 1
#define DATAPIN 0
#define CLKPIN 7
//#define DELAYTIME 35
#define DELAYTIME 20

uint8_t GLCD_Data[128*8];

typedef unsigned char byte;

//
//  bitmapStatus.h
//  gbmon2
//
//  Created by michael on 27.10.2013.
//  Copyright (c) 2013 michael. All rights reserved.

const uint8_t fanOff[] = { 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
const uint8_t fanOn[] = { 0xcf,0x8d,0xc8,0xe0,0x07,0x13,0xb1,0xf3};
const uint8_t lightOff[] = { 0xe7,0xdb,0xbd,0xbd,0xdb,0xdb,0xe7,0xe7};
const uint8_t lightOn[] = { 0xe7,0xc3,0x81,0x81,0xc3,0xc3,0xe7,0xe7};
const uint8_t networkOff[] = { 0xdb,0xdb,0x81,0xbd,0xbd,0xdb,0xe7,0xe7};
const uint8_t networkOn[] = { 0xdb,0xdb,0x81,0x81,0x81,0xc3,0xe7,0xe7};
const uint8_t wifi0[] = { 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
const uint8_t wifi20[] = { 0xff,0xff,0xff,0xff,0xff,0xff,0xfd,0xfd};
const uint8_t wifi40[] = { 0xff,0xff,0xff,0xff,0xf7,0xf7,0xf5,0xf5};
const uint8_t wifi60[] = { 0xff,0xff,0xdf,0xdf,0xd7,0xd7,0xd5,0xd5};
const uint8_t wifi80[] = { 0x7f,0x7f,0x5f,0x5f,0x57,0x57,0x55,0x55};



byte reverseByte(byte a) {
    int i;
    byte b = 0;
	
    for ( i = 0 ; i < 8 ; i ++)
    {
        b <<= 1;
        b |=  ( (a & (1 << i)) >> i);
    }
    return b;
}

// ---------------------------------------------------------------------------
GLCDD_Font* createFont(const uint8_t* name) {
	GLCDD_Font* font = (GLCDD_Font*)malloc(sizeof(GLCDD_Font));
	font->name = (uint8_t*)name;
	font->color = 0;
	return font;
}




// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
const char *printBinary(int x) {
    static char b[9];
    b[0] = '\0';
	
    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
	
    return b;
}


void sendByte(unsigned char b) {
	int i;
	for(i = 0; i < 8; i++) {
		int j;
		for(j = 0; j < REPEAT; j++) {
			digitalWrite(DATAPIN, (b & (1 << i)) ? 1 : 0);
			//printf("%d ", (b & (1 << i)) ? 1 : 0);
			//delayMicroseconds(DELAYTIME);
		}
		
		for(j = 0; j < REPEAT; j++) {
			digitalWrite(CLKPIN, 1);
			delayMicroseconds(DELAYTIME);
		}
		for(j = 0; j < REPEAT; j++) {
			digitalWrite(CLKPIN, 0);
			delayMicroseconds(DELAYTIME);
		}
	}
}

void varDump(){
	int i;
	for (i=0; i < 128*8; i++) {
		printf("%4d  %s", i, printBinary(GLCD_Data[i]));
		printf("\t Reversed:  %s \n", printBinary(reverseByte(GLCD_Data[i])));
	}
	
}


int main() {
	if(wiringPiSetup() == -1)  exit(1);
	
	pinMode (CLKPIN, OUTPUT);
	pinMode (DATAPIN, OUTPUT);

	int i;
	int j;
	int counter;
	
	counter=0;
	/*
	unsigned char buff[2] = {85, 0};
	printf("Buffer: \n");
	//printf("=================================================\n");
	printf("  Char    Dec    Hex       Binary \n");
	printf(" ------  -----  -----    ----------\n");
	
	for (i=0; i<sizeof(buff); i++) {
		printf("   %1c \t %3d \t 0x%02x \t 0b%s\n",(char) buff[i], buff[i], buff[i],printBinary(buff[i]));
	}
	*/
	unsigned int start = millis();

	
	GLCDD_Clear();
	GLCDD_Rectangle(0, 0, 128, 9, 1);
	//GLCDD_Rectangle(0, 32, 32, 32, 1);
	
	// Wifi
	GLCDD_ClearEx(2, 1, 9, 8);
	GLCDD_XBMDraw((uint8_t*)wifi80, 2, 1, 8, 8);
	
	// Network
	GLCDD_ClearEx(12, 1, 19, 8);
	GLCDD_XBMDraw((uint8_t*)networkOn, 12, 1, 8, 8);
	
	// Fan
	GLCDD_ClearEx(108, 1, 115, 8);
	GLCDD_XBMDraw((uint8_t*)fanOn, 108, 1, 8, 8);
	
	// Light
	GLCDD_ClearEx(118, 1, 125, 8);
	GLCDD_XBMDraw((uint8_t*)lightOn, 118, 1, 8, 8);
	
	
	// create fonts
	fnt_dejavu_9 = createFont(dejavu_9);
	fnt_dejavu_9b = createFont(dejavu_9_b);
	fnt_silkscreen_8 = createFont(silkscreen_8);
	fnt_spaceLex_8=createFont(spaceLex_8);
	
	GLCDD_Rect r;
	r.x = 64-(GLCDD_StringWidth(fnt_spaceLex_8, "12:34")/2);
	r.w = GLCDD_StringWidth(fnt_spaceLex_8, "12:34");
	r.h = -1;
	
	r.y = 1;
	GLCDD_Printf(fnt_spaceLex_8, 1, &r, "%s", "12:34");
	
	
	r.x = 6;
	r.w = 128 - 12;
	r.h = -1;
	
	r.y = 26;
	GLCDD_Printf(fnt_dejavu_9, 0, &r, "%s: %s", "Test", "test");
	
	r.y = 38;
	GLCDD_Printf(fnt_dejavu_9, 0, &r, "%s: %s", "Version", "1.0");

	
	
	printf("varDump\n");
	varDump();
	
	
	for(j = 0; j < 1; j++) {
		for(i = 0; i < 128*8; i++) {
			sendByte(reverseByte(GLCD_Data[i]));
			counter++;
		}
		delayMicroseconds(DELAYTIME);
	}

	unsigned int end = millis();
	
	printf("counter: %d \n",counter);
	//printf("\n%d bytes in %dms => %d Bps\n", sizeof(buff), end - start, sizeof(buff) * 1000 / (end - start));
	printf("\n>>> %d Bytes in %dms => %d Bps\n", counter, end - start, counter * 1000 / (end - start));
	printf(">>> %d Byte in %dms => %d Bps\n", (counter/j), (end - start)/j, counter * 1000 / (end - start));
	printf("Done!\n");
	
}
