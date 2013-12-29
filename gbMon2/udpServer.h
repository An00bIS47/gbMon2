//
//  udpServer.h
//  gbMon2
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_udpServer_h
#define gbMon2_udpServer_h

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */
#include <pthread.h>
#include <stdbool.h>

#include "main.h"
#include "rc434.h"

#define MAX_MSG 100

char* getAllJSON();
void* serverMain(int portno);

#endif
