//
//  ringbuffer.h
//  gbMon2
//
//  Created by michael on 30.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#ifndef gbMon2_ringbuffer_h
#define gbMon2_ringbuffer_h

#include <stdio.h>
#include <stdlib.h>
#include "data.h"

//Struktur fuer einen Ringbuffer-Handler
//wird benoetigt, um mehrere Listen zu verwalten
typedef struct {
    int readPointer;		//Index zum Lesen
    int writePointer;		//Index zum Schreiben
    Data *fifo;				//Platz fuer Speicherelemente, eigentlicher Buffer
    int size;				//Groesse des Buffers, d.h. Anzahl der Elemente
	int dataSize;			// number of chars in buffer
} ringbuffer_handler_t;


int bufferIsFull(ringbuffer_handler_t *buffer);
int bufferIsEmpty(ringbuffer_handler_t *buffer);
void bufferPush(Data data, ringbuffer_handler_t *buffer);
void bufferPop(ringbuffer_handler_t *buffer);
ringbuffer_handler_t *bufferInit(int size);



#endif
