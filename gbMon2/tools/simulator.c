//
//  simulator.c
//  gbMon2
//
//  Created by michael on 25.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>                  /*  for sleep()  */
#include <curses.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#define ignore_result(x) ({ typeof(x) z = x; (void)sizeof z; })

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define SPACE_LEFT 15
#define SPACE_TOP 5

#define FONT_WIDTH 4
#define FONT_HEIGHT 5

uint8_t GLCD_Data[128*8];
uint8_t GLCD_Mode = 1; // 0=hardware, 1=simulate, 2=bmp
uint8_t* GLCD_Map;
int GLCD_MapFd;

int stringWidth;

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

void displayBorderAndScala(){
    int i;
    int j;
    
    // Display Scala Top & Bottom
    for (i=SPACE_LEFT+1; i<LCD_WIDTH+SPACE_LEFT+1; i++) {
        if ((i-SPACE_LEFT-1) % 10 == 0 ) {
            mvprintw(SPACE_TOP-2,i,"|");
            mvprintw(SPACE_TOP+3+LCD_HEIGHT,i,"|");
            j++;
        }
        mvprintw(SPACE_TOP-1,i,"%d",(i-SPACE_LEFT-1) % 10);
        mvprintw(SPACE_TOP+2+LCD_HEIGHT,i,"%d",(i-SPACE_LEFT-1) % 10);
    }
    
    // Display Scala Left & Right
    for (i=SPACE_TOP+1; i<LCD_HEIGHT+SPACE_TOP+1; i++) {
        if ((i-SPACE_TOP-1) % 10 == 0 ) {
            mvprintw(i,SPACE_LEFT-4,"-");
            mvprintw(i,SPACE_LEFT+LCD_WIDTH+4,"-");
            j++;
        }
        mvprintw(i,SPACE_LEFT-3,"%2d",(i-SPACE_TOP-1) % 10);
        mvprintw(i, SPACE_LEFT+LCD_WIDTH+2, "%2d",(i-SPACE_TOP-1) % 10);
    }
    
    
    // Display Border
    for (i=SPACE_LEFT; i<LCD_WIDTH+1+SPACE_LEFT+1; i++) {
        mvaddstr(SPACE_TOP, i, "=");
        mvaddstr(SPACE_TOP+1+LCD_HEIGHT, i, "=");
    }
    
    for (i=SPACE_TOP+1; i<LCD_HEIGHT+SPACE_TOP+2; i++) {
        mvaddstr(i, SPACE_LEFT+0, "|");
        mvaddstr(i, SPACE_LEFT+LCD_WIDTH+1, "|");
    }
}

void putChar(int x, int y, char *str){
    x=x+SPACE_TOP+1;
    y=y+SPACE_LEFT+1;
    
    mvprintw(x,y,str);
}

void clearScreen(){
    int x=SPACE_TOP+1;
    int y=SPACE_LEFT+1;
    int i;
    int j;
    
    unsigned char *c=" ";
    stringWidth=0;
    
    for (j=0; j<LCD_HEIGHT; j++){
        for (i=0; i<LCD_WIDTH/2; i++) {
            mvprintw(x+j, y+i, c);
            mvprintw(x+j, y+i+LCD_WIDTH/2, c);
        }
    }
    
}


// ---------------------------------------------------------------------------
int16_t GLCDD_abs(int16_t a) {
	if(a < 0) return -a; else return a;
}

// ---------------------------------------------------------------------------
void GLCDD_SetPixel(uint8_t x, uint8_t y) {
	GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)] |= 1 << (y & 7);
}

void drawChar(unsigned char c[]){
    int i;
    int j;
    int k;
    
    
    
    for (i=0; i<FONT_WIDTH; i++) {
        for (j=0; j<FONT_HEIGHT; j++) {
            k=(j*FONT_WIDTH)+i;
            mvprintw(2, 50, "\t k: %3d \t c: %1d", k, c[k]);
            if (c[k]==1) {
                putChar(j,i+stringWidth,"*");
            } else {
                putChar(j,i+stringWidth," ");
            }
        }
    }
    
    stringWidth=stringWidth+FONT_WIDTH;
}

