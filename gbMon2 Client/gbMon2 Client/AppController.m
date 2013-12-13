//
//  AppController.m
//  gbMon2 Client
//
//  Created by michael on 13.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#import "AppController.h"

//@synthesize inputStream, outputStream;
@implementation AppController 
    -(void)sendaResponse{
        NSString *response = [NSString stringWithFormat:@"Hello from Ipad Simulator"];
        NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
        [outputStream write:[data bytes] maxLength:[data length]];
        NSLog(@"%u", outputStream.streamStatus);
    }

-(void)initNetworkCommunication{
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    CFStreamCreatePairWithSocketToHost(NULL, (CFStringRef)@"192.168.178.20", 1000, &readStream, &writeStream);
    
    //NSInputStream *inputStream = (__bridge NSInputStream *) (readStream);
    //NSOutputStream *outputStream = (__bridge NSOutputStream *) (writeStream);
    
    [inputStream setDelegate:self];
    [outputStream setDelegate:self];
    [inputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [outputStream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    [inputStream open];
    [outputStream open];
    
    self.inputStream = inputStream;
    self.outputStream = outputStream;
    
    NSLog(@"InputStream: %u and  OutputStream: %u", inputStream.streamStatus, outputStream.streamStatus);
    NSString *response = [NSString stringWithFormat:@"getTemperature"];
    NSData *data = [[NSData alloc] initWithData:[response dataUsingEncoding:NSASCIIStringEncoding]];
    [outputStream write:[data bytes] maxLength:[data length]];
    NSLog(@"%u", outputStream.streamStatus);
}

-(void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    switch (eventCode) {
        case NSStreamEventOpenCompleted:
            NSLog(@"Stream Opened %u", outputStream.streamStatus);
            break;
            
        case NSStreamEventHasBytesAvailable:
            if(aStream == inputStream)
            {
                uint8_t buffer [1024];
                int len;
                
                while ([inputStream hasBytesAvailable])
                {
                    len = [inputStream read:buffer maxLength:sizeof(buffer)];
                    if(len>0)
                    {
                        NSString *output = [[NSString alloc] initWithBytes:buffer length:len encoding:NSASCIIStringEncoding];
                        
                        if(nil != output)
                        {
                            NSLog(@"Server said: %@", output);
                        }
                    }
                }
            }
            break;
            
        case NSStreamEventErrorOccurred:
            NSLog(@"Can not connect to the host!");
            break;
            
        case NSStreamEventEndEncountered:
            if(aStream == inputStream)
                NSLog(@"Ending input stream");
            if(aStream == outputStream)
                NSLog(@"Ending output stream");
            
            [aStream close];
            [aStream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
            //[aStream release];
            aStream = nil;
            break;
            
        default:
            break;
    }
}

@end
