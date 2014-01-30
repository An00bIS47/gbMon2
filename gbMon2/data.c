//
//  data.c
//  gbMon2
//
//  Created by michael on 30.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//


#include "data.h"

Temperature getTemperature(Data *this, int i){
    return this->temperature[i];
}

void setTemperature(Data *this, Temperature temperature, int i){
    this->temperature[i]=temperature;
}

Humidity getHumidity(Data *this){
    return this->humidity;
}


void setHumidity(Data *this, Humidity humidity){
    this->humidity=humidity;
}

EcLevel getecLevel(Data *this, int i){
    return this->ecLevel[i];
}

void setecLevel(Data *this, EcLevel ecLevel, int i){
    this->ecLevel[i]=ecLevel;
}


void data_init(Data *this, Temperature temperature[4], Humidity humidity){
    this->temperature[0].min = 0.0;
    this->humidity.min = 0.0;
}

int getLightValue(Data *this){
    return this->lightValue;
}

void printData(Data this){
    int i;
    Temperature temp;
    Humidity hum;
    EcLevel ecLevel;
    
    printf("Current Data:\n");
    for (i=0; i<NumberOfTemperatureSensors ; i++){
        temp = getTemperature(&this, i);
        printf("Temperature %i:\n", i);
        printf("\t min: %f\n", temp.min);
        printf("\t min: %f\n", temp.max);
        printf("\t current: %f\n", temp.current);
    }
    
    hum = getHumidity(&this);
    printf("Humidity:\n");
    printf("\t min: %f\n", hum.min);
    printf("\t min: %f\n", hum.max);
    printf("\t current: %f\n", hum.current);
    
    printf("Light Value: %i", this.lightValue);
    
    for (i=0; i<NumberOfECSensors ; i++){
        ecLevel = getecLevel(&this, i);
        printf("ecLevel %i:\n", i);
        printf("\t min: %f\n", ecLevel.min);
        printf("\t min: %f\n", ecLevel.max);
        printf("\t current: %f\n", ecLevel.current);
    }
    printf("===============================================================\n");
}

