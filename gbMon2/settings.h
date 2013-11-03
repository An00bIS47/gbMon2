//
//  settings.h
//  gbmon2
//
//  Created by michael on 19.10.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#ifndef gbmon2_settings_h
#define gbmon2_settings_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int Settings_Load(char* file);
int Settings_Unload();
char* Settings_Get(char* category, char* key);
int Settings_Add(char* category, char* key, char* value);
int Settings_Save(char* file);
char* trim(char* str);

#endif
