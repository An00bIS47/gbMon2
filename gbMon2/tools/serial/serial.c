//
//  serial.c
//  gbMon2
//
//  Created by michael on 07.03.14.
//  Copyright (c) 2014 michael. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <wiringSerial.h>
#define NOECSENSORS 3						// Number of EC Sensors

//Struktur fuer die Daten
typedef struct
{
    //Schluesselwert
    int key;
    //Name
    char inChar;
} userdata_t;

//Struktur fuer einen Ringbuffer-Handler
//wird benoetigt, um mehrere Listen zu verwalten
typedef struct
{
    //Index zum Lesen
    int readIndex;
    //Index zum Schreiben
    int writeIndex;
    //Platz fuer Speicherelemente, eigentlicher Buffer
    userdata_t *fifo;
    //Groesse des Buffers, d.h. Anzahl der Elemente
    int size;
} ringbuffer_handler_t;


/*
 * Substring
 *********************************************************************************
 */
char* substring(const char* str, size_t begin, size_t len){
    if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
        return 0;
    
    return strndup(str + begin, len);
}


//eine Funktion, um einen Ringbuffer anzulegen
//Parameter size: Groesse des Buffers (Anzahl der Elemente)
//Rückgabewert: Zeiger auf Ringbuffer-Handler
ringbuffer_handler_t *createFIFO(int size)
{
    //Zeiger auf Ringbuffer-Handler deklarieren und genuegend Speicher
    //reservieren
    ringbuffer_handler_t *buffer =
	(ringbuffer_handler_t *)malloc(sizeof(ringbuffer_handler_t));
    
    //Werte des Handler belegen
    //readIndex und WriteIndex zu Anfang 0
    //fuer fifo muss genuegend Speicher reserviert werden,
    //um size Datenelemente aufnehmen zu koennen
    //size gibt Anzahl der Elemente im Ringbuffer an (aus Parameter)
    buffer->readIndex=0;
    buffer->writeIndex=0;
    buffer->fifo = (userdata_t *)malloc(sizeof(userdata_t) * (size + 1));
    buffer->size = size;
	
    //Zeiger auf Ringbuffer-Handler zurueckgeben
    return buffer;
}

/*-------------------------------------------------------------------------*/

//eine Funktion, um Elemente anzuhaengen, d.h. in den Buffer zu schreiben
//Parameter data: Daten, die in den Buffer geschrieben werden sollen
//Parameter *buffer: Zeiger auf Ringbuffer-Handler
//kein Rueckgabewert
void appendFIFO(userdata_t data, ringbuffer_handler_t *buffer)
{
    //Ringbuffer-Handler muss gueltig sein
    if(buffer)
    {
		//Daten in den entsprechenden Platz im Buffer schreiben
		//dafuer ist writeIndex da
		buffer->fifo[buffer->writeIndex] = data;
		//writeIndex wird fuer das naechste Schreiben auf den neuen Wert
		//gesetzt
		//Achtung: wenn der Buffer voll ist, wird wieder vorne hineinge-
		//schrieben (deshalb RINGbuffer), Daten koennen verloren gehen,
		//wenn nicht zwischendurch gelesen wird
		buffer->writeIndex = buffer->writeIndex++ % (buffer->size+1);
		//an dieser Stelle ist es deshalb sinnvoll, den readIndex auf den
		//letzen wahren Wert zu setzen, wenn NICHT zwischendurch gelesen
		//wurde
		if(buffer->readIndex == buffer->writeIndex)
			buffer->readIndex = buffer->readIndex ++ % (buffer->size+1);
    }
}

/*-------------------------------------------------------------------------*/

//eine Funktion, um Elemente aus dem Buffer zu lesen
//Parameter *data: Zeiger auf ein Datenelement, das die gelesenen Daten
//aufnehmen soll
//Parameter *buffer: Zeiger auf Ringbuffer-Handler
//Rueckgabewert -1, wenn nicht gelesen werden konnte
//Rueckgabewert 1, wenn gelesen werden konnte
int readFIFO(userdata_t *data, ringbuffer_handler_t *buffer)
{
    //Ringbuffer-Handler muss gueltig sein
    if(buffer)
    {
		//der writeIndex zeigt immer auf das naechste zu beschreibende Element,
		//d.h. dieses Element ist noch nicht beschrieben worden und enthaelt
		//deshalb keine gueltigen Daten
		//wenn readIndex also gleich writeIndex, darf nicht gelesen werden
		if(buffer->readIndex != buffer->writeIndex)
		{
			//Daten kopieren
			*data = buffer->fifo[buffer->readIndex];
			//readIndex fuer das naechste Lesen hochsetzen
			buffer->readIndex = buffer->readIndex++ % (buffer->size+1);
			//Rueckgabewert 1, da gelesen wurde
			return 1;
		}
		else
			//es konnten keine Daten gelesen werden, da keine gueltigen Daten
			//Rueckgabewert also -1
			return -1;
    }
    //es konnte nicht gelesen werden, da Buffer-Handler nicht gueltig,
    //Ruckegabewert also -1
    else return -1;
}


int main (){
	int fd ;
	
	//eine Variable fuer die Daten
    userdata_t data;
    //ein Ringbuffer-Handler
    ringbuffer_handler_t *buffer;
    //eine Variable, um Ergebnisse von readFIFO() abzufragen
    int ergebnis;
	
    //einen Ringbuffer-Handler und damit auch einen Ringbuffer anlegen
    buffer = createFIFO(255);
	

	
	
	char inData[255];	// Allocate some space for the string
	char inChar = -1;	// Where to store the character read
	int spos = 0;		//Index into array; where to store the character
	
	printf("Welcome to SerialRead\n");
	if ((fd = serialOpen ("/dev/ttyAMA0", 115200)) < 0){
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	
	char buttons[8+1];
	char ldr[8+1];
	int counter=0;
	int i;
	int curPos=8;
	
	for (;;) {
		while (serialDataAvail(fd)) {
			inChar = (serialGetchar(fd));
			
			//Daten anlegen und in den Buffer schreiben
			data.key=spos;
			strcpy(&data.inChar, &inChar);
			appendFIFO(data, buffer);
			
			//inData[spos]= inChar;
			spos++;
			
			//Daten aus Buffer holen, wenn erfolgreich, dann ausgeben
			ergebnis = readFIFO(&data, buffer);
			if(ergebnis == 1)
			{
				printf("%d %c\n", data.key, data.inChar);
			}
			
			/*
			//inData[spos]= '\0';
			if (inChar == 10) {			// LF 10 ; CR 13
				//printf("%s\n",inData);
				
				
				//if (strncmp(inData,"11100111",8) == 0) {
				//	printf("*** FRAME ***\n");
				//}
				
				printf("Buttons:\t%s\n", substring(inData,curPos,8));
				curPos=curPos+8;
				printf("LDR:\t%s\n", substring(inData,16,8));
				curPos=curPos+8;
				
				for (i=0; i<NOECSENSORS; i++) {
					printf("EC %d:\t%s\n",i, substring(inData,curPos,8));
					curPos=curPos+8;
				}
				
				//if (strcmp(substring(inData,curPos,8),"11100111") == 0) {
				//	printf("*** FRAME ***\n");
				//}
				
				spos = 0;
				//inData[spos]='\0';
				
				//fflush (stdout) ;
			} // if (inChare == 13
			 */
			
		} // while
	} //for
}
