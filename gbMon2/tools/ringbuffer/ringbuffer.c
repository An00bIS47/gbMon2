/* CIRCULAR BUFFER QUEUE */

#include <stdio.h>

#define BUFFER_SIZE 8

int data_size = 0;      // number of chars in buffer
int read_pointer = 0;   // indice number of last read char
int write_pointer = 0;  // indice number of last written char
int input;              // user input
char add;               // char to add

char buffer[BUFFER_SIZE];

// prototypes
int buffer_full(void);
int buffer_empty(void);
void push_char(char c);
void pull_char(void);

int main(void) {
	
	int i;
	
	printf("Circular Buffer Queue Implementation");
	
	// make sure there are no random chars in array, all spaces
	for (i = 0; i < BUFFER_SIZE; i++) buffer[i] = 0x20;
	
	while (input != 4) {
		
		printf("\n    press 1 to push char");
		printf("\n    press 2 to pop char");
		printf("\n    press 3 to show queue");
		printf("\n    press 4 to exit\n");
		scanf("%d", &input);
		
		// push char
		if (input == 1) {
			
			printf("\nEnter char: ");
			scanf("%c", &add);
			scanf("%c", &add); // twice otherwise it will get the last enter as input
			
			if (! buffer_full())
				push_char(add);
			else
				printf("\nBUFFER IS FULL!");
			
		}
		// pull char
		else if (input == 2) {
			
			if (! buffer_empty())
				pull_char();
			else
				printf("\nBUFFER IS EMPTY!");
		}
		// display buffer info
		else if (input == 3) {
			
			printf("\n data_size: %d read_pointer: %d write_counter: %d",
				   data_size, read_pointer, write_pointer);
			
			printf("\nQueue content:\n");
			for (i = 0; i < BUFFER_SIZE; i++) printf("[%c]", buffer[i]);
			
		}
		
		printf("\n----");
	}
	
	return 0;
}

// adds a char
void push_char(char c) {
    // increase write_pointer, check if at end of array
    if (++write_pointer >= BUFFER_SIZE) write_pointer = 0;
	
    buffer[write_pointer] = c;
    data_size++;
}

// returns 1 if buffer is full, 0 if buffer is not full
int buffer_full(void) {
	return read_pointer == write_pointer &&
	data_size == BUFFER_SIZE;
}

// returns 1 if buffer is empty, 0 if buffer is not empty
int buffer_empty(void) {
	return read_pointer == write_pointer &&
	data_size == 0;
}

// pull char from queue
void pull_char(void) {
	if (++read_pointer >= BUFFER_SIZE) read_pointer = 0;
	
	printf("\nPopped char %c", buffer[read_pointer]);
	
	// enter space on place of read char so we can see it is removed
	buffer[read_pointer] = 0x20;
	data_size--;
}