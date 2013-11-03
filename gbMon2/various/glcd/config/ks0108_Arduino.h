/*
 * ks0108_Arduino.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set io pins
 * This version is for a standard ks0108 display
 * connected using the default Arduino wiring
 *
*/

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Arduino"

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/
/* Data pin definitions
 */


/*
 
 GoldenTek GG-1206H0 mit KS0108
 
 PIN      NAME             DESC               ARDUINO
-----  --------------  --------------------  ---------
  1     VCC(+5V)        LogicLevel               -
  2     GND (0V)        Ground                   -
  3     VEE (-2V/-5V)   Contrast                 -
  4     DB0             Bidirect. Data           8
  5     DB1             Bidirect. Data           9
  6     DB2             Bidirect. Data          10
  7     DB3             Bidirect. Data          11
  8     DB4             Bidirect. Data           4
  9     DB5             Bidirect. Data           5
 10     DB6             Bidirect. Data           6
 11     DB7             Bidirect. Data           7
 12     CS1             Chip Select 1           14 (Analog 0)
 13     CS2             Chip Select 2           15 (Analog 1)
 14     RESET           Reset (L=active)         -
 15     R/W             Read/(L=)Write          16 (Analog 2)
 16     DI              Data/(L=)Instruction    17 (Analog 3)
 17     E               Enable                  18 (Analog 4)
 18     GND (0V)        Ground                   -
 19     LED+            Backlight +              -
 20     LED-            Backlight -              -
 
 
 */
#define glcdData0Pin        8       // 8
#define glcdData1Pin        9       // 9
#define glcdData2Pin        10      // 10
#define glcdData3Pin        11      // 11
#define glcdData4Pin        4       // 4
#define glcdData5Pin        5       // 5
#define glcdData6Pin        6       // 6
#define glcdData7Pin        7       // 7

/* Arduino pins used for Commands
 * default assignment uses the first five analog pins
 */

#define glcdCSEL1        14
#define glcdCSEL2        15

#if NBR_CHIP_SELECT_PINS > 2
#define glcdCSEL3         3   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdCSEL4         2   // fourth chip select if needed
#endif

#define glcdRW           16
#define glcdDI           17
#define glcdEN           18
// Reset Bit  - uncomment the next line if reset is connected to an output pin
//#define glcdRES          19    // Reset Bit

#endif //GLCD_PIN_CONFIG_H
