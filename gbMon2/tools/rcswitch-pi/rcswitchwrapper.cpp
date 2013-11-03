//
//  rcswitchwrapper.cpp
//  gbMon2
//
//  Created by michael on 03.11.13.
//  Copyright (c) 2013 michael. All rights reserved.
//
// g++ -c rcswitchwrapper.cpp -o rcswitchwrapper.o

#include "rcswitchwrapper.h"
#include "RCSwitch.h"

extern "C" {
    RCSwitch* newRCSwitch() {
        return new RCSwitch();
    }
    
    //void enableTransmit(int nTransmitterPin);
    void RCSwitch_enableTransmit(RCSwitch* rcs, int nTransmitterPin){
        rcs->enableTransmit(nTransmitterPin);
    }
	
	// mySwitch.switchOn(systemCode, unitCode);
	// void switchOn(int nGroupNumber, int nSwitchNumber);
    // void switchOff(int nGroupNumber, int nSwitchNumber)
	void RCSwitch_switchOn(RCSwitch* rcs, char* sGroup, int nSwitchNumber){
		rcs->switchOn(sGroup, nSwitchNumber);
	}
	
	void RCSwitch_switchOff(RCSwitch* rcs, char* sGroup, int nSwitchNumber){
		rcs->switchOff(sGroup, nSwitchNumber);
	}
    
    void deleteRCSwitch(RCSwitch* rcs) {
        delete rcs;
    }
}