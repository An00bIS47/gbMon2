//
//  udpServer.c
//  gbMon2
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "udpServer.h"


/*
 * Returns information as json
 *********************************************************************************
 */
char* getAllJSON(){
    char buffer[111];
    sprintf(buffer,"{\"Humidity\":\"%.1f\",\"Temperature\":{\"Temperature1\":\"%.1f\",\"Temperature2\":\"%.1f\",\"Temperature3\":\"%.1f\"}, \"Fan\":\"%s\"}",current.humidity,current.temperature[0],current.temperature[1],current.temperature[2],getFanAsString());
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
	
	/* server infinite loop */
	while(1) {
		
		/* init buffer */
		memset(msg,0x0,MAX_MSG);
		
		/* receive message */
		cliLen = sizeof(cliAddr);
		n = recvfrom(sd, msg, MAX_MSG, flags,
					 (struct sockaddr *) &cliAddr, &cliLen);
		
		if(n<0) {
			printf("%s: cannot receive data \n","udpServer");
			continue;
		}
		
		// usage
        if((strcmp(msg, "usage") == 0) || (strcmp(msg, "help") == 0)){
            debugPrint(true, false, "usage", false,"");
			sendto(sd,"Available Commands:\n   getVersion \t\t- returns version\n   getServerTime \t- returns current server time\0\n   getWifiStrength \t- returns current wifi signal strength\n   getTemperature \t- returns current temperature\n   getHumidity \t- returns current humidity\n   getFan \t- returns current fan status \n   getAllJSON \t- returns all infos in JSON format \n\n setFan \t- toggle fan On or Off\0",400,flags,(struct sockaddr *)&cliAddr,cliLen);
        }

		if(strcmp(msg, "getAllJSON") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", getAllJSON());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
			sendto(sd,getAllJSON(),111,flags,(struct sockaddr *)&cliAddr,cliLen);
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
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", getTemperature());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
            //write(sock,getTemperature(),4);
			sendto(sd,getTemperature(),5,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		// getHumidity
		if(strcmp(msg, "getHumidity") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%s*", getHumidity());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
            //write(sock,getHumidity(),4);
			sendto(sd,getHumidity(),5,flags,(struct sockaddr *)&cliAddr,cliLen);
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
		
		// setFan
		if(strcmp(msg, "setFan") == 0) {
			char * buffer = (char*)malloc(buffersize);
            sprintf(buffer,"Sending Response: *%d*", setFan());
            debugPrint(true, true, buffer, true, "SERVER");
			free(buffer);
            //write(sock,getFanAsString(),2);
			sendto(sd,getFanAsString(),2,flags,(struct sockaddr *)&cliAddr,cliLen);
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
		
	}/* end of server infinite loop */
	//pthread_join( thread_id , NULL);
	return 0;
	
}