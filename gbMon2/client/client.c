//
//  client.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr

int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    
    
    
    printf("Welcome to Raspberry Pi gbmon Client \n");
    
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server.sin_addr.s_addr = inet_addr("192.168.178.20");
    server.sin_family = AF_INET;
    server.sin_port = htons( 1000 );
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    
    puts("Connected\n");
    
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        
        // exit
        if(strcmp(message, "exit") == 0) {
            close(sock);
            return 0;
        }
        
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        
        printf("Server Response: *%s* \n",server_reply);
        //puts("Server reply :");
        //puts(server_reply);
    }
    
    close(sock);
    return 0;
}