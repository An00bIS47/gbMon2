//
//  AppDelegate.m
//  gbMon2 Client
//
//  Created by michael on 13.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#import "AppDelegate.h"
#import "AppController.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    

    
    //int main (int argc, const char * argv[]) {
       // NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
        
        AppController *c = [[AppController alloc] init];
    
        
        [c initNetworkCommunication];
        //[c open];
    
        //[c writeOut:@"getTemperature"];
        //[pool drain];
        
        //return 0;
    //}
    
}

@end
