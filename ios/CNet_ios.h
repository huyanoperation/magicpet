//
//  CNet_ios.h
//  magicpet
//
//  Created by jia huyan on 11-11-23.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once

//- (int) connectServer: (NSString *) hostIP port:(int) hostPort;
//- (void) showMessage:(NSString *) msg;
//- (IBAction) sendMsg;
//- (IBAction) reConnect;
//- (IBAction) textFieldDoneEditing:(id)sender;
//- (IBAction) backgroundTouch:(id)sender;

class CNet
{
public:
    static void connectServer( char* hostip, int hostPort );  
    
    static void showMessage( char* msg );
    
    static void sendMsg( char* msg, long len );
    
    static void reConnect( char* msg );
    
    static void textFieldDoneEditing();
    
    static void backgroundTouch();
    
    static void onConnectToHost( const char *Ip, int port );
    
    static void willDisconnectWithError();
    
    static void onSocketDidDisconnect();
    
    static void onSocketDidSecure();
    
    static void didReadData( int len, char* data );
    
    static void disConnect();
    
    static bool isConnect();
    
};