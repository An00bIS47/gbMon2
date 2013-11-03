/* temperature bitmap file for GLCD library */
/* Bitmap created from temperature.bmp      */
/* Date: 21 Oct 2013      */
/* Image Pixels = 64    */
/* Image Bytes  = 8     */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef temperature_H
#define temperature_H

static uint8_t temperature[] PROGMEM = {
  8, // width
  8, // height

  /* page 0 (lines 0-7) */
  0x0,0x0,0x60,0x9e,0x81,0x9e,0x60,0x0,
};
#endif