// ---------------------------------------------------------------------------
void GLCDD_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	int8_t deltax, deltay, numpixels, d, dinc1, dinc2, xinc1, xinc2, yinc1, yinc2, x, y, i;
	deltax = GLCDD_abs(x2-x1);
	deltay = GLCDD_abs(y2-y1);
	
	if(deltax >= deltay) {
		numpixels = deltax+1;
		d = (2*deltay) - deltax;
		dinc1 = deltay << 1;
		dinc2 = (deltay-deltax) << 1;
		xinc1 = 1;
		xinc2 = 1;
		yinc1 = 0;
		yinc2 = 1;
	} else {
		numpixels = deltay+1;
		d = (2*deltax) - deltay;
		dinc1 = deltax << 1;
		dinc2 = (deltax-deltay) << 1;
		xinc1 = 0;
		xinc2 = 1;
		yinc1 = 1;
		yinc2 = 1;
	}
	
	if(x1 > x2) {
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if(y1 > y2) {
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}
	
	x=x1;
	y=y1;
	
	for(i = 1; i <= numpixels; i++) {
		GLCDD_SetPixel(x, y);
		if(d < 0) {
			d = d + dinc1;
			x = x + xinc1;
			y = y + yinc1;
		} else {
			d = d + dinc2;
			x = x + xinc2;
			y = y + yinc2;
		}
	}
}

// ---------------------------------------------------------------------------
void GLCDD_Rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t filled) {
	int xx, yy;
	if(filled) {
		for(xx = x; xx <= x+w; xx++) {
			for(yy = y; yy <= y+h; yy++) {
				GLCDD_SetPixel(xx, yy);
			}
		}
	} else {
		for(xx = x; xx < x+w; xx++) {
			GLCDD_SetPixel(xx, y);
			GLCDD_SetPixel(xx, y+h-1);
		}
		for(yy = y; yy < y+h; yy++) {
			GLCDD_SetPixel(x, yy);
			GLCDD_SetPixel(x+w-1, yy);
		}
	}
}

// ---------------------------------------------------------------------------
void GLCDD_Clear() {
	uint8_t x;
	for(x = 0; x < 128; x++) {
		GLCD_Data[x + 128*0] = 0;
		GLCD_Data[x + 128*1] = 0;
		GLCD_Data[x + 128*2] = 0;
		GLCD_Data[x + 128*3] = 0;
		GLCD_Data[x + 128*4] = 0;
		GLCD_Data[x + 128*5] = 0;
		GLCD_Data[x + 128*6] = 0;
		GLCD_Data[x + 128*7] = 0;
	}
}

