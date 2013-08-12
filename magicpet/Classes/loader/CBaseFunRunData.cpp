/*
 *  CBaseFunRunData.cpp
 *  game
 *
 *  Created by huyan on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CBaseFunRunData.h"
#include "CBaseFunLimitData.h"
#include "CCString.h"


static const char* s_RunFunIndex[] =
{
	"default",
	"runexp",
	"runskillexp",
	"runtitle",
	"runpet",
	"runeventflag",
	"runskill",
    "runmoney",
    "runplayermoney"
};

static const char* s_RunFunOperator[] = 
{
	"default",
	"+",
	"-",
	"*",
	"!",
	"+%"
};

CBaseFunRunData::CBaseFunRunData()
{
	m_iFunId	= 0;
	m_iOperator = 0;
	m_iTagetNum	= 0;
}
//-------------------------------------------------------------
//
//
CBaseFunRunData::~CBaseFunRunData()
{
	
}
//----------------------------------------------------------------
//
//
unsigned short CBaseFunRunData::FindIdFormName( short type, const char* ch)
{
	if( FIND_TYPEID_FUN_NAME == type )
	{
		for( int i = 0; i < MAX_FUN_NAME_NUM; i++ )
		{
			 if( 0 == strcmp( ch, s_RunFunIndex[i] ) )
			 {
				return i;
			 }
		}
	}
	else if( FIND_TYPEID_FUN_OPERATOR == type )
	{
		for( int i = 0; i < MAX_FUN_NAME_NUM; i++ )
		{	
			 if( 0 == strcmp( ch, s_RunFunOperator[i] ) )
			 {
				return i;
			 }
		}
	}
	return 0;
}