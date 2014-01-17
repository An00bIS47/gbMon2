//
//  testmdns.c
//  gbMon2
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//



#ifdef _WIN32
#include <winsock2.h>
#include <in6addr.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include "mdns.h"
#include "mdnsd.h"

int main(int argc, char *argv[]) {
	// create host entries
	char *hostname = "RaspberryPi.local";
	
	struct mdnsd *svr = mdnsd_start();
	if (svr == NULL) {
		printf("mdnsd_start() error\n");
		return 1;
	}
	
	printf("mdnsd_start OK. press ENTER to add hostname & service\n");
	getchar();
	
	mdnsd_set_hostname(svr, hostname, inet_addr("192.168.178.63"));
	
	struct rr_entry *a2_e = NULL;
	a2_e = rr_create_a(create_nlabel(hostname), inet_addr("192.168.178.63"));
	mdnsd_add_rr(svr, a2_e);
	
	struct rr_entry *aaaa_e = NULL;
	
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_flags = AI_NUMERICHOST;
	struct addrinfo* results;
	getaddrinfo(
				"fe80::5ab0:35ff:fe7f:cf27",
				NULL,
				&hints,
				&results);
	struct sockaddr_in6* addr = (struct sockaddr_in6*)results->ai_addr;
	struct in6_addr v6addr = addr->sin6_addr;
	freeaddrinfo(results);
	
	aaaa_e = rr_create_aaaa(create_nlabel(hostname), &v6addr);
	
	mdnsd_add_rr(svr, aaaa_e);
	
	const char *txt[] = {
		"path=/mywebsite",
		NULL
	};
	struct mdns_service *svc = mdnsd_register_svc(svr, "gbMon2",
												  "_gbmon._tcp.local", 1000, NULL, txt);
	mdns_service_destroy(svc);
	
	printf("added service and hostname. press ENTER to exit\n");
	getchar();
	
	mdnsd_stop(svr);
	
	return 0;
}
