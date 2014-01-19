//
//  avahi.h
//  gbMon2
//
//  Created by michael on 19.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#ifndef gbMon2_avahi_h
#define gbMon2_avahi_h

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>

#include <avahi-common/alternative.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

#include "debug.h"

static AvahiEntryGroup *group;
static AvahiSimplePoll *simple_poll;
static char *name;

static void create_services(AvahiClient *c);

static void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata);
static void create_services(AvahiClient *c);
static void modify_callback(AVAHI_GCC_UNUSED AvahiTimeout *e, void *userdata);
int avahiMain(AVAHI_GCC_UNUSED int argc, AVAHI_GCC_UNUSED char*argv[]);

#endif
