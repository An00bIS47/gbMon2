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

/* Circular buffer object */
typedef struct {
    int         size;   /* maximum number of elements           */
    int         start;  /* index of oldest element              */
    int         end;    /* index at which to write new element  */
    Data   *elems;  /* vector of elements                   */
} CircularBuffer;

int cbIsEmpty(CircularBuffer *cb);
void cbInit(CircularBuffer *cb, int size);
void cbFree(CircularBuffer *cb);
int cbIsFull(CircularBuffer *cb);
void cbWrite(CircularBuffer *cb, Data *elem);
void cbRead(CircularBuffer *cb, Data *elem);


#endif
