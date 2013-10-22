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
/**
	Load Settings from File
	@param file settingsFile
	@returns exit code
 */
int Settings_Load(char* file);

/**
	<#Description#>
	@returns <#return value description#>
 */
int Settings_Unload();

/**
	<#Description#>
	@param category <#category description#>
	@param key <#key description#>
	@returns <#return value description#>
 */
char* Settings_Get(char* category, char* key);

/**
	<#Description#>
	@param category <#category description#>
	@param key <#key description#>
	@param value <#value description#>
	@returns <#return value description#>
 */
int Settings_Add(char* category, char* key, char* value);

/**
	<#Description#>
	@param file <#file description#>
	@returns <#return value description#>
 */
int Settings_Save(char* file);


/**
	<#Description#>
	@param str <#str description#>
	@returns <#return value description#>
 */
char* trim(char* str);

#endif
