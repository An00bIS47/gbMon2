//
//  data.h
//  gbMon2
//
//  Created by michael on 30.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#ifndef gbMon2_data_h
#define gbMon2_data_h

#define NumberOfTemperatureSensors 3
#define NumberOfECSensors 3

#include <stdio.h>

typedef struct {
    float min;
    float max;
    float current;
} Humidity;

typedef struct {
    int min;
    int max;
    int current;
} EcLevel;

typedef struct {
    float min;
    float max;
    float current;
} Temperature;

typedef struct {
    Temperature temperature[NumberOfTemperatureSensors];
    Humidity humidity;
    EcLevel ecLevel[NumberOfECSensors];
    int lightValue;
} Data;

Data data;
Data ringbuffer[100];

// Getter
Temperature getTemperature(Data *this, int i);
Humidity getHumidity(Data *this);
EcLevel getecLevel(Data *this, int i);

// Setter
void setTemperature(Data *this, Temperature temperature, int i);
void setHumidity(Data *this, Humidity humidity);
void setecLevel(Data *this, EcLevel ecLevel, int i);

void printData(Data this);
void data_init(Data *this, Temperature temperature[4], Humidity humidity);
/*
 void Auto_fahre(Auto *this);
 void Auto_hupe(Auto *this, int anzahl);
 */
#endif