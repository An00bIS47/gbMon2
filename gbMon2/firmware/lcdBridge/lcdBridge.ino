/*
 * GLCDexample/Users/michael/Development/Arduino/ArduinoSPI_Sample/ArduinoSPI_Sample.ino
 *
 * Basic test code for the Arduino GLCD library.
 * This code exercises a range of graphic functions supported
 * by the library and provides examples of its use.
 * It also gives an indication of performance, showing the
 * number of frames drawn per second.
 */

#include <glcd.h>

#include "fonts/allFonts.h"         // system and arial14 fonts are used
#include "bitmaps/allBitmaps.h"       // all images in the bitmap dir


/*
 * SPI
 */
#include <SPI.h>

/*
 * Declare Icons
 */
Image_t bootIcon;

/*
 * Declare SPI Variables
 */
char buf [1000];
volatile byte pos;
volatile boolean process_it;


// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;



void countdown(int count){
    while(count--){  // do countdown
        delay(500);
    }  
}

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
        GLCD.BarGraph(10, 40, 108, 8, 2, i);
        countdown(1);
    }
     
    
    printScreenTemplate();
}

void printHex(int num, int precision) {
	char tmp[16];
	char format[128];
	
	sprintf(format, "0x%%.%dX", precision);
	
	sprintf(tmp, format, num);
	Serial.println(tmp);
}



// SPI interrupt routine
ISR (SPI_STC_vect) {
    byte c = SPDR;  // grab byte from SPI Data Register
    
	Serial.println(">>> Interrupt!");
	printHex(c, 2);
	
    // add to buffer if room
    if (pos < sizeof buf)
    {
        buf [pos++] = c;
		if (pos==1) {
			Serial.println("pos===1");
			process_it = true;
		}
		 //Serial.println (buf[pos], HEX);
		
        //SPI.transfer(c);
        //Serial.print(c);
        // example: newline means time to process buffer
        if (c == '\n') {
			Serial.println("BREAK - ZERO TERMINATED STRING");
			//Serial.println("BREAK - newLine");
			//Serial.println(buf);
			process_it = true;
		}
        
    }  // end of room available
}  // end of interrupt routine SPI_STC_vect




void setup()
{
    Serial.begin (115200);   // debugging
    Serial.print ("Starting gbMon2 lcdBridge...\n");

	Serial.print ("Setting PINS ...");
	// have to send on master in, *slave out*
    pinMode(MISO, OUTPUT);
    pinMode(led, OUTPUT);
	Serial.print ("OK\n");
	
    Serial.print ("Initialising gLCD ...");
    GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen
	Serial.print ("OK\n");
    
    SPI.setBitOrder(MSBFIRST);
	
	// turn on SPI in slave mode
    SPCR |= _BV(SPE);
    
    // get ready for an interrupt
    pos = 0;   // buffer empty
    process_it = false;
    
    Serial.print ("Display IntroScreen ...");
    introScreen();
	Serial.print ("OK\n");
	
	Serial.print ("Turning on interrupts ...OK\n");
	// now turn on interrupts
    SPI.attachInterrupt();

    //GLCD.DrawBitmap(bootIcon, 32,0); //draw the bitmap at the given x,y position
}



void loop() {
    
	/*
    drawStatusWifi(2,1, 84);
    drawStatusNetwork(12, 1 , 1);
    drawStatusLight(117, 1, 1);
    drawStatusFan(107, 1, 1);
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);
    
    drawStatusLight(117, 1, 0);
    drawStatusFan(107, 1, 0);
    drawStatusNetwork(12, 1 , 0);
    drawStatusWifi(2,1, 4);
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);
	 */

    
	 
    if (process_it) {
		Serial.println("Processing ...");
        buf [pos] = 0;

        //if (buf == "")
        GLCD.CursorToXY(GLCD.Width/2 - 44, 45);
		Serial.println(buf);
        GLCD.print(buf);
        
        pos = 0;
        process_it = false;
		Serial.println("Processing done");
    }  // end of flag set
    
}

