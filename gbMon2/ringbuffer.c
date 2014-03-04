//
//  ringbuffer.c
//  gbMon2
//
//  Created by michael on 30.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include "ringbuffer.h"

//eine Funktion, um einen Ringbuffer anzulegen
//Parameter size: Groesse des Buffers (Anzahl der Elemente)
//Rückgabewert: Zeiger auf Ringbuffer-Handler
ringbuffer_handler_t *bufferInit(int size) {
	
    //Zeiger auf Ringbuffer-Handler deklarieren und genuegend Speicher
    //reservieren
    ringbuffer_handler_t *buffer = (ringbuffer_handler_t *)malloc(sizeof(ringbuffer_handler_t));
    
    //Werte des Handler belegen
    //readIndex und WriteIndex zu Anfang 0
    //fuer fifo muss genuegend Speicher reserviert werden,
    //um size Datenelemente aufnehmen zu koennen
    //size gibt Anzahl der Elemente im Ringbuffer an (aus Parameter)
    buffer->readPointer=0;
    buffer->writePointer=0;
    buffer->fifo = (Data *)malloc(sizeof(Data) * (size + 1));
    buffer->size = size;
	buffer->dataSize = 0;
	
    //Zeiger auf Ringbuffer-Handler zurueckgeben
    return buffer;
}

// adds a char
void bufferPush(Data data, ringbuffer_handler_t *buffer) {
	
	if(buffer){
		// increase write_pointer, check if at end of array
		if (++buffer->writePointer >= buffer->size) {
			buffer->writePointer = 0;
		}
		
		//buffer[write_pointer] = c;
		buffer->fifo[buffer->writePointer] = data;
		
		printf("\n--> PUSHED IN DATA AT INDEX: %d", buffer->writePointer);
		buffer->dataSize++;
	}
}


// returns 1 if buffer is full, 0 if buffer is not full, -1 if error
int bufferIsFull(ringbuffer_handler_t *buffer) {
	if (buffer) {
		return buffer->readPointer == buffer->writePointer && buffer->dataSize == buffer->size;
	} else
		return -1;
}


// returns 1 if buffer is empty, 0 if buffer is not empty, -1 if error
int bufferIsEmpty(ringbuffer_handler_t *buffer) {
	if (buffer) {
		return buffer->readPointer == buffer->writePointer && buffer->dataSize == 0;
	} else
		return -1;
}


// pull char from queue
void bufferPop(ringbuffer_handler_t *buffer) {
	if (++buffer->readPointer >= buffer->size) {
		buffer->readPointer = 0;
	}
	
	//printf("\nPopped char %c", buffer->fifo[buffer->readPointer]);
	printf("\n--> POPPED OUT DATA AT INDEX: %d", buffer->readPointer);
	
	// enter space on place of read char so we can see it is removed
	// buffer->fifo[buffer->readPointer]= 0x20;
	
	buffer->dataSize--;
}
