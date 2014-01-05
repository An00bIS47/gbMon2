//
//  AppDelegate.m
//  gbMon2 Client
//
//  Created by michael on 28.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#import "AppDelegate.h"
#import "GCDAsyncUdpSocket.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize addrField;
@synthesize portField;
@synthesize messageField;
@synthesize sendButton;
@synthesize logView;
@synthesize fanStatus;

@synthesize descField;
@synthesize temperatureField;
@synthesize minTemperatureField;
@synthesize maxTemperatureField;
@synthesize humidityField;
@synthesize minHumidityField;
@synthesize maxHumidityField;
@synthesize lightIndicator;

@synthesize webImage;

#define FORMAT(format, ...) [NSString stringWithFormat:(format), ##__VA_ARGS__]


- (void)updateDisplay{
    
    for (; ; ) {
        NSString *host = [addrField stringValue];
        if ([host length] == 0){
            [self logError:@"Address required"];
            return;
        }
        
        int port = [portField intValue];
        if (port <= 0 || port > 65535){
            [self logError:@"Valid port required"];
            return;
        }
        
        NSString *msg = @"getAllJSON";
        tag=0;
        NSLog(@"SENT (%i): %@", (int)tag, msg);
        //[self logMessage:FORMAT(@"SENT (%i): %@", (int)tag, msg)];
        NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
        [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
        
        
        
        // Load webcam image from lighttpd webserver and image.jpg at document root
        //NSURL *imageURL = [NSURL URLWithString:@"http://chart.apis.google.com/chart?cht=p3&chs=700x400&chd=t:20,20,20,20,20&chl=A%7CB%7CC%7CD%7CE&chco=66FF33,3333CC"];
        NSURL *imageURL = [NSURL URLWithString:@"http://192.168.178.20/latest.jpg"];
        NSData *imageData = [imageURL resourceDataUsingCache:NO];
        NSImage *imageFromBundle = [[NSImage alloc] initWithData:imageData];
        [webImage setImage:imageFromBundle];
        
        sleep(1);
    }

}

- (IBAction)showDetails:(id)sender {
    //NSWindow *window =[window];
    NSRect frameWinOld = [_window frame];
    
    // Titlebar Height seems to be 22 Pixel -> +22
    NSRect frameWinSmall = NSMakeRect(frameWinOld.origin.x, frameWinOld.origin.y, 369,frameWinOld.size.height);
    NSRect frameWinBig = NSMakeRect(frameWinOld.origin.x, frameWinOld.origin.y, 720,frameWinOld.size.height);
    
    if (toggleDetails == YES){
        NSLog(@"Show Details");
        
        [descField setStringValue:@"Hide Details"];
        //[otherBox setHidden:NO];
        [addrField setHidden:NO];;
        [portField setHidden:NO];
        [messageField setHidden:NO];
        [logView setHidden:NO];
        [sendButton setHidden:NO];
        [_window setFrame:frameWinBig display:YES animate:YES];
        toggleDetails = NO;
        
    } else {
        NSLog(@"Hide Details");
        //[window setFrame:NSRectMake(frameWinOld.origin.x, frameWinOld.origin.y, frameWinOld.width, frameWinOld.height - heightOtherBoxWithMargin) display:YES];
        //[otherBox setHidden:YES];
        [descField setStringValue:@"Show Details"];
        [addrField setHidden:YES];
        [portField setHidden:YES];
        [messageField setHidden:YES];
        [logView setHidden:YES];
        [sendButton setHidden:YES];
        [_window setFrame:frameWinSmall display:YES animate:YES];
        toggleDetails = YES;
    }
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Setup our socket.
	// The socket will invoke our delegate methods using the usual delegate paradigm.
	// However, it will invoke the delegate methods on a specified GCD delegate dispatch queue.
	//
	// Now we can configure the delegate dispatch queues however we want.
	// We could simply use the main dispatc queue, so the delegate methods are invoked on the main thread.
	// Or we could use a dedicated dispatch queue, which could be helpful if we were doing a lot of processing.
	//
	// The best approach for your application will depend upon convenience, requirements and performance.
	//
	// For this simple example, we're just going to use the main thread.
	
	udpSocket = [[GCDAsyncUdpSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
	
	NSError *error = nil;
	
	if (![udpSocket bindToPort:0 error:&error])
	{
		NSLog(@"Error binding: %@", error);
		return;
	}
	if (![udpSocket beginReceiving:&error])
	{
		NSLog(@"Error receiving: %@", error);
		return;
	}
	
    [portField setStringValue:@"1000"];
    [addrField setStringValue:@"192.168.178.20"];
    
	/*
        Background Thread
        Starts thread constanstly requests getAllJSON to Update Display
        sleeps 1 seconds for next request
    */
    [self performSelectorInBackground:@selector(updateDisplay) withObject:nil];

    
    [fanStatus setTarget:self];
    [fanStatus setAction:@selector(fanStatusClicked:)];
    
    toggleDetails = YES;
    [addrField setHidden:YES];
    [portField setHidden:YES];
    [logView setHidden:YES];
    
    NSLog(@"gbMon2 Client started");
    
    /*
    sleep(1);
    msg = @"getTemperature";
    tag=1;
    NSLog(@"TAG: (%i)",(int)tag);
    [self logMessage:FORMAT(@"SENT (%i): %@", (int)tag, msg)];
    data = [msg dataUsingEncoding:NSUTF8StringEncoding];
	[udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
    
    sleep(1);
    msg = @"getHumidity";
    tag=2;
    NSLog(@"TAG: (%i)",(int)tag);
    [self logMessage:FORMAT(@"SENT (%i): %@", (int)tag, msg)];
    data = [msg dataUsingEncoding:NSUTF8StringEncoding];
	[udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
    */
}

- (IBAction)makePic:(id)sender{
    
    
    NSString *host = [addrField stringValue];
    if ([host length] == 0){
        [self logError:@"Address required"];
        return;
    }
    
    int port = [portField intValue];
    if (port <= 0 || port > 65535){
        [self logError:@"Valid port required"];
        return;
    }
    NSLog(@"Make Picture");
    NSString *msg = @"makePic";
    tag=3;
    NSLog(@"SENT (%i): %@", (int)tag, msg);
    //[self logMessage:FORMAT(@"SENT (%i): %@", (int)tag, msg)];
    NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
    [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
}

- (IBAction)setFan:(id)sender{

    //int clickedSegmentTag = [[sender cell] tagForSegment:clickedSegment];
    if (toggleFan==YES){
        NSLog(@"Toggle Fan Off");
    } else {
        NSLog(@"Toggle Fan On");
    }
    
    NSString *host = [addrField stringValue];
    if ([host length] == 0){
        [self logError:@"Address required"];
        return;
    }
    
    int port = [portField intValue];
    if (port <= 0 || port > 65535){
        [self logError:@"Valid port required"];
        return;
    }
    
    NSString *msg = @"setFan";
    tag=3;
    NSLog(@"SENT (%i): %@", (int)tag, msg);
    //[self logMessage:FORMAT(@"SENT (%i): %@", (int)tag, msg)];
    NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
    [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
}

- (IBAction)fanStatusClicked:(id)sender{
    int clickedSegment = [sender selectedSegment];
    //int clickedSegmentTag = [[sender cell] tagForSegment:clickedSegment];
    if (clickedSegment==0){
        NSLog(@"Toggle Fan Off");
    } else {
        NSLog(@"Toggle Fan On");
    }
    
    NSString *host = [addrField stringValue];
    if ([host length] == 0){
        [self logError:@"Address required"];
        return;
    }
    
    int port = [portField intValue];
    if (port <= 0 || port > 65535){
        [self logError:@"Valid port required"];
        return;
    }
    
    NSString *msg = @"setFan";
    tag=3;
    NSLog(@"SENT (%i): %@", (int)tag, msg);
    //[self logMessage:FORMAT(@"SENT (%i): %@", (int)tag, msg)];
    NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
    [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
}

- (void)scrollToBottom {
	NSScrollView *scrollView = [logView enclosingScrollView];
	NSPoint newScrollOrigin;
	
	if ([[scrollView documentView] isFlipped])
		newScrollOrigin = NSMakePoint(0.0F, NSMaxY([[scrollView documentView] frame]));
	else
		newScrollOrigin = NSMakePoint(0.0F, 0.0F);
	
	[[scrollView documentView] scrollPoint:newScrollOrigin];
}

- (void)logError:(NSString *)msg {
	NSString *paragraph = [NSString stringWithFormat:@"%@\n", msg];
	
	NSMutableDictionary *attributes = [NSMutableDictionary dictionaryWithCapacity:1];
	[attributes setObject:[NSColor redColor] forKey:NSForegroundColorAttributeName];
	
	NSAttributedString *as = [[NSAttributedString alloc] initWithString:paragraph attributes:attributes];
	
	[[logView textStorage] appendAttributedString:as];
	[self scrollToBottom];
}

- (void)logInfo:(NSString *)msg {
	NSString *paragraph = [NSString stringWithFormat:@"%@\n", msg];
	
	NSMutableDictionary *attributes = [NSMutableDictionary dictionaryWithCapacity:1];
	[attributes setObject:[NSColor purpleColor] forKey:NSForegroundColorAttributeName];
	
	NSAttributedString *as = [[NSAttributedString alloc] initWithString:paragraph attributes:attributes];
	
	[[logView textStorage] appendAttributedString:as];
	[self scrollToBottom];
}

- (void)logMessage:(NSString *)msg {
	NSString *paragraph = [NSString stringWithFormat:@"%@\n", msg];
	
	NSMutableDictionary *attributes = [NSMutableDictionary dictionaryWithCapacity:1];
	[attributes setObject:[NSColor blackColor] forKey:NSForegroundColorAttributeName];
	
	NSAttributedString *as = [[NSAttributedString alloc] initWithString:paragraph attributes:attributes];
	
	[[logView textStorage] appendAttributedString:as];
	[self scrollToBottom];
}

- (IBAction)send:(id)sender {
	NSString *host = [addrField stringValue];
	if ([host length] == 0)
	{
		[self logError:@"Address required"];
		return;
	}
	
	int port = [portField intValue];
	if (port <= 0 || port > 65535)
	{
		[self logError:@"Valid port required"];
		return;
	}
	
	NSString *msg = [messageField stringValue];
	if ([msg length] == 0) {
		[self logError:@"Message required"];
		return;
	}

    if ([msg isEqualToString:@"getAllJSON"]){
        tag=0;
        NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
        [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
    } else if ([msg isEqualToString:@"getTemperature"]){
         tag=1;
         NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
         [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
    } else if ([msg isEqualToString:@"getHumidity"]){
         tag=2;
         NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
         [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
    } else {
        [self logMessage:FORMAT(@"SENT (%i): %@", (int)tag, msg)];
        NSData *data = [msg dataUsingEncoding:NSUTF8StringEncoding];
        [udpSocket sendData:data toHost:host port:port withTimeout:-1 tag:tag];
        NSLog(@"SENT (%i): %@", (int)tag, msg);
    }
    tag++;
 
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didSendDataWithTag:(long)tag
{
	// You could add checks here
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didNotSendDataWithTag:(long)tag dueToError:(NSError *)error
{
	// You could add checks here
}

- (void)udpSocket:(GCDAsyncUdpSocket *)sock didReceiveData:(NSData *)data
      fromAddress:(NSData *)address
withFilterContext:(id)filterContext
{
	NSString *msg = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
	if (msg) {
        
        if (tag==0) {
            NSLog(@"TAG: %li getAllJSON >>> : \n%@", tag, msg);
            [self logMessage:FORMAT(@"RECEIVED (%i): %@", (int)tag, msg)];
            NSError *error = NULL;
            NSData* data = [msg dataUsingEncoding:NSUTF8StringEncoding];
            NSDictionary* json = [NSJSONSerialization
                                  JSONObjectWithData:data
                                  options:kNilOptions
                                  error:&error];
            
            
            // Humidity
            [humidityField setStringValue:[NSString stringWithFormat: @"%@ %%", [json objectForKey:@"Humidity"]]];
            [minHumidityField setStringValue:[NSString stringWithFormat: @"%@ %%", [json objectForKey:@"minHumidity"]]];
            [maxHumidityField setStringValue:[NSString stringWithFormat: @"%@ %%", [json objectForKey:@"maxHumidity"]]];
            
            // Temperature
            NSArray *temperatureArray = [json objectForKey:@"Temperature"];//resultArray contains array type objects...
            /*
            for (NSArray *arr in resultArray) {
                NSLog([arr description]);
            }
            */
            [temperatureField setStringValue:[NSString stringWithFormat: @"%@°C", [temperatureArray valueForKey:@"Temperature1"]]];
            [minTemperatureField setStringValue:[NSString stringWithFormat: @"%@°C", [temperatureArray valueForKey:@"minTemperature1"]]];
            [maxTemperatureField setStringValue:[NSString stringWithFormat: @"%@°C", [temperatureArray valueForKey:@"maxTemperature1"]]];
            
            // Fan (SegmentControl)
            if ([[json objectForKey:@"Fan"] isEqualToString:@"1"]) {
                [fanStatus setSelectedSegment:0];
                toggleFan=YES;
            } else {
                [fanStatus setSelectedSegment:1];
                toggleFan=NO;
            }
            
            // Light (NSLevelIndicator)
            NSLog(@"LightValue: %@", [json objectForKey:@"LightValue"]);
            NSString *levelIndicatorString = [json objectForKey:@"LightValue"];
            CGFloat lightIndicatorValue = [levelIndicatorString floatValue];
            lightIndicatorValue = lightIndicatorValue / 100;
            NSLog(@"LightIndicatorValue: %f",lightIndicatorValue);
            [lightIndicator setFloatValue:lightIndicatorValue];
            
        } else if (tag==1){
            NSLog(@"TAG: %li >>>getTemperature: %@", tag, msg);
            [self logMessage:FORMAT(@"RECEIVED (%i): %@", (int)tag, msg)];
            //[temperatureField setStringValue:[NSString stringWithFormat: @"%@°C", msg]];
        } else if (tag==2){
            NSLog(@"TAG: %li >>>getHumidity: %@", tag, msg);
            [self logMessage:FORMAT(@"RECEIVED (%i): %@", (int)tag, msg)];
            //[humidityField setStringValue:[NSString stringWithFormat: @"%@ %%", msg]];
        } else if (tag==3){
            NSLog(@"TAG: %li >>>setFan: %@", tag, msg);
            //[humidityField setStringValue:[NSString stringWithFormat: @"%@ %%", msg]];
        } else {
            //[self logMessage:FORMAT(@"RECV: %@", msg)];
        }
        if (tag!=0){
            [self logMessage:FORMAT(@"RECEIVED (%i): %@", (int)tag, msg)];
        }

	} else {
		NSString *host = nil;
		uint16_t port = 0;
		[GCDAsyncUdpSocket getHost:&host port:&port fromAddress:address];
		
		[self logInfo:FORMAT(@"RECV: Unknown message from: %@:%hu", host, port)];
	}
}

@end
