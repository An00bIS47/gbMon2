/* temperatureInv10x10 bitmap file for GLCD library */
/* Bitmap created from temperatureInv10x10.bmp      */
/* Date: 20 Oct 2013      */
/* Image Pixels = 100    */
/* Image Bytes  = 20     */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef temperatureInv10x10_H
#define temperatureInv10x10_H

static uint8_t temperatureInv10x10[] PROGMEM = {
  10, // width
  10, // height

  /* page 0 (lines 0-7) */
  0xff,0xff,0x7f,0x3f,0x81,0xd4,0x81,0x3f,0x7f,0xff,
  /* page 1 (lines 8-15) */
  0x3,0x3,0x3,0x2,0x1,0x1,0x1,0x2,0x3,0x3,
};
#endif
