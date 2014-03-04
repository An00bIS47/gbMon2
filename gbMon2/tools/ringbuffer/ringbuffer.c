/* CIRCULAR BUFFER QUEUE */

#include <stdio.h>

#define BUFFER_SIZE 8
#define NumberOfTemperatureSensors 3
#define NumberOfECSensors 3


typedef struct {
    float min;
    float max;
    float current;
} Humidity;

typedef struct {
    float min;
    float max;
    float current;
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


//Struktur fuer die Daten
/*
typedef struct {
    int key;			//Schluesselwert
    char name[20];		//Name
} userdata_t;
*/

//Struktur fuer einen Ringbuffer-Handler
//wird benoetigt, um mehrere Listen zu verwalten
typedef struct {
    int readPointer;		//Index zum Lesen
    int writePointer;		//Index zum Schreiben
    Data *fifo;				//Platz fuer Speicherelemente, eigentlicher Buffer
    int size;				//Groesse des Buffers, d.h. Anzahl der Elemente
	int dataSize;			// number of chars in buffer
} ringbuffer_handler_t;

/****************************************************************************************************/

int data_size = 0;      // number of chars in buffer
int read_pointer = 0;   // indice number of last read char
int write_pointer = 0;  // indice number of last written char
int input;              // user input
char add;               // char to add

char buffer[BUFFER_SIZE];
/****************************************************************************************************/




// prototypes
int buffer_full(ringbuffer_handler_t *buffer);
int buffer_empty(ringbuffer_handler_t *buffer);
void push_char(Data data, ringbuffer_handler_t *buffer);
void pull_char(ringbuffer_handler_t *buffer);
ringbuffer_handler_t *createRingbuffer(int size);


/****************************************************************************************************/
//eine Funktion, um einen Ringbuffer anzulegen
//Parameter size: Groesse des Buffers (Anzahl der Elemente)
//Rückgabewert: Zeiger auf Ringbuffer-Handler
ringbuffer_handler_t *createRingbuffer(int size) {
	
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
/****************************************************************************************************/

/****************************************************************************************************/
int main(void) {
	
	int i;
	int j;
	
	printf("Circular Buffer Queue Implementation");
	
	//eine Variable fuer die Daten
    Data data;
    //ein Ringbuffer-Handler
    ringbuffer_handler_t *buffer;
    //eine Variable, um Ergebnisse von readFIFO() abzufragen
    int ergebnis;
	
    //einen Ringbuffer-Handler und damit auch einen Ringbuffer anlegen
    buffer = createRingbuffer(8);

	
	data.key=12;
    strcpy(data.name,"egon");
	
	// make sure there are no random chars in array, all spaces
	//for (i = 0; i < buffer->size; i++) buffer->fifo[i] = 0x20;
	
	printf("Fill Ringbuffer....\n");
	
	for (j=0; j<3; j++) {
		
		data.humidity.min=30.0;
		data.humidity.max=50.0;
		data.humidity.current=40.0;
		
		for (i=0; i<NumberOfTemperatureSensors; i++) {
			data.temperature[i].min=19.0
			data.temperature[i].max=21.0
			data.temperature[i].current=20.0
		}
		
		for (i=0; i<NumberOfECSensors; i++) {
			data.ecLevel[i].min=500;
			data.ecLevel[i].max=700;
			data.ecLevel[i].current=600;
		}
		push_char(data, buffer);
	}

	
	/*
	data.key=1;
    strcpy(data.name,"alpha");
	push_char(data, buffer);
	data.key=2;
    strcpy(data.name,"bravo");
	push_char(data, buffer);
	 */
	
	while (input != 4) {
		
		printf("\n    press 1 to push char");
		printf("\n    press 2 to pop char");	//pull_char
		printf("\n    press 3 to show queue");
		printf("\n    press 4 to exit\n");
		scanf("%d", &input);
		
		// push char
		if (input == 1) {
			
			// Create fake data
			data.humidity.min=30.0;
			data.humidity.max=50.0;
			data.humidity.current=40.0;
			
			for (i=0; i<NumberOfTemperatureSensors; i++) {
				data.temperature[i].min=19.0
				data.temperature[i].max=21.0
				data.temperature[i].current=20.0
			}
			
			for (i=0; i<NumberOfECSensors; i++) {
				data.ecLevel[i].min=500;
				data.ecLevel[i].max=700;
				data.ecLevel[i].current=600;
			}
			
			if (! buffer_full(buffer)) {
				push_char(data, buffer);
			} else {
				printf("\n!!! BUFFER IS FULL !!! ");
				
				// Pop "oldest" out
				pull_char(buffer);
				// Push new in
				push_char(data, buffer);
			}
				
		}
		// pull char
		else if (input == 2) {
			
			if (! buffer_empty(buffer))
				pull_char(buffer);
			else
				printf("\nBUFFER IS EMPTY!");
		}
		// display buffer info
		else if (input == 3) {
			
			printf("\n dataSize: %d readPointer: %d writePointer: %d",
				   buffer->dataSize, buffer->readPointer, buffer->writePointer);
			
			printf("\nQueue content:\n");
			for (i = 0; i < buffer->size; i++) {
				data = buffer->fifo[i];
				printf("[%s]", data.temperature[0].current);
			}
			
		}
		
		printf("\n----");
	}
	
	return 0;
}
/****************************************************************************************************/


/****************************************************************************************************/
// adds a char
void push_char(Data data, ringbuffer_handler_t *buffer) {
	
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
/****************************************************************************************************/


/****************************************************************************************************/
// returns 1 if buffer is full, 0 if buffer is not full, -1 if error
int buffer_full(ringbuffer_handler_t *buffer) {
	if (buffer) {
		return buffer->readPointer == buffer->writePointer && buffer->dataSize == buffer->size;
	} else
		return -1;
}
/****************************************************************************************************/


/****************************************************************************************************/
// returns 1 if buffer is empty, 0 if buffer is not empty, -1 if error
int buffer_empty(ringbuffer_handler_t *buffer) {
	if (buffer) {
		return buffer->readPointer == buffer->writePointer && buffer->dataSize == 0;
	} else
		return -1;
}
/****************************************************************************************************/


/****************************************************************************************************/
// pull char from queue
void pull_char(ringbuffer_handler_t *buffer) {
	if (++buffer->readPointer >= buffer->size) {
			buffer->readPointer = 0;
	}
	
	//printf("\nPopped char %c", buffer->fifo[buffer->readPointer]);
	printf("\n--> POPPED OUT DATA AT INDEX: %d", buffer->readPointer);
	
	// enter space on place of read char so we can see it is removed
	// buffer->fifo[buffer->readPointer]= 0x20;
	
	buffer->dataSize--;
}
/****************************************************************************************************/