//
//  rrdtool.c
//  gbMon2
//
//  Created by michael on 04.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#include "rrdtool.h"

/*
	rrdcreate myfile.rrd "DS:myval:GAUGE:600:0:U" "RRA:AVERAGE:0.5:1:576"
	>>>
	char *createparams[] = {
	"rrdcreate",
	"myfile.rrd",
	"DS:myval:GAUGE:600:0:U",
	"RRA:AVERAGE:0.5:1:576",
	NULL
	};
 
	optind = opterr = 0; // Because rrdtool uses getopt()
	rrd_clear_error();
	rrd_create(4, createparams);
 
 
 rrdtool create ~/.gbmon/db/temperature.rrd                                     \
 DS:temperature_1:GAUGE:600:0:U                                     \
 DS:temperature_2:GAUGE:600:0:U                                     \
 DS:temperature_3:GAUGE:600:0:U                                     \
 RRA:AVERAGE:0.5:1:2016                                             \
 RRA:AVERAGE:0.5:6:1824                                             \
 RRA:AVERAGE:0.5:12:1656                                            \
 RRA:AVERAGE:0.5:24:1176                                            \
 RRA:AVERAGE:0.5:288:830                                            \
 RRA:MIN:0.5:1:2016                                                 \
 RRA:MIN:0.5:6:1824                                                 \
 RRA:MIN:0.5:12:1656                                                \
 RRA:MIN:0.5:24:1176                                                \
 RRA:MIN:0.5:288:830                                                \
 RRA:MAX:0.5:1:2016                                                 \
 RRA:MAX:0.5:6:1824                                                 \
 RRA:MAX:0.5:12:1656                                                \
 RRA:MAX:0.5:24:1176                                                \
 RRA:MAX:0.5:288:830
 */

void createDBlightFan(){
	char *createparams[] = {
		"rrdcreate",
		"/home/pi/.gbmon/db/lightFan.rrd",
		"DS:light:GAUGE:600:0:U",
		"DS:fan:GAUGE:600:0:U",
		"RRA:AVERAGE:0.5:1:2016",
		"RRA:AVERAGE:0.5:6:1824",
		"RRA:AVERAGE:0.5:12:1656",
		"RRA:AVERAGE:0.5:24:1176",
		"RRA:AVERAGE:0.5:288:830",
		"RRA:MIN:0.5:1:2016",
		"RRA:MIN:0.5:6:1824",
		"RRA:MIN:0.5:12:1656",
		"RRA:MIN:0.5:24:1176",
		"RRA:MIN:0.5:288:830",
		"RRA:MAX:0.5:1:2016",
		"RRA:MAX:0.5:6:1824",
		"RRA:MAX:0.5:12:1656",
		"RRA:MAX:0.5:24:1176",
		"RRA:MAX:0.5:288:830",
		NULL
	};
	
	optind = opterr = 0; // Because rrdtool uses getopt()
	rrd_clear_error();
	rrd_create(19, createparams);
	
	/* Was it OK ? */
	if (rrd_test_error() != 0) {
		printf("Graph error: %s\n", rrd_get_error());
	}
	
}

void createDBecLevel(){
	char *createparams[] = {
		"rrdcreate",
		"/home/pi/.gbmon/db/ecLevel.rrd",
		"DS:ecLevel_1:GAUGE:600:0:U",
		"DS:ecLevel_2:GAUGE:600:0:U",
		"DS:ecLevel_3:GAUGE:600:0:U",
		"RRA:AVERAGE:0.5:1:2016",
		"RRA:AVERAGE:0.5:6:1824",
		"RRA:AVERAGE:0.5:12:1656",
		"RRA:AVERAGE:0.5:24:1176",
		"RRA:AVERAGE:0.5:288:830",
		"RRA:MIN:0.5:1:2016",
		"RRA:MIN:0.5:6:1824",
		"RRA:MIN:0.5:12:1656",
		"RRA:MIN:0.5:24:1176",
		"RRA:MIN:0.5:288:830",
		"RRA:MAX:0.5:1:2016",
		"RRA:MAX:0.5:6:1824",
		"RRA:MAX:0.5:12:1656",
		"RRA:MAX:0.5:24:1176",
		"RRA:MAX:0.5:288:830",
		NULL
	};
	
	optind = opterr = 0; // Because rrdtool uses getopt()
	rrd_clear_error();
	rrd_create(20, createparams);
	
	/* Was it OK ? */
	if (rrd_test_error() != 0) {
		printf("Graph error: %s\n", rrd_get_error());
	}
	
}

