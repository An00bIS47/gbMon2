//
//  lcddisplay.c
//  gbMon2
//
//  Created by michael on 30.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "lcddisplay.h"

uint8_t		GLCD_Data[128*8];

const uint8_t blank[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const uint8_t clientOff[] = { 0x24,0x24,0x7e,0x42,0x42,0x24,0x18,0x18};
const uint8_t clientOn[] = { 0x24,0x24,0x7e,0x7e,0x7e,0x3c,0x18,0x18};
const uint8_t fanOn[] = { 0x30,0x72,0x37,0x1f,0xf8,0xec,0x4e,0x0c};
const uint8_t lightOff[] = { 0x18,0x24,0x42,0x42,0x24,0x24,0x18,0x18};
const uint8_t lightOn[] = { 0x18,0x3c,0x7e,0x7e,0x3c,0x3c,0x18,0x18};
const uint8_t wifi20[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02};
const uint8_t wifi40[] = { 0x00,0x00,0x00,0x00,0x08,0x08,0x0a,0x0a};
const uint8_t wifi60[] = { 0x00,0x00,0x20,0x20,0x28,0x28,0x2a,0x2a};
const uint8_t wifi80[] = { 0x80,0x80,0xa0,0xa0,0xa8,0xa8,0xaa,0xaa};

// ---------------------------------------------------------------------------
void displayFan(){
	if (getFan()==1){
		GLCDD_ClearEx(108, 0, 115, 8);
		GLCDD_XBMDraw((uint8_t*)fanOn, 108, 0, 8, 8);
	} else {
		GLCDD_ClearEx(108, 0, 115, 8);
		GLCDD_XBMDraw((uint8_t*)blank, 108, 0, 8, 8);
	}

}

// ---------------------------------------------------------------------------
void displayClient(){
	sem_wait(&semaLockInfo);
	GLCDD_ClearEx(12, 0, 19, 8);
	if (clientIsConnected==false) {
		GLCDD_XBMDraw((uint8_t*)clientOff, 12, 0, 8, 8);
	} else if (clientIsConnected==true) {
		GLCDD_XBMDraw((uint8_t*)clientOn, 12, 0, 8, 8);
	} else {
		GLCDD_XBMDraw((uint8_t*)blank, 12, 0, 8, 8);
	}
	sem_post(&semaLockInfo);       // up semaphore
}
// ---------------------------------------------------------------------------
void displayWifi(int strength){
	// display Client Status if strength > 1
	if (strength>1){
		displayClient();
	} else {
		GLCDD_XBMDraw((uint8_t*)blank, 12, 0, 8, 8);
	}
	
	GLCDD_ClearEx(2, 0, 9, 8);
	if (strength < 20 ) {
		GLCDD_XBMDraw((uint8_t*)blank, 2, 0, 8, 8);
	}
	if (strength < 40 ) {
		GLCDD_XBMDraw((uint8_t*)wifi20, 2, 0, 8, 8);
	}
	if (strength < 40 ) {
		GLCDD_XBMDraw((uint8_t*)wifi40, 2, 0, 8, 8);
	}
	if (strength < 60 ) {
		GLCDD_XBMDraw((uint8_t*)wifi60, 2, 0, 8, 8);
	}
	if (strength > 81 ) {
		GLCDD_XBMDraw((uint8_t*)wifi80, 2, 0, 8, 8);
	}
}

void displaySettings(){
	GLCDD_Rect r;
	
	r.x = 6;
	r.w = 128 - 12;
	r.h = -1;
	
	
	//GLCDD_Rectangle(0,  9, 128, 16, 0);
	//GLCDD_Rectangle(2, 11, 124, 12, 0);
	r.y = 13;
	r.x = 2;
	r.w = GLCDD_StringWidth(fnt_spaceLex_8, "Information:");
	GLCDD_Printf(fnt_spaceLex_8, 0, &r, "Information:");
	
	r.x = 15;
	r.y = 25;
	r.w = 128 - r.x - 2;
	GLCDD_Printf(fnt_spaceLex_8, 0, &r, "%s: %s", "IP", getIP(appNetworkInterface));
	
	
	//GLCDD_Rectangle(0, 25, 128, 16, 0);
	//GLCDD_Rectangle(2, 27, 124, 12, 0);
	r.y += 12;
	GLCDD_Printf(fnt_spaceLex_8, 0, &r, "%s: %s", "Version", getVersion());
}


void displayMainScreen(){
	sem_wait(&semaLockInfo);       // down semaphore
	
	GLCDD_Rect r;
	
	r.w = 128 - 10;
	//r.h = -1;
	r.h = 12;
	
	
	// Humidity
	r.x = 5;
	r.y = 12;
	GLCDD_Printf(fnt_spaceLex_12, 0, &r, "%2.1f %%", data.humidity.current);
	r.x = 60;
	r.y = 12;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MAX:%2.1f", data.humidity.max);
	r.y += 7;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MIN:%2.1f", data.humidity.min);
	
	
	// Temperature DHT22	#0
	r.x = 5;
	r.y = 30;
	GLCDD_Printf(fnt_spaceLex_12, 0, &r, "%2.1f", data.temperature[0].current);
	//GLCDD_Printf(fnt_spaceLex_12, 0, &r, "%2.1f", 20.1);
	r.y = 44;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MAX:%2.1f", data.temperature[0].max);
	r.y += 7;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MIN:%2.1f", data.temperature[0].min);
	
	
	// Temperature DS18b20	#1
	r.x = 45;
	r.y = 30;
	GLCDD_Printf(fnt_spaceLex_12, 0, &r, "%2.1f", data.temperature[1].current);
	//GLCDD_Printf(fnt_spaceLex_12, 0, &r, "%2.1f", 20.2);
	r.y = 44;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MAX:%2.1f", data.temperature[1].max);
	r.y += 7;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MIN:%2.1f", data.temperature[1].min);
	
	
	// Temperature DS18b20	#2
	r.x = 85;
	r.y = 30;
	GLCDD_Printf(fnt_spaceLex_12, 0, &r, "%2.1f", data.temperature[2].current);
	//GLCDD_Printf(fnt_spaceLex_12, 0, &r, "%2.1f", 20.3);
	r.y = 44;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MAX:%2.1f", data.temperature[2].max);
	r.y += 7;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "MIN:%2.1f", data.temperature[2].min);
	
	
	//printf("Humidity = %.2f %% Temperature = %.2f *C \n", current.humidity, current.temperature[0] );
	
	//r.x = 6;
	//r.y += 12;
	//GLCDD_Printf(fnt_spaceLex_8, 0, &r, "%s: %llu", "Uptime", getUptime());
	
	sem_post(&semaLockInfo);       // up semaphore
}


