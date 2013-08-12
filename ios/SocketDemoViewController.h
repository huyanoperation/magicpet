//
//  SocketDemoViewController.h
//  SocketDemo
//
//  Created by xiang xiva on 10-7-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.

#import <UIKit/UIKit.h>
#import "AsyncSocket.h"
#define SRV_CONNECTED 0
#define SRV_CONNECT_SUC 1
#define SRV_CONNECT_FAIL 2
#define HOST_IP @"113.31.30.107"
#define HOST_PORT 1111

@interface SocketDemoViewController : UIViewController {
	
	UITextField *inputMsg;
	UILabel *outputMsg;
	AsyncSocket *client;
}

@property (nonatomic, retain) AsyncSocket *client;
@property (nonatomic, retain) IBOutlet UITextField *inputMsg;
@property (nonatomic, retain) IBOutlet UILabel *outputMsg;

- (int) connectServer: (NSString *) hostIP port:(int) hostPort;
- (void) showMessage:(NSString *) msg;
- (IBAction) sendMsg:(NSData*) msg leng:(long) tag;
- (IBAction) reConnect;
- (IBAction) textFieldDoneEditing:(id)sender;
- (IBAction) backgroundTouch:(id)sender;
-(void)disconnectAfterReadingAndWriting;
-(bool)isConnected;
@end

extern SocketDemoViewController *g_pSocketControl;