#pragma once
#include "GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"

enum
{
	CFEEDSCORLLLAYER_NODE_TAG = 1
};

struct SScorllFeedItem 
{
	int ItemId;
	int ItemCount;
	bool isActive;
	int leaveTime;
    int activeCount;
    CCLabelTTF *pLabel;
    CCLabelTTF *pLabelItemCount;
	CCMenu *pMenu;
};

#define CFEEDSCORLLLAYER_ITEM_BUTTON_TAG_BEGIN 1000

class CFeedScorllLayer : public CCLayer
{
public:
	CFeedScorllLayer();
	~CFeedScorllLayer();
    
	virtual void onEnter();
	virtual void onExit();

	void visit();
	void preVisitWithClippingRect(CCRect clipRect);
	void postVisit();
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	void menuCallback(CCObject * pSender);
	void ItemMenuCallBack( CCObject * pSender );
	void ItemCompleteCallBack( CCObject * pSender );
	void ItemCancelCallBack( CCObject * pSender );
	void onButtonMove( int act );
    void OnActiveItem( int ItemId );
    

	CCNode* m_pGroupNode;

	vector< SScorllFeedItem > m_vScorllFeedItem;

	void CollectItem();
	void AddItemToShop();

	int  m_iEachItemSize;
	int  m_iInitPos;

	CCMenu* m_pLeftPoint;
	CCMenu* m_pRightPoint;
    void UpData( ccTime tt );
    
    
    int CFEEDSCORLLLAYER_ITEM_EACH_DIS;
    
};
