//
//  CMessageBoxYes.h
//  magicpet
//
//  Created by jia huyan on 12-3-8.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once
#include "GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"
#include "../CGameText.h"

class CMessageBoxYesNo : public CCXMLLayer
{
public:
	CMessageBoxYesNo( char *Message, SEL_CallFuncN pSelectorYes, SEL_CallFuncN pSelectorNo,SelectorProtocol *pTarget,int tag = 0, bool isSelfDelete = false );
	CMessageBoxYesNo();
    
	virtual void onEnter();
	virtual void onExit();
    
	void menuCallback(CCObject * pSender);
	void UpData( ccTime tt );
    
    char m_szBuffer[1024];
    char m_TitleBuffer[128];
    
    //Callback funciton pointer.
    SEL_CallFuncN m_pfnSelectorDelegateYes;
    SEL_CallFuncN m_pfnSelectorDelegateNo;
    SelectorProtocol *m_pTarget;
    
    static bool m_bIsSelfDelete;
    
    
};
