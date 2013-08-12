//
//  CNetManager.h
//  magicpet
//
//  Created by jia huyan on 11-11-24.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#pragma once

//static void connectServer( char* hostip, int hostPort );  
//static void showMessage( char* msg );
//static void sendMsg( char* msg );
//static void reConnect( char* msg );
//static void textFieldDoneEditing();
//static void backgroundTouch();
//static void onConnectToHost( const char *Ip, int port );
//static void willDisconnectWithError();
//static void onSocketDidDisconnect();
//static void onSocketDidSecure();
//static void didReadData( int len, const char* buffer );
#pragma pack( push,1 )

struct CThreadCommand1
{
    
public:
	unsigned long	m_dwSize;//
	char            m_Data[1];
public:
	unsigned long GetSize(){return 4 + m_dwSize;}
};

struct PT_GET_BONUS_LIST_CHECK_INFO
{
	unsigned short  wType;
	unsigned long   dwGameId;
	unsigned long   dwRoomId;
	unsigned long   dwUserId;
	char			chName[16];
	PT_GET_BONUS_LIST_CHECK_INFO():wType(123){}
};

#pragma pack(pop)

class CNetManager
{
public:
    CNetManager();
    ~CNetManager();
    static CNetManager *m_pInstance;
    static CNetManager *GetInstance();
    
    void OnSocketRecieveData( int len, const char* buffer );
    
    void willDisconnectWithError();

    void onSocketDidDisconnect();

    void onSocketDidSecure();
    
    void sendMessage( char* msg, int len );
    
    void onConnectToHost( const char *Ip, int port );
    
    void connectServer( char *Ip, int port );
    
    void disConnect();
    
    bool isConnect();

};