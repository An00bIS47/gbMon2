//
//  AppDelegate.m
//  gbMon2 Menu
//
//  Created by michael on 10.01.14.
//  Copyright (c) 2014 michael. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}



-(void)awakeFromNib{
    note = [[NSUserNotification alloc] init];
    unc = [NSUserNotificationCenter defaultUserNotificationCenter];
    //unc.delegate = self;
    
    note.title = @"gbMon";
    note.subtitle = @"gbMon started";
    [unc deliverNotification:note];
    
    
    statusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
    [statusItem setMenu:statusMenu];
    //[statusItem setTitle:@"21.1° 37.6%"];
    [statusItem setHighlightMode:YES];
    [statusItem setImage:[ NSImage imageNamed:@"temperature-16"]];
    
    NSDictionary *attributes = @{
                                 NSFontAttributeName: [NSFont fontWithName:@"Lucida Grande" size:13.0]};
    NSAttributedString *attributedTitle = [[NSAttributedString alloc] initWithString:@"21.1°  36.7%" attributes:attributes];
    [statusItem setAttributedTitle:attributedTitle];
}

@end
