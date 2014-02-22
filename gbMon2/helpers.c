//
//  helpers.c
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "helpers.h"


char* getPiSerial(){
	FILE *f = fopen("/proc/cpuinfo", "r");
	if (!f) {
		return 1;
	}
	
	char serial_string[16 + 1];
	char line[256];
	
	//int serial;
	while (fgets(line, 256, f)) {
		if (strncmp(line, "Serial", 6) == 0) {
			
			strcpy(serial_string, strchr(line, ':') + 2);
			
			printf("Serial::: %s\n",serial_string);
			//serial = atoi(serial_string);
		}
	}
	
	fclose(f);
	
	return serial_string;
}

/*
 * Substring
 *********************************************************************************
 */
char* substring(const char* str, size_t begin, size_t len){
    if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
        return 0;
    
    return strndup(str + begin, len);
}

unsigned long long getUptime(){
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	//printf("tv_sec=%llu tv_nsec=%llu\n",(unsigned long long)t.tv_sec, (unsigned long long)t.tv_nsec);
	
	return (unsigned long long)t.tv_sec;
}

int getWifiStrength() {
	FILE *in;
	extern FILE *popen();
	char buff[512];
	
	
	//("/sbin/ifconfig | grep 'inet addr' | grep -v 127.0.0.1 | awk '{print $2}' | sed 's/[^0-9\\.]//g'", "r");
	if(!(in = popen("iwconfig wlan0 | grep Quality | cut -d'=' -f2 | cut -d' ' -f1 | cut -d'/' -f1", "r"))){
		printf("Error getting Wifi Signal Strength\n");
		return 0;
	}
	
	while(fgets(buff, sizeof(buff), in)!=NULL){
		//printf("%s", buff);
	}
	pclose(in);
	
	return strtol(buff,NULL,10);
	//return 1;
}


byte reverseByte(byte a) {
    int i;
    byte b = 0;
	
    for ( i = 0 ; i < 8 ; i ++)
    {
        b <<= 1;
        b |=  ( (a & (1 << i)) >> i);
    }
    return b;
}

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

char* getLightValueAsString(){
    char *buf = (char *) malloc(4 * sizeof(char));
    sprintf (buf, "%d", getLightValue()) ;
	buf[4]="\0";
    return buf;
}

char* getVersion(){
    char *buf = (char *) malloc(4 * sizeof(char));
    sprintf (buf, "%.1f", appVersion) ;
	buf[4]="\0";
    return buf;
}

char* getTimeShort(){
    //static char *buf [32] ;
    char *buf = (char *) malloc(32 * sizeof(char));
    struct tm *t ;
    time_t tim ;
    tim = time (NULL) ;
    t = localtime (&tim) ;
    sprintf (buf, "%02d:%02d", t->tm_hour, t->tm_min) ;
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
    int s;
	//int family;
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

