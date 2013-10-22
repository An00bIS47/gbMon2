//
//  helpers.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "helpers.h"


const char *printBinary(int x) {
    static char b[9];
    b[0] = '\0';
	
    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
	
    return b;
}

char* getVersion(){
    char *buf = (char *) malloc(4 * sizeof(char));
    sprintf (buf, "%.1f", appVersion) ;
	buf[4]="\0";
    return buf;
}


char* getTime(){
    //static char *buf [32] ;
    char *buf = (char *) malloc(32 * sizeof(char));
    struct tm *t ;
    time_t tim ;
    tim = time (NULL) ;
    t = localtime (&tim) ;
    sprintf (buf, "%02d/%02d/%04d %02d:%02d:%02d", t->tm_mday, t->tm_mon + 1, t->tm_year+1900, t->tm_hour, t->tm_min, t->tm_sec) ;
    return buf;
}



/*
 * interface wlan0, eth0
 */
/**
	<#Description#>
	@param interface <#interface description#>
	@returns <#return value description#>
 */
char* getIP(char *interface) {
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];
    char *buffer[16];
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;
        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        if((strcmp(ifa->ifa_name,interface)==0)&&(ifa->ifa_addr->sa_family==AF_INET))
        {
            if (s != 0)
            {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            //printf("\tInterface : <%s>\n",ifa->ifa_name );
            //printf("\t  Address : <%s>\n", host);
            strcpy(buffer,host);
        }
    }
    freeifaddrs(ifaddr);
    return buffer;
}

