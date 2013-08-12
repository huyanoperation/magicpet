//
//  CMessageBoxYes.h
//  magicpet
//
//  Created by jia huyan on 12-3-8.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once
#include "../GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"
#include "../CGameText.h"

#define MAX_LETTER_LINES 5

struct SGameMessage
{
	int messageType;
	string strTime;
	string strMessage;
    string name;
};

class CLetterMoveLayer : public CCLayer
{
    
public:
	CLetterMoveLayer();
	~CLetterMoveLayer();
    
	void visit();
	void preVisitWithClippingRect(CCRect clipRect);
	void postVisit();
    
};

class CGameLetter : public CCXMLLayer
{
public:
	CGameLetter();
	~CGameLetter();
    
	virtual void onEnter();
	virtual void onExit();
    
	void menuCallback(CCObject * pSender);
    
    void weiboCallback(CCObject * pSender);
    void pushBack(CCObject * pSender);
    
	void UpData( ccTime tt );

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	vector< SGameMessage > m_vMessages; 
	void AddMessage( string messgeTime, string message, int MessageType = 0, string name = "" );

	CCNode* m_pGroupNode;
    int m_iEachSizeDis;
    int m_iPushBackPlayerId;
    
    CCRenderTexture *m_pTarget;
};
