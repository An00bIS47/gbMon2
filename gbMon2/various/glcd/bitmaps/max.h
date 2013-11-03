/* max bitmap file for GLCD library */
/* Bitmap created from max.bmp      */
/* Date: 21 Oct 2013      */
/* Image Pixels = 64    */
/* Image Bytes  = 8     */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef max_H
#define max_H

static uint8_t max[] PROGMEM = {
  8, // width
  8, // height

  /* page 0 (lines 0-7) */
  0x8,0xc,0x7e,0x7f,0x7e,0xc,0x8,0x0,
};
#endif
