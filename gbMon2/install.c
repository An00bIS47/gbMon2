//
//  install.c
//  gbMon2
//
//  Created by michael on 06.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include "install.h"


int checkDirExist(char* path) {
	struct stat s;
	int err = stat(path, &s);
	if(-1 == err) {
		if(ENOENT == errno) {
			/* does not exist */
			return 0;
		} else {
			perror("stat");
			return -1;
		}
	} else {
		if(S_ISDIR(s.st_mode)) {
			/* does exist */
			return 1;
		} else {
			/* exists but is no dir */
			return 2;
		}
	}
}
