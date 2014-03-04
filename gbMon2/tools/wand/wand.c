//
//  wand.c
//  gbMon2
//
//  Created by michael on 04.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include <stdio.h>
#include <wand/magick_wand.h>
#include <stdint.h>


typedef unsigned char byte;

uint8_t GLCD_Data[128*8];


char* substring(const char* str, size_t begin, size_t len){
    if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
        return 0;
    
    return strndup(str + begin, len);
}

const char *printBinary(int x) {
    static char b[9];
    b[0] = '\0';
	
    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
	
    return b;
}

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
	
	int i, j, counter;
	counter=0;
	
	for (j = 0; j < 8; j++) {
		for(i = 0; i < 128; i++) {
			printf("%s ",printBinary(GLCD_Data[counter]));
			
			for (k=0; k<8; k++) {
				char pixel = substring(printBinary(GLCD_Data[counter]), k+(j*8), 1);
				if(pixel==1){
					DrawPoint(d_wand,k,i);
				}
			}
			counter++;
		}
		printf("\n");
	}
	

//	DrawPoint(d_wand,1,1);
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