//
//  wand.c
//  gbMon2
//
//  Created by michael on 04.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include <stdio.h>
// Last updated 2010/04/18 13:04

/*
 Create a simple grayscale gradient using Pixel Iterators
 */
//#include <windows.h>
#include <wand/magick_wand.h>
#include <stdint.h>

typedef unsigned char byte;

uint8_t GLCD_Data[128*8];

byte reverseByte(byte a) {
    int i;
    byte b = 0;
	
    for ( i = 0 ; i < 8 ; i ++)
    {
        b <<= 1;
        b |=  ( (a & (1 << i)) >> i);
    }
    return b;
}

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

int main() {
	MagickWand *m_wand = NULL;
	PixelWand *p_wand = NULL;
	DrawingWand *d_wand = NULL;
	PixelIterator *iterator = NULL;
	PixelWand **pixels = NULL;
	int x,y,gray;
	char hex[128];
	
	MagickWandGenesis();
	
	p_wand = NewPixelWand();
	PixelSetColor(p_wand,"white");
	m_wand = NewMagickWand();
	// Create a 100x100 image with a default of white
	MagickNewImage(m_wand,128,64,p_wand);
	

	d_wand = NewDrawingWand();
	PixelSetColor(p_wand,"black");
	DrawSetFillColor(d_wand,p_wand);
	
	
	
	int i;
	for(i = 0; i < 128*8; i++) {
		printf(reverseByte(GLCD_Data[i]));
	}
	
	
	DrawPoint(d_wand,1,0);
	DrawPoint(d_wand,1,1);
	DrawPoint(d_wand,1,2);
	DrawPoint(d_wand,1,3);
	DrawPoint(d_wand,1,4);
	MagickDrawImage(m_wand,d_wand);

	/*
	// Get a new pixel iterator
	iterator=NewPixelIterator(m_wand);
	for(y=0;y<100;y++) {
		// Get the next row of the image as an array of PixelWands
		pixels=PixelGetNextIteratorRow(iterator,&x);
		// Set the row of wands to a simple gray scale gradient
		for(x=0;x<100;x++) {
			gray = x*255/100;
			sprintf(hex,"#%02x%02x%02x",gray,gray,gray);
			PixelSetColor(pixels[x],hex);
		}
		// Sync writes the pixels back to the m_wand
		PixelSyncIterator(iterator);
	}
	*/
	
	
	MagickWriteImage(m_wand,"display.jpg");
	
	// Clean up
	if (d_wand) d_wand = DestroyDrawingWand(d_wand);
	if (p_wand) p_wand = DestroyPixelWand(p_wand);
	if (m_wand) m_wand = DestroyMagickWand(m_wand);
	MagickWandTerminus();
	
}