#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <wiringPi.h>
#include <wiringPiSPI.h>


const char *printBinary(int x)
{
    static char b[9];
    b[0] = '\0';
	
    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
	
    return b;
}


int main (void) {
    
    int i;
    printf("Initialising wiringPi SPI ...");
    if (wiringPiSPISetup(0, 5000000) == -1) {
        return -1;
    }
    printf(" OK\n");
    
/*
	char c[] = {0x48};
    //unsigned char c[] = {0x48, 0x45, 0x4C, 0x4C, 0x4F, 0x20, 0x57, 0x4F, 0x52, 0x4C, 0x44, 0x0A};
    //unsigned char c[] = {1, 2, 0, 0x4C, 0x4F, 0x20, 0x57, 0x4F, 0x52, 0x4C, 0x44, 0x0A};
    printf("Command to send: \n");
    for(i = 0; i < sizeof(c); i++) {
        printf("0x%02x ",c[i]);
    }
    
    printf("\n");
*/
    int k=0;
    for (k=0;k<1;k++) {
        //uint8_t c = 0x00;
        
        
        //        unsigned char c[] = {
        //            0x48, 0x0A, 0x45, 0x0A, 0x4C, 0x0A, 0x4F, 0x0A,
        //            0x0A
        //        };
        //
        
        
        //c[] = {0x48, 0x45, 0x4C, 0x4C, 0x4F, 0x20, 0x57, 0x4F, 0x52, 0x4C, 0x44, 0x0A};
		/*
		char buff[2];
		
		buff[0] = 0x48;
		buff[1] = 0x00;
		*/
		unsigned char buff[5] = {1,'a',3,4,5};
		
		printf("Buffer: \n");
		//printf("=================================================\n");
		printf("  Char    Dec    Hex       Binary \n");
		printf(" ------  -----  -----    ----------\n");

		for (i=0; i<sizeof(buff); i++) {
			printf("   %1c \t %3d \t 0x%02x \t 0b%s\n",(char) buff[i], buff[i], buff[i],printBinary(buff[i]));
		}
        //printf("=================================================\n");
		
		//buff[0] = 'H';
		//buff[1] = 0x4C;
		//buff[2] = 0x4C;
		//buff[3] = 0x4F;
		//buff[4] = 0x45;
		//buff[5] = 0x00;

		for (i=0; i<sizeof(buff); i++) {
			//printf(" BUFF%d: %1c : 0x%X - size: %lu\n", i, buff[i] ,buff[i], sizeof(buff[i]));
		}
		//printf(" BUFF GESAMT: *%s* - size %lu \n", buff, sizeof(buff));
		
		
        //printf("%d : Sending command (buffer): %s\n", k, buff);
        //wiringPiSPIDataRW(0, &buff, sizeof(buff));
		
		printf("Sending buffer ...");
		digitalWrite(12,buff);
		printf(" OK\n");
        //printf("%d : After sending: (buffer): %s\n", k, buff);

        /*
        for(i = 0; i < sizeof(c); i++) {
            char buff = c[i];
            printf("0x%02x ",c[i]);
            wiringPiSPIDataRW(0, &buff, 1);
            //wiringPiSPIDataRW(0, 0x20, 1);
        }
        */
        //wiringPiSPIDataRW(0, &c, sizeof(c));
        delay(1000);
    }
    
}