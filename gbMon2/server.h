//
//  server.h
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbmon2_server_h
#define gbmon2_server_h

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

//#include "main.h"

/*
 * Deklarationen
 *********************************************************************************
 */

/**
	<#Description#>
	@param socket_desc <#socket_desc description#>
 */
void *connectionHandler(void *socket_desc);

/**
	<#Description#>
	@param portno <#portno description#>
	@returns <#return value description#>
 */
int serverMain(int portno);


#endif
