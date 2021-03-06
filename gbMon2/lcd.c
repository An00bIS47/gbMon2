//
//  lcd.c
//  gbMon2
//
//  Created by michael on 27.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include <stdio.h>
#include "lcd.h"


uint8_t GLCD_Data[128*8];
char GLCD_Umlauts[] = UMLAUTS;
char* GLCD_UmlautReplace[] = UMLAUTS_REPLACE;


void sendByte(unsigned char b) {
	int i;
	for(i = 0; i < 8; i++) {
		int j;
		for(j = 0; j < REPEAT; j++) {
			digitalWrite(DATAPIN, (b & (1 << i)) ? 1 : 0);
			//printf("%d ", (b & (1 << i)) ? 1 : 0);
			//delayMicroseconds(DELAYTIME);
		}
		
		for(j = 0; j < REPEAT; j++) {
			digitalWrite(CLKPIN, 1);
			delayMicroseconds(DELAYTIME);
		}
		for(j = 0; j < REPEAT; j++) {
			digitalWrite(CLKPIN, 0);
			delayMicroseconds(DELAYTIME);
		}
	}
}

// ---------------------------------------------------------------------------
GLCDD_Font* createFont(const uint8_t* name) {
	GLCDD_Font* font = (GLCDD_Font*)malloc(sizeof(GLCDD_Font));
	font->name = (uint8_t*)name;
	font->color = 0;
	return font;
}


// ---------------------------------------------------------------------------
int isUmlaut(char c) {
	if(c == -61) return 1;
	else return 0;
}

// ---------------------------------------------------------------------------
char* replaceUmlaut(char uml) {
	int i;
	for(i = 0; i < UMLAUTS_COUNT; i++) {
		if(GLCD_Umlauts[i] == uml) return GLCD_UmlautReplace[i];
	}
	return NULL;
}

// ---------------------------------------------------------------------------
void GLCDD_SetPixel(uint8_t x, uint8_t y) {
	GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)] |= 1 << (y & 7);
}

// ---------------------------------------------------------------------------
uint8_t GLCDD_GetPixel(uint8_t x, uint8_t y) {
	return (GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)]) & (1 << (y & 7));
}

// ---------------------------------------------------------------------------
void GLCDD_ClearPixel(uint8_t x, uint8_t y) {
	GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)] &= ~(1 << (y & 7));
}

// ---------------------------------------------------------------------------
void GLCDD_TogglePixel(uint8_t x, uint8_t y) {
	GLCD_Data[(x & 127) + (((y & 63) >> 3) << 7)] ^= (1 << (y & 7));
}

// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
int16_t GLCDD_abs(int16_t a) {
	if(a < 0) return -a; else return a;
}

// ---------------------------------------------------------------------------
void GLCDD_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	int8_t deltax, deltay, numpixels, d, dinc1, dinc2, xinc1, xinc2, yinc1, yinc2, x, y, i;
	deltax = GLCDD_abs(x2-x1);
	deltay = GLCDD_abs(y2-y1);
	
	if(deltax >= deltay) {
		numpixels = deltax+1;
		d = (2*deltay) - deltax;
		dinc1 = deltay << 1;
		dinc2 = (deltay-deltax) << 1;
		xinc1 = 1;
		xinc2 = 1;
		yinc1 = 0;
		yinc2 = 1;
	} else {
		numpixels = deltay+1;
		d = (2*deltax) - deltay;
		dinc1 = deltax << 1;
		dinc2 = (deltax-deltay) << 1;
		xinc1 = 0;
		xinc2 = 1;
		yinc1 = 1;
		yinc2 = 1;
	}
	
	if(x1 > x2) {
		xinc1 = -xinc1;
		xinc2 = -xinc2;
	}
	if(y1 > y2) {
		yinc1 = -yinc1;
		yinc2 = -yinc2;
	}
	
	x=x1;
	y=y1;
	
	for(i = 1; i <= numpixels; i++) {
		GLCDD_SetPixel(x, y);
		if(d < 0) {
			d = d + dinc1;
			x = x + xinc1;
			y = y + yinc1;
		} else {
			d = d + dinc2;
			x = x + xinc2;
			y = y + yinc2;
		}
	}
}

