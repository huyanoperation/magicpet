#pragma once
#include "GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"
#include "../loader/CCLientLoader.h"
#include "../Sprite/CPet.h"

enum
{
	TAG_BEGIN_WOOD_SPRITE = 1000
};
#define ACTION_TAG_ATTACK_WOOD 10

class CWood : public AdvanceSprite
{
public:
	CWood();
	~CWood();

	void onEnter();
	void onExit();

	void scheduleDelete( ccTime tt );

	void SetTime( ccTime tt ) { m_tTime = tt;  }
	 
	ccTime m_tTime;
};

enum  
{
    CWOOD_KIND_1,
    CWOOD_KIND_2,
    CWOOD_KIND_3
};

class CMiniGameBitWood : public CCXMLLayer
{
public:
	CMiniGameBitWood();
	~CMiniGameBitWood();
    
	virtual void onEnter();
	virtual void onExit();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	void AddSprite();
    
    CPet *m_pPet;
	CBaseDataAnimation *m_pBaseDataAnimation;

	void menuCallback( CCObject *pObject );
	void AttackFinishCallBack();

	void UpData( ccTime tt );

	void MoveToAttack( CCPoint pos, float time, int tag );
	void AttackBeginCallBack( CCNode *pNode, void *d );
    void TimeUpdata( ccTime tt );

	void Finsh( ccTime tt );

	ccTime m_iTime;
	int m_iStartTime;
    int m_iTotalTime;
    int m_iMaxTime;
    
    CCSprite *m_pTimeSprite;
    float  m_iTimeBar;
    
    bool m_bIsAttackState;
	bool m_bIsFinish;
    
    CCLabelTTF *m_plabelName;
    int m_iHitCount;
    
    
};