void createDBHumidity(){
	char *createparams[] = {
		"rrdcreate",
		"/home/pi/.gbmon/db/humidity.rrd",
		"DS:humidity_1:GAUGE:600:0:U",
		"RRA:AVERAGE:0.5:1:2016",
		"RRA:AVERAGE:0.5:6:1824",
		"RRA:AVERAGE:0.5:12:1656",
		"RRA:AVERAGE:0.5:24:1176",
		"RRA:AVERAGE:0.5:288:830",
		"RRA:MIN:0.5:1:2016",
		"RRA:MIN:0.5:6:1824",
		"RRA:MIN:0.5:12:1656",
		"RRA:MIN:0.5:24:1176",
		"RRA:MIN:0.5:288:830",
		"RRA:MAX:0.5:1:2016",
		"RRA:MAX:0.5:6:1824",
		"RRA:MAX:0.5:12:1656",
		"RRA:MAX:0.5:24:1176",
		"RRA:MAX:0.5:288:830",
		NULL
	};
	
	optind = opterr = 0; // Because rrdtool uses getopt()
	rrd_clear_error();
	rrd_create(18, createparams);
	
	/* Was it OK ? */
	if (rrd_test_error() != 0) {
		printf("Graph error: %s\n", rrd_get_error());
	}
	
}

void createDBTemperature(){
	char *createparams[] = {
		"rrdcreate",
		"/home/pi/.gbmon/db/temperature.rrd",
		"DS:temperature_1:GAUGE:600:0:U",
		"DS:temperature_2:GAUGE:600:0:U",
		"DS:temperature_3:GAUGE:600:0:U",
		"RRA:AVERAGE:0.5:1:2016",
		"RRA:AVERAGE:0.5:6:1824",
		"RRA:AVERAGE:0.5:12:1656",
		"RRA:AVERAGE:0.5:24:1176",
		"RRA:AVERAGE:0.5:288:830",
		"RRA:MIN:0.5:1:2016",
		"RRA:MIN:0.5:6:1824",
		"RRA:MIN:0.5:12:1656",
		"RRA:MIN:0.5:24:1176",
		"RRA:MIN:0.5:288:830",
		"RRA:MAX:0.5:1:2016",
		"RRA:MAX:0.5:6:1824",
		"RRA:MAX:0.5:12:1656",
		"RRA:MAX:0.5:24:1176",
		"RRA:MAX:0.5:288:830",
		NULL
	};
	
	optind = opterr = 0; // Because rrdtool uses getopt()
	rrd_clear_error();
	rrd_create(20, createparams);
	
	/* Was it OK ? */
	if (rrd_test_error() != 0) {
		printf("Graph error: %s\n", rrd_get_error());
	}
	
}

void createDBs(){
	createDBTemperature();
	createDBHumidity();
	createDBecLevel();
	createDBlightFan();
}



/*
 rrdupdate myfile.rrd "0:42"
 >>>
 char *updateparams[] = {
 "rrdupdate",
 "myfile.rrd",
 "0:42",
 NULL
 };
 
 optind = opterr = 0;
 rrd_clear_error();
 rrd_update(3, updateparams);
 */


void updateDBHumidity(){
	sem_wait(&semaLockInfo);       // down semaphore
	char str[80];
	
	sprintf(str, "N:%.1f", current.humidity);
	sem_post(&semaLockInfo);       // up semaphore
	
	char *updateparams[] = {
		"rrdupdate",
		"/home/pi/.gbmon/db/humidity.rrd",
		str,
		NULL
	};
	
	optind = opterr = 0;
	rrd_clear_error();
	rrd_update(3, updateparams);
	
	/* Was it OK ? */
	if (rrd_test_error() != 0) {
		printf("rrd error: %s\n", rrd_get_error());
	}
	
}


