//
//  avahi.c
//  gbMon2
//
//  Created by michael on 19.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "avahi.h"

static AvahiEntryGroup *group = NULL;
static AvahiSimplePoll *simple_poll = NULL;
static char *name = NULL;

static void create_services(AvahiClient *c);

static void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata) {
    assert(g == group || group == NULL);
    group = g;
	
    /* Called whenever the entry group state changes */
	
    switch (state) {
        case AVAHI_ENTRY_GROUP_ESTABLISHED :
            /* The entry group has been established successfully */
			debugPrint(false, false, "OK", true, "AVAHI");
            fprintf(stderr, "Service '%s' successfully established.\n", name);
            break;
			
        case AVAHI_ENTRY_GROUP_COLLISION : {
            char *n;
			
            /* A service name collision with a remote service
             * happened. Let's pick a new name */
            n = avahi_alternative_service_name(name);
            avahi_free(name);
            name = n;
			
            fprintf(stderr, "Service name collision, renaming service to '%s'\n", name);
			
            /* And recreate the services */
            create_services(avahi_entry_group_get_client(g));
            break;
        }
			
        case AVAHI_ENTRY_GROUP_FAILURE :
			
            fprintf(stderr, "Entry group failure: %s\n", avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(g))));
			
            /* Some kind of failure happened while we were registering our services */
            avahi_simple_poll_quit(simple_poll);
            break;
			
        case AVAHI_ENTRY_GROUP_UNCOMMITED:
        case AVAHI_ENTRY_GROUP_REGISTERING:
            ;
    }
}

static void create_services(AvahiClient *c) {
    char *n;
	char *r[128];			// Can be removed or replaced
    int ret;
    assert(c);
	
	char *txt[128];


    /* If this is the first time we're called, let's create a new
     * entry group if necessary */
	
    if (!group)
        if (!(group = avahi_entry_group_new(c, entry_group_callback, NULL))) {
            fprintf(stderr, "avahi_entry_group_new() failed: %s\n", avahi_strerror(avahi_client_errno(c)));
            goto fail;
        }
	
    /* If the group is empty (either because it was just created, or
     * because it was reset previously, add our entries.  */
	
    if (avahi_entry_group_is_empty(group)) {
        fprintf(stderr, "Adding service '%s'\n", name);
		
        /* Create some random TXT data */
        snprintf(r, sizeof(r), "random=%i", rand());		// Can be removed or replaced
		sprintf(txt, "version = %1.1f",appVersion);

		
        /* We will now add two services and one subtype to the entry
         * group. The two services have the same name, but differ in
         * the service type (IPP vs. BSD LPR). Only services with the
         * same name should be put in the same entry group. */
		
        /* Add the service for IPP */
        if ((ret = avahi_entry_group_add_service(group, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 0, name, "_gbmon._tcp", NULL, NULL, 1000, txt, r, NULL)) < 0) {
			
            if (ret == AVAHI_ERR_COLLISION)
                goto collision;
			
            fprintf(stderr, "Failed to add _ipp._tcp service: %s\n", avahi_strerror(ret));
            goto fail;
        }
		
        /* Tell the server to register the service */
        if ((ret = avahi_entry_group_commit(group)) < 0) {
            fprintf(stderr, "Failed to commit entry group: %s\n", avahi_strerror(ret));
            goto fail;
        }
    }
	
    return;
	
collision:
	
    /* A service name collision with a local service happened. Let's
     * pick a new name */
    n = avahi_alternative_service_name(name);
    avahi_free(name);
    name = n;
	
    fprintf(stderr, "Service name collision, renaming service to '%s'\n", name);
	
    avahi_entry_group_reset(group);
	
    create_services(c);
    return;
	
fail:
    avahi_simple_poll_quit(simple_poll);
}

static void client_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata) {
    assert(c);
	
    /* Called whenever the client or server state changes */
	
    switch (state) {
        case AVAHI_CLIENT_S_RUNNING:
			
            /* The server has startup successfully and registered its host
             * name on the network, so it's time to create our services */
            create_services(c);
            break;
			
        case AVAHI_CLIENT_FAILURE:
			
            fprintf(stderr, "Client failure: %s\n", avahi_strerror(avahi_client_errno(c)));
            avahi_simple_poll_quit(simple_poll);
			
            break;
			
        case AVAHI_CLIENT_S_COLLISION:
			
            /* Let's drop our registered services. When the server is back
             * in AVAHI_SERVER_RUNNING state we will register them
             * again with the new host name. */
			
        case AVAHI_CLIENT_S_REGISTERING:
			
            /* The server records are now being established. This
             * might be caused by a host name change. We need to wait
             * for our own records to register until the host name is
             * properly esatblished. */
			
            if (group)
                avahi_entry_group_reset(group);
			
            break;
			
        case AVAHI_CLIENT_CONNECTING:
            ;
    }
}

static void modify_callback(AVAHI_GCC_UNUSED AvahiTimeout *e, void *userdata) {
    AvahiClient *client = userdata;
	
    fprintf(stderr, "Doing some weird modification\n");
	
    avahi_free(name);
    name = avahi_strdup("Modified gbMon2");
	
    /* If the server is currently running, we need to remove our
     * service and create it anew */
    if (avahi_client_get_state(client) == AVAHI_CLIENT_S_RUNNING) {
		
        /* Remove the old services */
        if (group)
            avahi_entry_group_reset(group);
		
        /* And create them again with the new name */
        create_services(client);
    }
}

int avahiMain(AVAHI_GCC_UNUSED int argc, AVAHI_GCC_UNUSED char*argv[]) {
    AvahiClient *client = NULL;
    int error;
    int ret = 1;
    //struct timeval tv;		// Not needed due no modifications made by avahi service
	// Comes from sample code
	
    /* Allocate main loop object */
    if (!(simple_poll = avahi_simple_poll_new())) {
        fprintf(stderr, "Failed to create simple poll object.\n");
        goto fail;
    }
	
    name = avahi_strdup("gbMon2");
	
    /* Allocate a new client */
    client = avahi_client_new(avahi_simple_poll_get(simple_poll), 0, client_callback, NULL, &error);
	
    /* Check wether creating the client object succeeded */
    if (!client) {
        fprintf(stderr, "Failed to create client: %s\n", avahi_strerror(error));
        goto fail;
    }
	
    /* After 10s do some weird modification to the service */
    /*
	 avahi_simple_poll_get(simple_poll)->timeout_new(
	 avahi_simple_poll_get(simple_poll),
	 avahi_elapse_time(&tv, 1000*10, 0),
	 modify_callback,
	 client);
	 */
    /* Run the main loop */
	

    avahi_simple_poll_loop(simple_poll);
	
    ret = 0;
	
fail:
	
    /* Cleanup things */
	
    if (client)
        avahi_client_free(client);
	
    if (simple_poll)
        avahi_simple_poll_free(simple_poll);
	
    avahi_free(name);
	
    return ret;
}

