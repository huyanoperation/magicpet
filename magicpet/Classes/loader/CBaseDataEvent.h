/*
 *  CBaseDataEvent.h.h
 *  game
 *
 *  Created by huyan on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "cocos2d.h"

#define MAX_SUBEVENT_ID_INPOINT 16
#define MAX_FUN_COUNT_INEVENT 16

typedef struct __SSubEvent
{
	unsigned short		 m_wId;
	unsigned short		 m_wSubEventLogID;
	unsigned short		 m_wSubEventNameID;
           						
	unsigned short		 m_wLimitCount;
	unsigned short       m_iLimitId[MAX_FUN_COUNT_INEVENT];

	unsigned short		 m_wEventRunCount;
	unsigned short       m_iFunctionRun[MAX_FUN_COUNT_INEVENT];

	string				 eventicon;

}SSubEvent;


typedef struct __SEventPoint
{
	unsigned short					m_wPointId;
	unsigned short					m_wSubEventCount;
	unsigned short					m_iSubEventId[MAX_SUBEVENT_ID_INPOINT];

	unsigned short					m_iPointLimitCount;
	unsigned short					m_iPointLimitArray[MAX_FUN_COUNT_INEVENT];
	
	unsigned short					m_iPointRunCount;
	unsigned short					m_iPointRunArray[MAX_FUN_COUNT_INEVENT];
}SEventPoint;

typedef struct __SEventLimit
{
	unsigned short					m_wId;
	unsigned short					*m_wParam;
	unsigned short					m_iOperator;
	unsigned short					m_wLimtDataCount;
}SEventLimit;
//====================================================================================
//
//
typedef struct __SEventRun
{
	unsigned short					m_wId;
	unsigned short					*m_wParam;
	unsigned short					m_iOperator;
	unsigned short					m_wCmdDataCount;
}SEventRun;


class CBaseDataEvent
{
	
public:
	CBaseDataEvent();
	~CBaseDataEvent();

	SEventPoint		m_EventPointInfo;
	SSubEvent		m_SubEvent;
	//SEventLimit		*m_pEventLimit;
	//SEventRun		*m_pEventRun;
	
	int GeId(){ return m_SubEvent.m_wId; }


};