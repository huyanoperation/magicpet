#pragma once
#include "GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"
#include "../Sprite/AdvanceSprite.h"

enum
{
    MINIGAME_GAMBLING_LAYER_BEGIN = 100
};


class CMiniGameGamblingLayer : public CCXMLLayer
{
public:
	CMiniGameGamblingLayer();
	~CMiniGameGamblingLayer();
    
	virtual void onEnter();
	virtual void onExit();

	//virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	//virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    void menuCallback(CCObject * pSender);

	void BeginRote( ccTime tt );

	void UpData( ccTime tt );
    void scheculePauseData( ccTime tt );

	bool m_bIsTouch;

	void TouchCallBack( CCNode *pNode, void *d );
	bool CheckIsWin();

    CCSprite *m_pPushBar;

	AdvanceSprite *m_pFood[3];

	CCSprite *m_pSpriteWin;
	CCSprite *m_pSpriteLost;

	CCLabelTTF *m_plabelMoney;
	CCLabelTTF *m_plabelWinMoney;
	CCLabelTTF *m_plabelLostMoney;
	int m_iWinMoney;
    int m_iRollIndex;

	int m_Code[3];
};