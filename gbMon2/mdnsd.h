//
//  msdnsd.h
//  gbMon2
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbMon2_mdnsd_h
#define gbMon2_mdnsd_h

#include <stdint.h>

struct mdnsd;
struct mdns_service;

// starts a MDNS responder instance
// returns NULL if unsuccessful
struct mdnsd *mdnsd_start();

// stops the given MDNS responder instance
void mdnsd_stop(struct mdnsd *s);

// sets the hostname for the given MDNS responder instance
void mdnsd_set_hostname(struct mdnsd *svr, const char *hostname, uint32_t ip);

// adds an additional RR
void mdnsd_add_rr(struct mdnsd *svr, struct rr_entry *rr);

// registers a service with the MDNS responder instance
struct mdns_service *mdnsd_register_svc(struct mdnsd *svr, const char *instance_name,
										const char *type, uint16_t port, const char *hostname, const char *txt[]);

// destroys the mdns_service struct returned by mdnsd_register_svc()
void mdns_service_destroy(struct mdns_service *srv);

#endif