int main(int argc, const char * argv[]){
    int x;
    int y;
	int i;
    int j;
    int counter;
	
    WINDOW * mainwin;
    
    
    /*  Initialize ncurses  */
    
    if ( (mainwin = initscr()) == NULL ) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    
    
    /*  Display "Hello, world!" in the centre of the
     screen, call refresh() to show our changes, and
     sleep() for a few seconds to get the full screen effect  */
    
    //int mvaddstr(int y, int x, const char *str);
    mvaddstr(0, 0, "Welcome to gLCD Simulator");
    
    // Display Settings
    mvaddstr(1, 0, "Display Settings");
    mvprintw(2, 0, "\t Width: %3d \t Height: %3d",LCD_WIDTH, LCD_HEIGHT);
    
	if(GLCD_Mode == 1) { // set up memory mapped output file
		GLCD_MapFd = open("lcd.sim", O_RDWR | O_CREAT | O_TRUNC, 0777);
		if(GLCD_MapFd == -1) {
			perror("Error opening file 'lcd.sim' for writing");
			mvprintw(2, 0, "Error opening file 'lcd.sim' for writing");
			sleep(3);
			return;
		}

		ignore_result(write(GLCD_MapFd, "\0", 128*8)); // write 1k 0-byte to file
		GLCD_Map = (uint8_t*)mmap(0, 128*8, PROT_READ | PROT_WRITE, MAP_SHARED, GLCD_MapFd, 0);
		if(GLCD_Map == MAP_FAILED) {
			mvprintw(2, 0, "Error mapping file 'lcd.sim'");
			perror("Error mapping file 'lcd.sim'");
			sleep(3);
			return;
		}
	}
	
    displayBorderAndScala();
	
	//char *c="01000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000010000000100000001000000";
	
	char *c="10111101";
	GLCDD_Clear();
	
	refresh();
	sleep(5);
	GLCDD_Rectangle(0,0,8,8,1);
	
	//char myArray[] = { 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101, 0b10111101,};
	
	//drawHLine();
	//refresh();
	/*
	for (i=0; i<128; i++) {
		myArray[i]=0b11111111;
	}
	*/
	/*
	size_t myArraySize = sizeof(GLCD_Data);  // myArraySize
	
	for(x=0; x < myArraySize; x++) {
		mvprintw(2, 0, "\t Width: %3d \t Height: %3d \t Length: %d \t i: %3d \t Value: %s",LCD_WIDTH, LCD_HEIGHT, myArraySize, x, printBinary(GLCD_Data[x]));
		
		char *bit = printBinary(GLCD_Data[x]);
		for (i=0; i<8; i++){
			if (bit[i]=='1') {
				putChar(i,x,"*");
			} else {
				putChar(i,x," ");
			}
		}
		refresh();
		//sleep(1);
		
	}

	*/
	refresh();
	memcpy(GLCD_Map, GLCD_Data, 128*8);
	
	counter=0;
	/*
	for(i=0; i< strlen(c); i++) {
		for (x=0; x<128; x++){
			for (y=0; y<8; y++){
				mvprintw(2, 0, "\t Width: %3d \t Height: %3d \t Length: %d \t Counter: %4d \t Value: %c",LCD_WIDTH, LCD_HEIGHT, strlen(c), counter, c[counter]);
				if (c[counter]=='1') {
					putChar(y+(i*8),x,"*");
				} else {
					putChar(y+(i*8),x," ");
				}
				counter++;
				//refresh();
			}
			if (counter >= strlen(c)) {
				break;
			}
		}
		if (counter >= strlen(c)) {
			break;
		}
	}
	/*
	for (i=0; i<128; i++){
		for (j=0; j < 8; j++) {
			mvprintw(2, 0, "\t Width: %3d \t Height: %3d \t Length: %d \t Counter: %4d \t Value: %c",LCD_WIDTH, LCD_HEIGHT, strlen(c), counter, c[counter]);
			if (c[counter]=='1') {
				putChar(j,i,"*");
			} else {
				putChar(j,i," ");
			}
			counter++;
			refresh();
		}
	}
	
    for (i=0; i<64; i++) {
        putChar(i,i,"*");
    }
	*/
	
    refresh();
    sleep(10);
    
	
	
	/*
    clearScreen();
    refresh();
    
    unsigned char H[]={1,0,1,0,
        1,0,1,0,
        1,1,1,0,
        1,0,1,0,
        1,0,1,0};
    
    unsigned char A[]={1,1,1,0,
        1,0,1,0,
        1,1,1,0,
        1,0,1,0,
        1,0,1,0};
    
    unsigned char L[]={1,0,0,0,
        1,0,0,0,
        1,0,0,0,
        1,0,0,0,
        1,1,1,0};
    
    unsigned char O[]={1,1,1,0,
        1,0,1,0,
        1,0,1,0,
        1,0,1,0,
        1,1,1,0};
    
    stringWidth=0;
    drawChar(H);
    drawChar(A);
    drawChar(L);
    drawChar(L);
    drawChar(O);
    
    refresh();
    sleep(5);
    */
    
    /*  Clean up after ourselves  */
    
    delwin(mainwin);
    endwin();
    refresh();
    
    return EXIT_SUCCESS;
}