void updateDBTemperature(){
	sem_wait(&semaLockInfo);       // down semaphore

	char str[80];
	sprintf(str, "N:%.1f:%.1f:%.1f", current.temperature[0],current.temperature[1],current.temperature[2]);

	sem_post(&semaLockInfo);       // up semaphore

	char *updateparams[] = {
		"rrdupdate",
		"/home/pi/.gbmon/db/temperature.rrd",
		str,
		NULL
	};
	
	optind = opterr = 0;
	rrd_clear_error();
	rrd_update(3, updateparams);
	
	/* Was it OK ? */
	if (rrd_test_error() != 0) {
		printf("rrd error: %s\n", rrd_get_error());
	}
}

/*
void updateDBlightFan(){
	char * str;
	
	if (strcmp("Light On ",current.status[0])) {
		sprintf(str, "N:%d:%d", 1,fanToggle);
	} else {
		sprintf(str, "N:%d:%d", 0,fanToggle);
	}

	char *updateparams[] = {
		"rrdupdate",
		"/home/pi/.gbmon/db/lightFan.rrd",
		str,
		NULL
	};
	
	optind = opterr = 0;
	rrd_clear_error();
	rrd_update(3, updateparams);
	
	// Was it OK ?
	if (rrd_test_error() != 0) {
		printf("rrd error: %s\n", rrd_get_error());
	}
}

void updateDBecLevel(){
	char * str;
	sprintf(str, "N:%d:%d:%d", current.value[4],current.value[5],current.value[6]);
	char *updateparams[] = {
		"rrdupdate",
		"/home/pi/.gbmon/db/ecLevel.rrd",
		str,
		NULL
	};
	
	optind = opterr = 0;
	rrd_clear_error();
	rrd_update(3, updateparams);
	
	// Was it OK ?
	if (rrd_test_error() != 0) {
		printf("rrd error: %s\n", rrd_get_error());
	}
}
*/

void updateDBs(){
	updateDBHumidity();
	updateDBTemperature();
}



/*
	 char **calcpr = NULL;
	 int rrdargcount, xsize, ysize, result;
	 double ymin, ymax;
	 char *rrdargs[] = {
	 "rrdgraph",
	 "mygraph.png",
	 "-a", "PNG",
	 "--title", "My Fancy Graph",
	 "DEF:myval=myfile.rrd:myval:AVERAGE",
	 "LINE2:myval#00CC00:The value",
	 NULL
	 };
	 
	 optind = opterr = 0;
	 rrd_clear_error();
	 #ifdef RRDTOOL12
	 result = rrd_graph(8, rrdargs, &calcpr, &xsize, &ysize, NULL, &ymin, &ymax);
	 #else
	 result = rrd_graph(8, rrdargs, &calcpr, &xsize, &ysize);
	 #endif
	 
	 // Was is OK??
	if (rrd_test_error() || (result != 0)) {
		if (calcpr) {
			int i;
			for (i=0; (calcpr[i]); i++) free(calcpr[i]);
			calcpr = NULL;
		}
		
		printf("Graph error: %s\n", rrd_get_error());
	}

*/

void* rrdMain(void *args){
	debugPrint(true, true, "RRD Update Thread started", true, "RRDTOOL");
	//debugPrint(false, false, "OK", true, "RRDTOOL");
	
	int lastTime=(int)time(NULL);
	debugPrint(true, true, "Updating Database ...", false, "RRDTOOL");
	updateDBs();
	debugPrint(false, false, "OK", true, "RRDTOOL");
	
	
	for (;;) {
		
		if (lastTime + 300 <= ((int)time(NULL)) ) {
			debugPrint(true, true, "Updating Database ...", false, "RRDTOOL");
			updateDBs();
			debugPrint(false, false, "OK", true, "RRDTOOL");
			
			lastTime=(int)time(NULL);
			//printf("Lasttime: %d", lastTime);
		}
		
	
	}
	
	return 0;
}