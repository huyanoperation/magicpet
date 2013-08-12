/*
 *  CBaseFunRunData.h
 *  game
 *
 *  Created by huyan on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#define MAX_FUN_NAME_NUM 32
#define MAX_FUN_PARAM_NUM 8 

#include "cocos2d.h"

enum
{
	FIND_TYPEID_FUN_NAME,
	FIND_TYPEID_FUN_OPERATOR
};

class CBaseFunRunData
{
	
public:
	CBaseFunRunData();
	~CBaseFunRunData();
	
	unsigned short GetFunId(){ return m_iFunId; }
	unsigned short GetOperator(){ return m_iOperator; }
	unsigned short GetTargetNum(){ return m_iTagetNum; }
	unsigned short FindIdFormName( short type, const char* ch);

	unsigned short m_iID;
	unsigned short m_iFunId;
	unsigned short m_iOperator;
	unsigned short m_iTagetNum;
	unsigned short m_arrParam[MAX_FUN_PARAM_NUM];
	string		   m_strIcon;
};