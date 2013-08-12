/*****************************************************************************/
//File name:	CTaskEventSubGui.h
//Author: 		huyan
//Description:	任务界面、
/******************************************************************************/
#pragma once

#include "../GameConstant.h"
#include "../CCXMLLayer.h"


class CTaskMoveLayer : public CCLayer
{

public:
	CTaskMoveLayer();
	~CTaskMoveLayer();

	void visit();
	void preVisitWithClippingRect(CCRect clipRect);
	void postVisit();

};

struct STaskEvent
{
	int eventId;
	string eventLog;
	string eventIcon;
	int targetNum;
	string RunIcon;
};

class CTaskEventSubGui : public CCXMLLayer
{

public:
	CTaskEventSubGui();
	~CTaskEventSubGui();

	void onEnter();
	void onExit();

	void menuCallback(CCObject * pSender);

	vector<STaskEvent> m_vTaskEvent;

	CCNode *m_pGroupNode;

	CCSprite *m_pUpScorl;
	CCSprite *m_pDownScorl;

	int m_iEachSizeDis;
    
    int m_iEachSizeHeight;

	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

};
