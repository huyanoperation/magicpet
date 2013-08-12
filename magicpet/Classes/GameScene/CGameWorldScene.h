//
//  CGameMenu.h
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once
#include "GameConstant.h"
#include "CGameScene.h"
#include "../Sprite/CPet.h"

const char s_WorldLeveFile[] = "ingame.plist";

enum
{
	CGAMEWORLDSCENE_BUTTON_ACHIEVEMENT, 
	CGAMEWORLDSCENE_BUTTON_BATTLELOG, 
	CGAMEWORLDSCENE_BUTTON_SHOPPING,
    CGAMEWORLDSCENE_BUTTON_SYSTEM,
    
    CGAMEWORLDSCENE_BUTTON_BUTTON_GAMEROOM,
    CGAMEWORLDSCENE_BUTTON_BUTTON_GAMBLINGROOM, 
    CGAMEWORLDSCENE_BUTTON_BUTTON_FIGHT, 
    CGAMEWORLDSCENE_BUTTON_BUTTON_ACCESSFOOD,
    CGAMEWORLDSCENE_BUTTON_BUTTON_HITSTAKES, 
    CGAMEWORLDSCENE_BUTTON_BUTTON_SLOTMACHINE,
	CGAMEWORLDSCENE_BUTTON_BATTLE,
	CGAMEWORLDSCENE_BUTTON_EAT,
    CGAMEWORLDSCENE_BUTTON_FRIEND_BATTLE,
	CGAMEWORLDSCENE_BUTTON_PETSTAMINAPOINT,
};

enum
{
	CGAMEWORLDSCENE_PUSH_ATTRIBUTE = 1,
};

#define NET_PLAYER_LOADING_TAG 1001

#define CPETWORLDLAYER_TAG 1
#define ACTION_EVENTBUTTONJUMP 10
#define ACTION_LETTERBUTTONJUMP 11
#define ACTION_SH0P_JUMP 12
#define ACTION_JINGLI_JUMP 13

#define MINIGAME_SUB_MOENY 100
#define MINIGAME_SUB_STAMINAPOINT 5

#define FIGHT_SUB_MONEY 100
#define FIGHT_SUB_STAMINAPOINT 10

class CCXMLLayer;


class CGameWorldScene : public CGameScene
{
    
public:
	CGameWorldScene();
	~CGameWorldScene();
    
	static void Show( int turnpetid = 0 );
	virtual void onEnter();
	virtual void onExit();
    void scheduleShowGameRoom( ccTime tt );
    void scheduleShowGamblingroom( ccTime tt );
    void UpData( ccTime tt );

	void EnableAllWorldButton( bool isEnable );
    
    void menuCallback(CCObject * pSender);
    
    void cheatMenuCallBack(CCObject * pSender);
	void SaveItemRecord( int itemID );
    void SetTurnPetId( int Id );
	void AddstaminapointYes( CCNode *pNode );
    
    void TurnPetCallback();

	static int s_iPetAniId;

	CCXMLLayer *m_pXMLLayer;

	CPet *m_pPet;
    int m_iTurnId;

	time_t m_iGameBeginTime;
	CCLabelTTF *m_pPlayerMoney;
	CCLabelTTF *m_pPetMoney;
	CCLabelTTF *m_pMaxstaminapoint; 

	CCMenu *m_pEventButton;
	CCMenu *m_pLetterButton;
    CCMenu *m_pShop;
    CCMenu *m_petstaminapoint;

	static AdvanceSprite *m_pAdvanceSprite;
    bool m_bIsShopJump;
    bool m_bIsJingliJump;
    
    static void OnUserCallBack( string uin, string name );
    static void OnUserFinishCallBack();
    
    
};

extern CGameWorldScene* g_pGameWorldScene;

class CPetWorldLayer : public CCLayer
{
public:
	CPetWorldLayer();
	~CPetWorldLayer();

	static void Show();
	virtual void onEnter();

	void PetAiSchedule( ccTime tt );

	//virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	//virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

	void animationEndCallBank();

	void PetMoveEndCallBank( CCNode *pNode, void* d );

	void SetPetPosNode( CCNode *pNode );

	CPet *m_pPet;

	CCNode *m_pPetPosNode;

	bool m_bIsMove; 

};
