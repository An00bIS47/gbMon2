//
//  debug.h
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbmon2_debug_h
#define gbmon2_debug_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdbool.h>
#include "helpers.h"
#include "pinLayout.h"


#include "main.h"


void debugPrintPinLayout ();
int myprintf( const char * format, ... );

/**
	Prints debug information on screen
	@param info if true it prints out " DEBUG MAIN: "
	@param prefix if true it prints out "TIME >>> "
	@param msg the message to print
	@param newLine if true a "\n"
	@param instance prints info which function
 */
void debugPrint(bool info, bool prefix, char *msg, bool newLine, char *instance);


void debugPrintInfo();


#endif
