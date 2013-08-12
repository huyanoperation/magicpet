//
//  CNet_ios.m
//  magicpet
//
//  Created by jia huyan on 11-11-23.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#import "CNet_ios.h"
#import "SocketDemoViewController.h"
#include "../Classes/Net/CNetManager.h"

//- (int) connectServer: (NSString *) hostIP port:(int) hostPort;
//- (void) showMessage:(NSString *) msg;
//- (IBAction) sendMsg;
//- (IBAction) reConnect;
//- (IBAction) textFieldDoneEditing:(id)sender;
//- (IBAction) backgroundTouch:(id)sender;

void CNet::connectServer( char* hostip, int hostPort )
{
    NSString *str = [NSString stringWithUTF8String:hostip];
    [g_pSocketControl connectServer:str port:hostPort];
}
//----------------------------------------------------
//
//
void CNet::showMessage( char* msg )
{
    NSString *str = [NSString stringWithUTF8String:msg];
    [g_pSocketControl showMessage:str ];
}
//----------------------------------------------------
//
//
void CNet::sendMsg( char* msg, long len )
{
    NSData *content=[NSData dataWithBytes:msg length:len];
    [g_pSocketControl sendMsg:content leng:len ];
}
//----------------------------------------------------
//
//
void CNet::reConnect( char* msg )
{
    [g_pSocketControl reConnect ];
}
//----------------------------------------------------
//
//
void CNet::textFieldDoneEditing()
{

}
//----------------------------------------------------
//
//
void CNet::backgroundTouch()
{
    
}
//----------------------------------------------------
//
//
void CNet::onConnectToHost( const char *Ip, int port )
{
    CNetManager::GetInstance()->onConnectToHost(Ip, port);
}
//----------------------------------------------------
//
//
void CNet::willDisconnectWithError()
{
    CNetManager::GetInstance()->willDisconnectWithError();
}
//----------------------------------------------------
//
//
void CNet::onSocketDidDisconnect()
{
    CNetManager::GetInstance()->onSocketDidDisconnect();
}
//----------------------------------------------------
//
//
void CNet::onSocketDidSecure()
{
    CNetManager::GetInstance()->onSocketDidSecure();
}
//---------------------------------------------------
//
//
void CNet::disConnect()
{
    [g_pSocketControl disconnectAfterReadingAndWriting];
}
//---------------------------------------------------
//
//
bool CNet::isConnect()
{
    return [g_pSocketControl isConnected];
}
//----------------------------------------------------
//
//
void CNet::didReadData( int len, char* data )
{
    //char buffer[1024];
    //memmove(buffer, [NSData dataWithBytes:data length:1024], 1024);
    CNetManager::GetInstance()->OnSocketRecieveData(len, data);
}
