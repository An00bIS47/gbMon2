/*
 * GLCDexample
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

Image_t icon;

void setup()
{
  GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen
  if(GLCD.Height >= 64)   
    icon = Apple;  // the 64 pixel high icon

  //introScreen();
  GLCD.ClearScreen(); 

  GLCD.SelectFont(System5x7, BLACK); // font for the default text area
}


void loop() {
  GLCD.DrawBitmap(icon, 32,0); //draw the bitmap at the given x,y position
}