//
//  SocketDemoViewController.m
//  SocketDemo
//
//  Created by xiang xiva on 10-7-10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "SocketDemoViewController.h"

SocketDemoViewController *g_pSocketControl = nil;

@implementation SocketDemoViewController

@synthesize inputMsg, outputMsg;
@synthesize client;
/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    //[super viewDidLoad];
	[self connectServer:HOST_IP port:HOST_PORT];
	//¼àÌý¶ÁÈ¡
	
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return YES;
}

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    //[super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	self.client = nil;
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
}

- (int) connectServer: (NSString *) hostIP port:(int) hostPort{
	
	if (client == nil) {
		client = [[AsyncSocket alloc] initWithDelegate:self];
		NSError *err = nil;
		//192.168.110.128
		if (![client connectToHost:hostIP onPort:hostPort error:&err]) {
			NSLog(@"%@ %@", [err code], [err localizedDescription]);
			
			UIAlertView *alert = [[UIAlertView alloc] initWithTitle:[@"Connection failed to host " 
											stringByAppendingString:hostIP] 
															message:[[[NSString alloc]initWithFormat:@"%@",[err code]] stringByAppendingString:[err localizedDescription]] 
														   delegate:self 
												  cancelButtonTitle:@"OK"
												  otherButtonTitles:nil];
			[alert show];
			[alert release];
			//client = nil;
			return SRV_CONNECT_FAIL;
		} else {
			NSLog(@"Conectou!");
			return SRV_CONNECT_SUC;
		}
	}
	else {
		[client readDataWithTimeout:-1 tag:0];
		return SRV_CONNECTED;
	}
	
}

- (IBAction) reConnect{
	int stat = [self connectServer:HOST_IP port:HOST_PORT];
	switch (stat) {
		case SRV_CONNECT_SUC:
			[self showMessage:@"connect success"];
			break;
		case SRV_CONNECTED:
			[self showMessage:@"It's connected,don't agian"];
			break;
		default:
			break;
	}
}

- (IBAction) sendMsg{
	
	NSString *inputMsgStr = self.inputMsg.text;
	NSString * content = [inputMsgStr stringByAppendingString:@"\r\n"];
	NSLog(@"%a",content);
	NSData *data = [content dataUsingEncoding:NSISOLatin1StringEncoding];
	[client writeData:data withTimeout:-1 tag:0];
	
	//[data release];
	//[content release];
	//[inputMsgStr release];
	//¼ÌÐø¼àÌý¶ÁÈ¡
	//[client readDataWithTimeout:-1 tag:0];
}

#pragma mark -
#pragma mark close Keyboard
- (IBAction) textFieldDoneEditing:(id)sender{
	[sender resignFirstResponder];
}

- (IBAction) backgroundTouch:(id)sender{
	[inputMsg resignFirstResponder];
}

#pragma mark socket uitl

- (void) showMessage:(NSString *) msg{
	UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"Alert!"
                                                    message:msg
                                                   delegate:nil
                                          cancelButtonTitle:@"OK"
                                          otherButtonTitles:nil];
    [alert show];
    [alert release];
}


#pragma mark socket delegate

- (void)onSocket:(AsyncSocket *)sock didConnectToHost:(NSString *)host port:(UInt16)port{
	[client readDataWithTimeout:-1 tag:0];
}

- (void)onSocket:(AsyncSocket *)sock willDisconnectWithError:(NSError *)err
{
    NSLog(@"Error");
}

- (void)onSocketDidDisconnect:(AsyncSocket *)sock
{
	NSString *msg = @"Sorry this connect is failure";
	[self showMessage:msg];
	[msg release];
	client = nil;
}

- (void)onSocketDidSecure:(AsyncSocket *)sock{
	
}

- (void)onSocket:(AsyncSocket *)sock didReadData:(NSData *)data withTag:(long)tag{
	
	NSString* aStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
	NSLog(@"Hava received datas is :%@",aStr);
	self.outputMsg.text = aStr;
	[aStr release];
	[client readDataWithTimeout:-1 tag:0];
}

#pragma mark dealloc

- (void)dealloc {
	
	[client release];
	[inputMsg release];
	[outputMsg release];
    //[super dealloc];
}

@end