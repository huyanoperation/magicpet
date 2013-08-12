#pragma once
#include "GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"

class CFeedScorllLayer;

enum
{
	CCFEEDLAYER_BUTTON_LEFT,
	CCFEEDLAYER_BUTTON_RIGHT,
	CCFEEDLAYER_BUTTON_BACK,
    CCFEEDLAYER_BUTTON_ADDFEED
};

class CCFeedLayer : public CCXMLLayer
{
public:
	CCFeedLayer();
	~CCFeedLayer();
    
	virtual void onEnter();
	virtual void onExit();	

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	void menuCallback(CCObject * pSender);
	void feedMenuCallback(CCObject * pSender);
	void OnItemSelect( int ItemId );
	void ShowCanFeedButton();
    void close();
    void UpData(ccTime tt);
    
    void ItemCompleteYes( CCNode* pNode );
    void ItemCompleteNo( CCNode* pNode );
    
    void AddFeedYes( CCNode* pNode );

	CCNode *m_pItemLogGroup;
	CFeedScorllLayer *m_pScrollLayer;

	CCMenu *m_pFeedMenu[4];

	CCMenu *m_pFeedControlPoint[3];

	int m_iCurItemId;
    
    CCLabelTTF *m_pLabelFeedCount;
    
};
