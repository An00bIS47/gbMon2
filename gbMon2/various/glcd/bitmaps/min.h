/* min bitmap file for GLCD library */
/* Bitmap created from min.bmp      */
/* Date: 21 Oct 2013      */
/* Image Pixels = 64    */
/* Image Bytes  = 8     */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef min_H
#define min_H

static uint8_t min[] PROGMEM = {
  8, // width
  8, // height

  /* page 0 (lines 0-7) */
  0x10,0x30,0x7e,0xfe,0x7e,0x30,0x10,0x0,
};
#endif
