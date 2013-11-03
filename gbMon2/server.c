//
//  server.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "server.h"


/*
 * The Connection Handler for each client
 *********************************************************************************
 */
void *connectionHandler(void *socket_desc) {
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *buffer[255];
    char * client_message[2000];
	
	clientIsConnected = true;
	setUpdateDisplay(true);
	
    //Send some messages to the client
    //strcmp(message,"Welcome to Raspberry Pi gbmon2\n");
    //write(sock , message , strlen(message));
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ){
        //end of string marker
        sprintf(buffer,"Received Command: *%s*", client_message);
        debugPrint(true, true, buffer, true, "SERVER");
		
		// usage
        if(strcmp(client_message, "usage") == 0) {
            debugPrint(true, false, "usage", false,"");
            write(sock,"Available Commands:\n   getVersion \t\t- returns version\n   getServerTime \t- returns current server time\0\n   getWifiStrength \t- returns current wifi signal strength\n   getTemperature \t- returns current temperature\n   getHumidity \t- returns current humidity\n   getFan \t- returns current fan status \n\n setFan \t- toggle fan On or Off\0",400);
        }
		
		// getServerTime
        if(strcmp(client_message, "getServerTime") == 0) {
            sprintf(buffer,"Sending Response: *%s*", getTime());
            debugPrint(true, true, buffer, true, "SERVER");
            write(sock,getTime(),30);
        }
		
		// getVersion
        if(strcmp(client_message, "getVersion") == 0) {
            sprintf(buffer,"Sending Response: *%s*", getVersion());
            debugPrint(true, true, buffer, true, "SERVER");
            write(sock,getVersion(),3);
        }
		
		// getWifiStrength
		if(strcmp(client_message, "getWifiStrength") == 0) {
            sprintf(buffer,"Sending Response: *%d*", getWifiStrength());
            debugPrint(true, true, buffer, true, "SERVER");
            write(sock,getWifiStrength(),3);
        }
		
		// getTemperature
		if(strcmp(client_message, "getTemperature") == 0) {
            sprintf(buffer,"Sending Response: *%s*", getTemperature());
            debugPrint(true, true, buffer, true, "SERVER");
            write(sock,getTemperature(),4);
        }
		
		// getHumidity
		if(strcmp(client_message, "getHumidity") == 0) {
            sprintf(buffer,"Sending Response: *%s*", getHumidity());
            debugPrint(true, true, buffer, true, "SERVER");
            write(sock,getHumidity(),4);
        }
		
		// getFan
		if(strcmp(client_message, "getFan") == 0) {
			sprintf(buffer,"Sending Response: *%s*", getFanAsString());
            debugPrint(true, true, buffer, true, "SERVER");
			write(sock,getFanAsString(),2);
        }
		
		// setFan
		if(strcmp(client_message, "setFan") == 0) {
            sprintf(buffer,"Sending Response: *%d*", setFan());
            debugPrint(true, true, buffer, true, "SERVER");
            write(sock,getFanAsString(),2);
        }
		
        /*
         if(strcmp(client_message, "getHumidity") == 0) {
         printf(">>> getHumidity \n");
         char tmp[4];
         sprintf(tmp,"%.1f\0",measure.humidity);
         //send(sock, &measure.humidity, sizeof(float),0);
         //write(sock,&measure.humidity, sizeof(float));
         write(sock,tmp,4);
         }
         if(strcmp(client_message, "getTemperature") == 0) {
         printf(">>> getTemperature \n");
         char tmp[4];
         sprintf(tmp,"%.1f\0",measure.temperature[0]);
         write(sock,tmp,4);
         }
         if(strcmp(client_message, "getAll") == 0) {
         if (debug==2) {
         printf(">>> getAll\n");
         }
         char tmp[255];
         sprintf(tmp,"%s\0",getAllJSON());
         write(sock,tmp,strlen(tmp));
         //n = write(sock,measure.humidity,6);
         }
         if(strcmp(client_message, "makePic") == 0) {
         printf(">>> makePic \n");
         char shellCommand [100];
         struct tm *t ;
         time_t tim ;
         char buf2[32];
         tim = time (NULL) ;
         t = localtime (&tim) ;
         sprintf (buf2,"%02d-%02d-%04d.jpg", t->tm_mday, t->tm_mon + 1, t->tm_year+1900);
         sprintf(shellCommand,"raspistill -o /home/pi/.gbmon/pics/%s",buf2);
         //printf("CAM command: %s",shellCommand);
         FILE * pp ;
         //char shellCommand[] = buf;
         pp = popen(shellCommand, "r");
         if (pp != NULL) {
         while (1) {
         char *line;
         char buf[1000];
         line = fgets(buf, sizeof buf, pp);
         if (line == NULL) break;
         if (line[0] == 'd') printf("%s", line); // line includes '\n'
         }
         pclose(pp);
         }
         sprintf(shellCommand,"sshpass -p 'alpine' scp /home/pi/.gbmon/pics/%s service@192.168.178.10:./Sites/MacServer/gbmon/pics/",buf2);
         pp = popen(shellCommand, "r");
         pclose(pp);
         write(sock,"Picture updated\0",20);
         }
         */
        //client_message[read_size] = '\0';
        //Send the message back to client
        //write(sock , client_message , strlen(client_message));
        //clear the message buffer
        read_size=1;
        memset(client_message, 0, 2000);
    }
    if(read_size == 0) {
        debugPrint(true, true, "Client disconnected", true, "SERVER");
        fflush(stdout);
		// DIsplay UPdate
		clientIsConnected = false;
		setUpdateDisplay(true);
		
    } else if(read_size == -1) {
        perror("recv failed");
    }
    return 0;
}


int serverMain(int portno){
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
	clientIsConnected = false;
	
    debugPrint(true, true, "Creating Socket ...", false, "SERVER");
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        debugPrint(false, true, "Could not create Socket", true, "ERROR");
    }
    debugPrint(false, false, "OK", true, "");
	
	
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( portno );
    
	
	//Bind
    debugPrint(true, true, "Starting Bind ...", false, "SERVER");
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        debugPrint(false, true, "Bind failed to start", true, "ERROR");
        return 1;
    }
    debugPrint(false, false, "OK", true, "SERVER");
    
	
	//Listen
    listen(socket_desc , 3);
    
	//Accept and incoming connection
    //puts("Waiting for incoming connections...");
    //c = sizeof(struct sockaddr_in);
    //Accept and incoming connection
    debugPrint(true, true, "Waiting for incoming connections ...", false, "SERVER");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;
    debugPrint(false, false, "OK", true, "SERVER");
    
	
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ) {

        debugPrint(true, true, "Connection accepted", true, "SERVER");
        debugPrint(true, true, "Assigning Handler ...", false, "SERVER");
        
		if( pthread_create( &thread_id , NULL ,  connectionHandler , (void*) &client_sock) < 0) {
            debugPrint(false, true, "Could not create thread", true, "ERROR");
            return 1;
        }
        debugPrint(false, false, "OK", true, "SERVER");
		
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
    }
	
    if (client_sock < 0) {
        debugPrint(false, true, "Could not create thread", true, "ERROR");
        return 1;
    }
	
    pthread_join( thread_id , NULL);
    return 0;
}
