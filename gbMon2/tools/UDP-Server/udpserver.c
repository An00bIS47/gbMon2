//
//  udpserver.c
//  gbMon2
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */

#define LOCAL_SERVER_PORT 1500
#define MAX_MSG 100

float		appVersion              =   0.1;

char* getVersion(){
    char *buf = (char *) malloc(4 * sizeof(char));
    sprintf (buf, "%.1f", appVersion) ;
	buf[4]="\0";
    return buf;
}

int main(int argc, char *argv[]) {
	
	int sd, rc, n, cliLen, flags;
	struct sockaddr_in cliAddr, servAddr;
	char msg[MAX_MSG];
	
	/* socket creation */
	sd=socket(AF_INET, SOCK_DGRAM, 0);
	if(sd<0) {
		printf("%s: cannot open socket \n",argv[0]);
		exit(1);
	}
	
	/* bind local server port */
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(LOCAL_SERVER_PORT);
	rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
	if(rc<0) {
		printf("%s: cannot bind port number %d \n",
			   argv[0], LOCAL_SERVER_PORT);
		exit(1);
	}
	
	printf("%s: waiting for data on port UDP %u\n",
		   argv[0],LOCAL_SERVER_PORT);
	
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
			printf("%s: cannot receive data \n",argv[0]);
			continue;
		}
		
		if (strcmp("getTemperature", msg)==0){
			//sleep(1);
			sendto(sd,"27.2",5,flags,(struct sockaddr *)&cliAddr,cliLen);
		}
		
		if(strcmp(msg, "getServerTime") == 0) {
            //sprintf(buffer,"Sending Response: *%s*", getTime());
            //debugPrint(true, true, buffer, true, "SERVER");
			//sendto(sd,getTime(),30,flags,(struct sockaddr *)&cliAddr,cliLen);
            //write(sock,getTime(),30);
        }
		
		// getVersion
        if(strcmp(msg, "getVersion") == 0) {
            //sprintf(buffer,"Sending Response: *%s*", getVersion());
            //debugPrint(true, true, buffer, true, "SERVER");
			sendto(sd,getVersion(),3,flags,(struct sockaddr *)&cliAddr,cliLen);
        }
		
		/* print received message */
		printf("%s: from %s:UDP%u : %s \n",
			   argv[0],inet_ntoa(cliAddr.sin_addr),
			   ntohs(cliAddr.sin_port),msg);
		
		/* BEGIN jcs 3/30/05 */
		
		sleep(1);
		sendto(sd,msg,n,flags,(struct sockaddr *)&cliAddr,cliLen);
		
		/* END jcs 3/30/05 */
		
	}/* end of server infinite loop */
	
	return 0;
	
}