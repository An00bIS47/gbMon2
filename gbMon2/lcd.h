//
//  lcd.h
//  gbMon2
//
//  Created by michael on 27.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_lcd_h
#define gbMon2_lcd_h

#include <wiringPi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>

#define UMLAUTS_COUNT     7
//                         ä     ö     ü     Ä     Ö     Ü     ß
#define UMLAUTS          {-92,  -74,  -68,  -124,  -106,  -100,  -97}
#define UMLAUTS_REPLACE  {"ae", "oe", "ue", "Ae", "Oe", "Ue", "ss"}

#define REPEAT 1
#define DATAPIN 0
#define CLKPIN 3
//#define DELAYTIME 35
#define DELAYTIME 20

void sendByte(unsigned char b);


typedef struct {
	uint8_t* name;
	uint8_t color;
} GLCDD_Font;

typedef struct {
	int x, y, w, h;
} GLCDD_Rect;

/*
 * Fonts
 */
GLCDD_Font* fnt_dejavu_9;
GLCDD_Font* fnt_dejavu_9b;
GLCDD_Font* fnt_spaceLex_12;
GLCDD_Font* fnt_spaceLex_5;
GLCDD_Font* fnt_spaceLex_8;
GLCDD_Font* fnt_spaceLex_8b;

GLCDD_Font* createFont(const uint8_t* name);

void GLCDD_SetPixel(uint8_t x, uint8_t y);
uint8_t GLCDD_GetPixel(uint8_t x, uint8_t y);
void GLCDD_ClearPixel(uint8_t x, uint8_t y);
void GLCDD_TogglePixel(uint8_t x, uint8_t y);
void GLCDD_Clear();
void GLCDD_Line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void GLCDD_Circle(uint8_t x, uint8_t y, uint8_t r, uint8_t filled);
void GLCDD_Rectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t filled);
void GLCDD_RectRounded(uint8_t x1, uint8_t y1, uint8_t w, uint8_t h, uint8_t filled);
int GLCDD_PutCharCol(const uint8_t *font, int x, int y, char c, uint8_t color);
int GLCDD_PutChar(const uint8_t *font, int x, int y, char c);
int GLCDD_Print(GLCDD_Font* font, GLCDD_Rect* area, char* str);
int GLCDD_Printf(GLCDD_Font* font, int color, GLCDD_Rect* area, char* format, ...);
int GLCDD_CharWidth(const uint8_t* font, char c);
int GLCDD_StringWidth(GLCDD_Font* font, char* str);
int GLCDD_FontHeight(GLCDD_Font* font);
void GLCDD_Invert(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void GLCDD_ClearEx(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void GLCDD_XBMDraw(uint8_t* img, uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/*
 void GLCDD_SetSimulate(uint8_t sim);
 void GLCDD_Init();
void GLCDD_Draw();
void GLCDD_BacklightTimeout(uint32_t seconds);
void GLCDD_BacklightReset();
void GLCDD_BacklightUpdate();
*/
#endif