// ---------------------------------------------------------------------------
void GLCDD_Circle(uint8_t x, uint8_t y, uint8_t r, uint8_t filled) {
	uint8_t xCenter, yCenter, radius;
	int8_t p, xx, yy;
	
	xCenter = x;
	yCenter = y;
	radius = r;
	
	xx = 0;
	yy = radius;
	p = 3 - 2*radius;
	
	while(xx <= yy) {
		GLCDD_SetPixel(xCenter + xx, yCenter + yy);
		GLCDD_SetPixel(xCenter - xx, yCenter + yy);
		GLCDD_SetPixel(xCenter + xx, yCenter - yy);
		GLCDD_SetPixel(xCenter - xx, yCenter - yy);
		
		GLCDD_SetPixel(xCenter + yy, yCenter + xx);
		GLCDD_SetPixel(xCenter - yy, yCenter + xx);
		GLCDD_SetPixel(xCenter + yy, yCenter - xx);
		GLCDD_SetPixel(xCenter - yy, yCenter - xx);
		
		if(filled) {
			GLCDD_Line(xCenter - xx, yCenter + yy, xCenter + xx, yCenter + yy);
			GLCDD_Line(xCenter + xx, yCenter - yy, xCenter - xx, yCenter - yy);
			GLCDD_Line(xCenter + yy, yCenter + xx, xCenter - yy, yCenter + xx);
			GLCDD_Line(xCenter + yy, yCenter - xx, xCenter - yy, yCenter - xx);
		}
		
		if(p < 0) {
			xx++;
			p += 4*xx + 6;
		} else {
			xx++;
			yy--;
			p += 4*(xx-yy) + 10;
		}
	}
}

// ---------------------------------------------------------------------------
void GLCDD_Rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t filled) {
	int xx, yy;
	if(filled) {
		for(xx = x; xx <= x+w; xx++) {
			for(yy = y; yy <= y+h; yy++) {
				GLCDD_SetPixel(xx, yy);
			}
		}
	} else {
		for(xx = x; xx < x+w; xx++) {
			GLCDD_SetPixel(xx, y);
			GLCDD_SetPixel(xx, y+h-1);
		}
		for(yy = y; yy < y+h; yy++) {
			GLCDD_SetPixel(x, yy);
			GLCDD_SetPixel(x+w-1, yy);
		}
	}
}

// ---------------------------------------------------------------------------
void GLCDD_Invert(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	uint8_t x, y;
	for(x = x1; x <= x2; x++) {
		for(y = y1; y <= y2; y++) {
			GLCDD_TogglePixel(x, y);
		}
	}
}

// ---------------------------------------------------------------------------
void GLCDD_ClearEx(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	uint8_t x, y;
	for(x = x1; x <= x2; x++) {
		for(y = y1; y <= y2; y++) {
			GLCDD_ClearPixel(x, y);
		}
	}
}


// ---------------------------------------------------------------------------
void GLCDD_RectRounded(uint8_t x1, uint8_t y1, uint8_t w, uint8_t h, uint8_t filled) {
	GLCDD_Rectangle(x1, y1, w, h, filled);
	GLCDD_ClearPixel(x1, y1);
	GLCDD_ClearPixel(x1 + w, y1);
	GLCDD_ClearPixel(x1, y1 + h);
	GLCDD_ClearPixel(x1 + w, y1 + h);
}

// ---------------------------------------------------------------------------
void GLCDD_XBMDraw(uint8_t* img, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	uint8_t __x, __y, __b, __rw;
	if((w & 7) == 0) __rw = w;
	else __rw = ((w >> 3) + 1) << 3;
	for(__y = 0; __y < h; __y++) {
		for(__x = 0; __x < w; __x++) {
			__b = *(img + (__x + __y*__rw)/8);
			if(__b & (1 << ((__x + __y*__rw) & 7))) GLCDD_SetPixel(x + __x, y + __y);
		}
	}
}




