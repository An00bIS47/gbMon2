/* settings10x10 bitmap file for GLCD library */
/* Bitmap created from settings10x10.bmp      */
/* Date: 20 Oct 2013      */
/* Image Pixels = 100    */
/* Image Bytes  = 20     */

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef settings10x10_H
#define settings10x10_H

static uint8_t settings10x10[] PROGMEM = {
  10, // width
  10, // height

  /* page 0 (lines 0-7) */
  0x60,0x20,0x60,0xf0,0x38,0x1f,0xd,0x8,0xc,0x0,
  /* page 1 (lines 8-15) */
  0x0,0x0,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
};
#endif
