//
//  zlogTest.c
//  gbMon2
//
//  Created by michael on 31.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include <stdio.h>
#include "zlog.h"

int main(int argc, char** argv) {
	int rc;
	zlog_category_t *c;
	
	rc = zlog_init("/home/pi/gbmon2/gbMon2/zlog.conf");
	if (rc) {
		printf("init failed\n");
		return -1;
	}
	
	c = zlog_get_category("my_cat");
	if (!c) {
		printf("get cat fail\n");
		zlog_fini();
		return -2;
	}
	
	zlog_info(c, "hello, zlog");
	zlog_debug(c, "%d", 12);
	/*
	 zlog_fatal(cat, format, ...)
	 
	 zlog_error(cat, format, ...)
	 
	 zlog_warn(cat, format, ...)
	 
	 zlog_notice(cat, format, ...)
	 
	 zlog_info(cat, format, ...)
	 
	 zlog_debug(cat, format, ...)
	 */
	zlog_fini();
	
	return 0;
}