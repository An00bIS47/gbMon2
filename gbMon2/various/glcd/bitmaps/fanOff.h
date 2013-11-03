/* fanOff bitmap file for GLCD library */
/* Bitmap created from fanOff.bmp      */
/* Date: 21 Oct 2013      */
/* Image Pixels = 64    */
/* Image Bytes  = 8     */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef fanOff_H
#define fanOff_H

static uint8_t fanOff[] PROGMEM = {
  8, // width
  8, // height

  /* page 0 (lines 0-7) */
  0xc,0x4a,0xac,0xf8,0x1f,0x35,0x52,0x30,
};
#endif
