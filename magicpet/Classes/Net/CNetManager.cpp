//
//  CNetManager.cpp
//  magicpet
//
//  Created by jia huyan on 11-11-24.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include "CNetManager.h"
#include <iostream>
#include "../platform/CNet.h"
#include "../CChineseCode.h"

CNetManager *CNetManager::m_pInstance = NULL;

CNetManager::CNetManager()
{
    
}
//-------------------
//
//
CNetManager::~CNetManager()
{
    
}
//-------------------
//
//
CNetManager *CNetManager::GetInstance()
{
    if( NULL == m_pInstance )
    {
        m_pInstance = new CNetManager();
    }
    return m_pInstance; 
}
//----------------------------------------------------
//
//
void CNetManager::sendMessage( char* msg, int len )
{
    CThreadCommand1 thread;
    thread.m_dwSize = len;
    memmove( thread.m_Data, msg, len);
    CNet::sendMsg( (char*)&thread, thread.GetSize() );
    
}
//-----------------------------------------------------
//
//
void CNetManager::connectServer( char *Ip, int port )
{
    CNet::connectServer(Ip, port);
}
//----------------------------------------------------
//
//
void CNetManager::onConnectToHost( const char *Ip, int port )
{
    //CThreadCommand1 info;
    //PT_GET_BONUS_LIST_CHECK_INFO infoData;

    //infoData.dwUserId = 10;
    //infoData.dwRoomId = 11;
    //infoData.dwGameId = 12;
    
    //info.m_dwSize = sizeof( PT_GET_BONUS_LIST_CHECK_INFO );
    //memmove( info.m_Data, &infoData, sizeof( PT_GET_BONUS_LIST_CHECK_INFO ));
    //CNet::sendMsg( (char*)&info, info.GetSize() );
    
    PT_GET_BONUS_LIST_CHECK_INFO InfoData;
    
    InfoData.dwUserId = 10;
    InfoData.dwRoomId = 11;
    InfoData.dwGameId = 12;
	
	strcpy( InfoData.chName, "魏征" );
    
   // CChineseCode::UTF_8ToUnicode( InfoData.chName, "魏征" );
    sendMessage( (char*)&InfoData,sizeof( PT_GET_BONUS_LIST_CHECK_INFO ) );
}
//-----------------------------------------------------
//
//
void CNetManager::OnSocketRecieveData(int len, const char* buffer)
{
	short type;
	memmove( &type, &buffer[4], 2 );
    
    PT_GET_BONUS_LIST_CHECK_INFO infodata;
	memmove( &infodata, &buffer[4], sizeof( PT_GET_BONUS_LIST_CHECK_INFO ) );
    
    //char name[32];
    //CChineseCode::UnicodeToUTF_8( name, infodata.chName);

}
//-----------------------------------------------------
//
//
void CNetManager::disConnect()
{
    CNet::disConnect();
}
//----------------------------------------------------
//
//
bool CNetManager::isConnect()
{
    return CNet::isConnect();
}
//-----------------------------------------------------
//
//
void CNetManager::willDisconnectWithError()
{
     int c = 0;
}
//-----------------------------------------------------
//
//
void CNetManager::onSocketDidDisconnect()
{
     int c = 0;
}
//-----------------------------------------------------
//
//
void CNetManager::onSocketDidSecure()
{
     int c = 0;
}