// ---------------------------------------------------------------------------
int GLCDD_CharWidth(const uint8_t* font, char c) {
	uint8_t firstChar = font[4];
	uint8_t charCount = font[5];
	
	if(c < firstChar || c >= (firstChar+charCount)) {
		return 0;
	}
	c-= firstChar;
	
	return font[6+c];
}

// ---------------------------------------------------------------------------
int GLCDD_FontHeight(GLCDD_Font* font) {
	return font->name[3];
}


// ---------------------------------------------------------------------------
int GLCDD_StringWidth(GLCDD_Font* font, char* str) {
	int width = 0;
	while(str && *str) {
		width += GLCDD_CharWidth((const uint8_t*)font->name, *str++) + 1;
	}
	return width;
}

// ---------------------------------------------------------------------------
int GLCDD_PutCharCol(const uint8_t *font, int x, int y, char c, uint8_t col) {
	uint8_t width = 0;
	uint8_t height = font[3];
	uint8_t bytes = (height+7)/8;
	
	uint8_t firstChar = font[4];
	uint8_t charCount = font[5];
	
	uint16_t index = 0;
	int start_x = x;
	
	if(c < firstChar || c >= (firstChar+charCount)) {
		return 0;
	}
	c-= firstChar;
	
	// read width data, to get the index
	uint8_t i;
	for(i=0; i<c; i++) {
		index += font[6+i];
	}
	index = index*bytes+charCount+6;
	width = font[6+c];
	
	// last but not least, draw the character
	for(i=0; i<bytes; i++) {
		uint8_t j;
		uint8_t page = i*width;
		for(j=0; j<width; j++) {
			uint8_t data = font[index+page+j];
			
			if(height < (i+1)*8) {
				data >>= (i+1)*8-height;
			}
			
			int k;
			for(k = 0; k < 8; k++) {
				if(data & (1 << k)) {
					if(col == 0) GLCDD_SetPixel(x, y + k);
					else GLCDD_ClearPixel(x, y + k);
				}
			}
			x++;
		}
		x = start_x;
		y += 8;
	}
	return width + 1;
}

// ---------------------------------------------------------------------------
int GLCDD_PutChar(const uint8_t *font, int x, int y, char c) {
	return GLCDD_PutCharCol(font, x, y, c, 0);
}


// ---------------------------------------------------------------------------
int GLCDD_PrintGeneric(const uint8_t* font, int x, int y, int max_w, char* str, uint8_t col, uint8_t del_before) {
	int w = 0, uml;
	while(str && *str) {
		if(isUmlaut(*str)) {
			uml = GLCDD_PrintGeneric(font, x, y, max_w - w, replaceUmlaut(*(str + 1)), col, del_before);
			w += uml;
			x += uml;
			str += 2;
		} else {
			w += GLCDD_CharWidth(font, *str) + 1;
			if(max_w != -1 && w > max_w) break;
			x += GLCDD_PutCharCol(font, x, y, *str, col);
			str++;
		}
	}
	return w;
}


// ---------------------------------------------------------------------------
int GLCDD_Print(GLCDD_Font* font, GLCDD_Rect* area, char* str) {
	return GLCDD_PrintGeneric(font->name, area->x, area->y, area->w, str, font->color, 0);
}

// ---------------------------------------------------------------------------
// Added ..int color ... -> kann auch weggelassen werden, wenn font->color genutzt wird
int GLCDD_Printf(GLCDD_Font* font, int color, GLCDD_Rect* area, char* format, ...) {
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	//int w = GLCDD_PrintGeneric(font->name, area->x, area->y, area->w, buffer, font->color, 0);
	int w = GLCDD_PrintGeneric(font->name, area->x, area->y, area->w, buffer, color, 0);
	va_end(args);
	return w;
}
