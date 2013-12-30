//
//  AppDelegate.h
//  gbMon2 Client
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class GCDAsyncUdpSocket;


@interface AppDelegate : NSObject <NSApplicationDelegate>
{
	long tag;
	GCDAsyncUdpSocket *udpSocket;
}

@property (unsafe_unretained) IBOutlet NSWindow    * window;
@property  IBOutlet NSTextField * addrField;
@property  IBOutlet NSTextField * portField;
@property  IBOutlet NSTextField * messageField;
@property  IBOutlet NSButton    * sendButton;
@property  IBOutlet NSSegmentedControl    * fanStatus;

@property IBOutlet  NSTextField *temperatureField;
@property IBOutlet  NSTextField *humidityField;
@property IBOutlet  NSTextField *descField;
@property  IBOutlet NSTextView  *logView;


- (void)updateDisplay;
- (IBAction)disclosureTriangleClicked:(id)sender;
- (IBAction)send:(id)sender;

@end