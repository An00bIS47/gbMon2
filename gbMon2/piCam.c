//
//  piCam.c
//  gbMon2
//
//  Created by michael on 05.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#include <stdio.h>


/*
 * Cam (used in RRD-Thread)
 *********************************************************************************
 */
void* useCam(){
    
    char shellCommand [100];
    struct tm *t ;
    time_t tim ;
    char buf2[32];
    
    tim = time (NULL) ;
    t = localtime (&tim) ;
    
    sprintf (buf2,"%02d-%02d-%04d.jpg", t->tm_mday, t->tm_mon + 1, t->tm_year+1900);
    sprintf(shellCommand,"raspistill -o /home/pi/.gbmon/pics/%s",buf2);
    //printf("CAM command: %s",shellCommand);
    
    FILE * pp ;
    //char shellCommand[] = buf;
    pp = popen(shellCommand, "r");
    if (pp != NULL) {
        while (1) {
            char *line;
            char buf[1000];
            line = fgets(buf, sizeof buf, pp);
            if (line == NULL) break;
            if (line[0] == 'd') printf("%s", line); // line includes '\n'
        }
        pclose(pp);
    }
    
	/*
	 sprintf(shellCommand,"sshpass -p 'alpine' scp /home/pi/.gbmon/pics/%s service@192.168.178.10:./Sites/MacServer/gbmon/pics/",buf2);
	 pp = popen(shellCommand, "r");
	 pclose(pp);
	 */
    return 0;
}