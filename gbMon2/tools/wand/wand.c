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

void test_wand() {
	MagickWand *m_wand = NULL;
	PixelWand *p_wand = NULL;
	PixelIterator *iterator = NULL;
	PixelWand **pixels = NULL;
	int x,y,gray;
	char hex[128];
	
	MagickWandGenesis();
	
	p_wand = NewPixelWand();
	PixelSetColor(p_wand,"white");
	m_wand = NewMagickWand();
	// Create a 100x100 image with a default of white
	MagickNewImage(m_wand,100,100,p_wand);
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
	MagickWriteImage(m_wand,"bits_demo.gif");
	
	// Clean up
	iterator=DestroyPixelIterator(iterator);
	DestroyMagickWand(m_wand);
	MagickWandTerminus();
	
}