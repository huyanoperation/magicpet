//
//  CMiniGameScene
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once
#include "GameConstant.h"
#include "Box2D/Box2D.h"
#include "../CCXMLLayer.h"

enum
{
    MINIGAME_LAYER_BEGIN = 100
};

enum  
{
    MINIGAME_PET_MOVE_NONE,
    MINIGAME_PET_MOVE_LEFT,
    MINIGAME_PET_MOVE_RIGHT,
};

class CPet;
class CBaseDataAnimation;

struct SMiniGameCatchLayer_ItemSave
{
	int itemIndex;
	int count;
};



class CMiniGameCatchLayer : public CCXMLLayer
{
public:
	CMiniGameCatchLayer();
	~CMiniGameCatchLayer();
    
	virtual void onEnter();
	virtual void onExit();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	b2World* world;
    
    void menuCallback(CCObject * pSender);
	void update(ccTime dt);
	void AddCartchSprite(CCPoint p);
	void AddControlPlayer();
	void UpDataCatch(ccTime dt);
    void UpData(ccTime dt);
    void UpDataTime( ccTime dt );
	void Finsh( ccTime dt );
	void HitCallBack();

    CPet *m_pPet;
    CBaseDataAnimation *m_pBaseDataAnimation;
    
    int m_iPetActionState;
    
    CCSprite *m_pTimeSprite;
    
    int m_iTime;
    int m_iMaxTime;
    int m_iTimeBar;
	bool m_bIsFinish;

	CCLabelTTF *m_plabelName;
	bool m_bIsHit;

	vector<SMiniGameCatchLayer_ItemSave> m_ItemSave;
    
};
