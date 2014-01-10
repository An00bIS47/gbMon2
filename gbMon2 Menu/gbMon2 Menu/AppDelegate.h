//
//  AppDelegate.h
//  gbMon2 Menu
//
//  Created by michael on 10.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject {
    //NSWindow *window;
    IBOutlet NSMenu *statusMenu;
    NSStatusItem * statusItem;
    NSUserNotification * note;
    NSUserNotificationCenter * unc;
}

@property (assign) IBOutlet NSWindow *window;

@end
