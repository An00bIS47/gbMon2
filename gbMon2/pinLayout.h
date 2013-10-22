//
//  pinLayout.h
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbmon2_pinLayout_h
#define gbmon2_pinLayout_h

/*
 
 My mappings of the KS0108 registers:
 
 
 LCD PIN  MCP23017 PIN  Name   Purpose
 
 ---- Wire these pins together as shown ------
 
 --- Port "A" - control lines
 LCD PIN  MCP23017 PIN  Name   Purpose
 6      28 (GPA7)     E      Enable data transfer on 1 -> 0 transition  (see LCD_ENABLE)
 5      27 (GPA6)     R/~W   1 = read, 0 = write (to LCD) (see LCD_READ)
 4      26 (GPA5)     D/~I   1 = data, 0 = instruction    (see LCD_DATA)
 17      25 (GPA4)     ~RST   1 = not reset, 0 = reset
 16      24 (GPA3)     CS2    Chip select for IC2 (1 = active)  (see LCD_CS2)
 15      23 (GPA2)     CS1    Chip select for IC1 (1 = active)  (see LCD_CS1)
 */

#define MCP23S17_BASE         100

#define LCD_CS1     (MCP23S17_BASE +  23)   // chip select 1  (pin 23)                            0x04
#define LCD_CS2     (MCP23S17_BASE +  24)   // chip select 2  (pin 24)                            0x08
#define LCD_RESET   (MCP23S17_BASE +  25)   // reset (pin 25)                                     0x10
#define LCD_DATA    (MCP23S17_BASE +  26)   // 1xxxxxxx = data; 0xxxxxxx = instruction  (pin 26)  0x20
#define LCD_READ    (MCP23S17_BASE +  27)   // x1xxxxxx = read; x0xxxxxx = write  (pin 27)        0x40
#define LCD_ENABLE  (MCP23S17_BASE +  28)   // enable by toggling high/low  (pin 28)              0x80

/*
 --- Port "B" - data lines
 LCD PIN  MCP23017 PIN  Name   Purpose
 7      1  (GPB0)     DB0    Data bit 0
 8      2  (GPB1)     DB1    Data bit 1
 9      3  (GPB2)     DB2    Data bit 2
 10      4  (GPB3)     DB3    Data bit 3
 11      5  (GPB4)     DB4    Data bit 4
 12      6  (GPB5)     DB5    Data bit 5
 13      7  (GPB6)     DB6    Data bit 6
 14      8  (GPB7)     DB7    Data bit 7
 */
#define LCD_DB0     (MCP23S17_BASE + 8)     // GPB0
#define LCD_DB1     (MCP23S17_BASE + 9)     // GPB1
#define LCD_DB2     (MCP23S17_BASE + 10)    // GPB2
#define LCD_DB3     (MCP23S17_BASE + 11)    // GPB3
#define LCD_DB4     (MCP23S17_BASE + 12)    // GPB4
#define LCD_DB5     (MCP23S17_BASE + 13)    // GPB5
#define LCD_DB6     (MCP23S17_BASE + 14)    // GPB6
#define LCD_DB7     (MCP23S17_BASE + 15)    // GPB7

/*
 ---- Pins on LCD display which are not connected to the I/O expander ----
 LCD PIN  MCP23017 PIN  Name   Purpose
 1                    GND    LCD logic ground
 2                    +5V    LCD logic power
 3                    V0     Contrast - connect to contrast pot, middle (wiper)
 17                   ~RST    Tie to +5V via 10K resistor (reset signal)
 18                    Vee    Negative voltage - connect to contrast pot, one side *
 19                    A      Power supply for LED light (+5V)  A=anode
 20                    K      GND for LED light                 K=cathode
 
 * Third leg of contrast pot is wired to ground.
 
 ---- Pins on MCP23017 which are not connected to the LCD display ----
 LCD PIN  MCP23017 PIN  Name   Purpose
 9   (VDD)            +5V    Power for MCP23017
 10   (VSS)            GND    Ground for MCP23017
 11   (CS)             SS     (Slave Select) - connect to Arduino pin D10 if using SPI (D53 on the Mega)
 12   (SCL/SCK)        SCL    (Clock) - connect to Arduino pin A5 for I2C (D21 on the Mega) (for SPI SCK: D13, or D52 on the Mega)
 13   (SDA/SI)         SDA    (Data)  - connect to Arduino pin A4 for I2C (D20 on the Mega) (for SPI MOSI: D11, or D51 on the Mega)
 14   (SO)             MISO   (SPI slave out) - connect to Arduino pin D12 if using SPI (D50 on the Mega)
 15   (A0)             Address jumper 0 - connect to ground (unless you want a different address)
 16   (A1)             Address jumper 1 - connect to ground
 17   (A2)             Address jumper 2 - connect to ground
 18   (~RST)           Tie to +5V via 10K resistor (reset signal)
 19   (INTA)           Interrupt for port A (not used)
 20   (INTB)           Interrupt for port B (not used)
 21   (GPA0)           Not used
 22   (GPA1)           Not used
 23   (GPA2)           Not used
 
 */

// Commands sent when LCD in "instruction" mode (LCD_DATA bit set to 0)

#define LCD_ON          0x3F
#define LCD_OFF         0x3E
#define LCD_SET_ADD     0x40   // plus X address (0 to 63)
#define LCD_SET_PAGE    0xB8   // plus Y address (0 to 7)
#define LCD_DISP_START  0xC0   // plus X address (0 to 63) - for scrolling

#endif
