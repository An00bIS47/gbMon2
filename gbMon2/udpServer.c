//
//  udpServer.c
//  gbMon2
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "udpServer.h"

#define RESPONSE_SIZE 255

/*
 * Returns information as json
 *********************************************************************************
 */
char* getAllJSON(){
    char buffer[RESPONSE_SIZE];
    sem_wait(&semaLockInfo);
    
    sprintf(buffer,"{\"Humidity\":{\"min\":\"%.1f\",\"max\":\"%.1f\",\"current\":\"%.1f\"},\"Temperature\":{\"min0\":\"%.1f\",\"max0\":\"%.1f\",\"current0\":\"%.1f\"},\"LightValue\":\"%i\",\"ec Level\":{\"min0\":\"%.1f\",\"max0\":\"%.1f\",\"current0\":\"%.1f\"},\"Fan\":\"%s\"}",
            data.humidity.min, data.humidity.max, data.humidity.current, data.temperature[0].min, data.temperature[0].max, data.temperature[0].current, data.lightValue, data.ecLevel[0].min, data.ecLevel[0].max, data.ecLevel[0].current, getFanAsString());
    
	//printf("JSON: %s\n", buffer);
    sem_post(&semaLockInfo);
    return buffer;
}

void* serverMain(int portno){
	
	int sd, rc, n, cliLen, flags;
	int buffersize = 255;
	struct sockaddr_in cliAddr, servAddr;
	char msg[MAX_MSG];
	
	//char *argv[0] = "udpServer";
	
	/* socket creation */
	sd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sd<0) {
		printf("%s: cannot open socket \n","udpServer");
		exit(1);
	}
	
	/* bind local server port */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(portno);
	rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
	if(rc<0) {
		printf("%s: cannot bind port number %d \n",
			   "udpServer", portno);
		exit(1);
	}
	
	printf("%s: waiting for data on port UDP %u\n",
		   "udpServer",portno);
	
	/* BEGIN jcs 3/30/05 */
	
	flags = 0;
	
	/* END jcs 3/30/05 */
	
	int timecounter=0;
	/* server infinite loop */
	while(1) {
		
		/* init buffer */
		memset(msg,0x0,MAX_MSG);
		
		/* receive message */
		cliLen = sizeof(cliAddr);
		n = recvfrom(sd, msg, MAX_MSG, flags,
					 (struct sockaddr *) &cliAddr, &cliLen);
		
		sem_wait(&semaLockInfo);
		if (clientIsConnected == false){
			clientIsConnected = true;
			setUpdateDisplay(true);
		}
		sem_post(&semaLockInfo);       // up semaphore
		
		if(n<0) {
			printf("%s: cannot receive data \n","udpServer");
			continue;
		}
		
		// usage
        if((strcmp(msg, "usage") == 0) || (strcmp(msg, "help") == 0)){
            debugPrint(true, false, "usage", false,"");
			sendto(sd,"Available Commands:\n   getVersion \t\t- returns version\n   getServerTime \t- returns current server time\0\n   getWifiStrength \t- returns current wifi signal strength\n   getTemperature \t- returns current temperature\n   getHumidity \t- returns current humidity\n   getFan \t- returns current fan status \n   resetTemperature \t- reset min and max temperature\n   reset Humidity \t- reset min and max humidity\n   getLightValue \t- returns current light value\n   getAllJSON \t- returns all infos in JSON format \n\n setFan \t- toggle fan On or Off\n   makePic \t- take picture with pi Cam\0",450,flags,(struct sockaddr *)&cliAddr,cliLen);
        }

		if(strcmp(msg, "getAllJSON") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", getAllJSON());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			sendto(sd,getAllJSON(),strlen(getAllJSON()),flags,(struct sockaddr *)&cliAddr,cliLen);
		}
		
		
		if(strcmp(msg, "getServerTime") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", getTime());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			sendto(sd,getTime(),20,flags,(struct sockaddr *)&cliAddr,cliLen);
            //write(sock,getTime(),30);
        }
		
		// getVersion
        if(strcmp(msg, "getVersion") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", getVersion());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			sendto(sd,getVersion(),3,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		
		// getWifiStrength
		if(strcmp(msg, "getWifiStrength") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%d*", getWifiStrength());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			sendto(sd,getWifiStrength(),4,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		// getTemperature
		if(strcmp(msg, "getTemperature") == 0) {
			sem_wait(&semaLockInfo);
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%.1f*", data.temperature[0].current);
            debugPrint(true, true, buffer, true, "SERVER");
            //write(sock,getTemperature(),4);
			sprintf(buffer,"%.1f",data.temperature[0].current);
			sendto(sd,buffer,5,flags,(struct sockaddr *)&cliAddr,cliLen);
			free(buffer);
			sem_post(&semaLockInfo);
        }
		
		// getHumidity
		if(strcmp(msg, "getHumidity") == 0) {
			sem_wait(&semaLockInfo);
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%.1f*", data.humidity.current);
            debugPrint(true, true, buffer, true, "SERVER");

            //write(sock,getHumidity(),4);
			sprintf(buffer,"%.1f",data.humidity.current);
			sendto(sd,buffer,5,flags,(struct sockaddr *)&cliAddr,cliLen);
			free(buffer);
			sem_post(&semaLockInfo);
        }
		
		// getFan
		if(strcmp(msg, "getFan") == 0) {
			char * buffer = (char*)malloc(buffersize);
			sprintf(buffer,"Sending Response: *%s*", getFanAsString());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			//write(sock,getFanAsString(),2);
			sendto(sd,getFanAsString(),2,flags,(struct sockaddr *)&cliAddr,cliLen);
        }

		// getLightValue
		if(strcmp(msg, "getLightValue") == 0) {
			char * buffer = (char*)malloc(buffersize);
			sprintf(buffer,"Sending Response: *%s*", getLightValueAsString());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			//write(sock,getFanAsString(),2);
			sendto(sd,getLightValueAsString(),4,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		// setFan
		if(strcmp(msg, "setFan") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%d*", setFan());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
            //write(sock,getFanAsString(),2);
			sendto(sd,getFanAsString(),2,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		// makePic
		if(strcmp(msg, "makePic") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", "makePic");
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			useCam();
            //write(sock,getFanAsString(),2);
			sendto(sd,"makePic",8,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		// resetTemperature
		if(strcmp(msg, "resetTemperature") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", "resetTemperature");
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			resetTemperature();
            //write(sock,getFanAsString(),2);
			sendto(sd,"resetTemperature",8,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		// resetHumidity
		if(strcmp(msg, "resetHumidity") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", "resetHumidity");
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			resetHumidity();
            //write(sock,getFanAsString(),2);
			sendto(sd,"resetHumidity",8,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		/* print received message */
		printf("%s: from %s:UDP%u : %s \n",
			   "udpServer",inet_ntoa(cliAddr.sin_addr),
			   ntohs(cliAddr.sin_port),msg);
		
		/* BEGIN jcs 3/30/05 */
		/*
		sleep(1);
		sendto(sd,msg,n,flags,(struct sockaddr *)&cliAddr,cliLen);
		*/
		/* END jcs 3/30/05 */
		
		timecounter++;
		// Just a small timer, for display update for client connection icon
		if (timecounter==100){
			sem_wait(&semaLockInfo);
			clientIsConnected = false;
			sem_post(&semaLockInfo);       // up semaphore
			setUpdateDisplay(true);
			timecounter=0;
		}
		
	}/* end of server infinite loop */
	//pthread_join( thread_id , NULL);
	return 0;
	
}