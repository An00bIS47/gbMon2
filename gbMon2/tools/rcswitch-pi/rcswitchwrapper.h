//
//  rcswitchwrapper.h
//  gbMon2
//
//  Created by michael on 03.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
// g++ -c rcswitchwrapper.cpp -o rcswitchwrapper.o

#ifndef __gbMon2__rcswitchwrapper__
#define __gbMon2__rcswitchwrapper__

//#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct RCSwitch RCSwitch;
    
    RCSwitch* newRCSwitch();
    
    void RCSwitch_enableTransmit(RCSwitch* rcs, int nTransmitterPin);
	void RCSwitch_switchOn(RCSwitch* rcs, char* sGroup, int nSwitchNumber);
	void RCSwitch_switchOff(RCSwitch* rcs, char* sGroup, int nSwitchNumber);
	
    void deleteRCSwitch(RCSwitch* rcs);
    
#ifdef __cplusplus
}
#endif


#endif /* defined(__C__inC__MyWrapper__) */