void displayStatusBar(){
	
	fnt_spaceLex_5=createFont(spaceLex_5);
	
	// Wifi and Client / Network
	displayWifi(getWifiStrength());
	
	// Fan
	displayFan();
	
	sem_wait(&semaLockInfo);       // down semaphore
	
	// Light
	GLCDD_ClearEx(118, 0, 125, 8);
	if (data.lightValue > 30) {
		GLCDD_XBMDraw((uint8_t*)lightOn, 118, 0, 8, 8);
	} else {
		GLCDD_XBMDraw((uint8_t*)lightOff, 118, 0, 8, 8);
	}
	
	GLCDD_Rect r;
	
	// Time
	char* strTime=getTimeShort();
	r.x = 64-(GLCDD_StringWidth(fnt_spaceLex_5, strTime)/2);
	r.w = GLCDD_StringWidth(fnt_spaceLex_5, strTime);
	r.h = -1;
	r.y = 1;
	GLCDD_Printf(fnt_spaceLex_5, 0, &r, "%s", strTime);
	
	sem_post(&semaLockInfo);       // up semaphore
}


// ---------------------------------------------------------------------------
void* displayMain(void *args){
	
	int i;
	int j;
	int counter;
	

	
	int lastTime=(int)time(NULL);
	//printf("Lasttime: %d", lastTime);
	updateDisplay = true;
	debugPrint(true, true, "Display Thread started", true, "LCDDISPLAY");
	for(;;) {
		
		
		if (getUpdateDisplay()==false) {
			if (lastTime + 60 <= ((int)time(NULL)) ) {
				setUpdateDisplay(true);
				lastTime=(int)time(NULL);
				//printf("Lasttime: %d", lastTime);
			}
		}

		if (getUpdateDisplay()==true) {

			counter=0;
			
			unsigned int start = millis();
		
			// create fonts
			fnt_dejavu_9 = createFont(dejavu_9);
			fnt_dejavu_9b = createFont(dejavu_9_b);
			fnt_spaceLex_12 = createFont(spaceLex_12);
			fnt_spaceLex_8=createFont(spaceLex_8);
			fnt_spaceLex_5=createFont(spaceLex_5);
			fnt_spaceLex_8b=createFont(spaceLex_8b);
			
			GLCDD_Clear();
			
			//GLCDD_Rectangle(0, 0, 128, 9, 1);
			//GLCDD_Rectangle(0, 32, 32, 32, 1);

			
			
			displayStatusBar();
			
			displayMainScreen();

			
			for(i = 0; i < 128*8; i++) {
				sendByte(reverseByte(GLCD_Data[i]));
				counter++;
			}
			delayMicroseconds(DELAYTIME);
			
			
			unsigned int end = millis();
			//printf("\n%d bytes in %dms => %d Bps\n", sizeof(buff), end - start, sizeof(buff) * 1000 / (end - start));
			
			char *string[100];
			sprintf(string,"%d Bytes in %dms => %d Bps", counter, end - start, counter * 1000 / (end - start));
			debugPrint(true, true, string, true, "LCDDISPLAY");
			
			//printf(">>> %d Bytes in %dms => %d Bps\n", counter, end - start, counter * 1000 / (end - start));
			
			simulateDisplay(GLCD_Data);
			
			//printf(">>> %d Byte in %dms => %d Bps\n", (counter/j), (end - start)/j, counter * 1000 / (end - start));
			//printf("Done!\n");
			
			setUpdateDisplay(false);
		}
	
	}
	
}

