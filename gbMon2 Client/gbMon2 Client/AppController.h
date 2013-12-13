//
//  AppController.h
//  gbMon2 Client
//
//  Created by michael on 13.12.13.
//  Copyright (c) 2013 michael. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AppController : NSObject <NSStreamDelegate> {
    NSOutputStream *outputStream;
    NSInputStream *inputStream;
}


@property (strong) NSInputStream *inputStream;
@property (strong) NSOutputStream *outputStream;
-(void)initNetworkCommunication;

@end
