/*****************************************************************************/
//File name:	CFightGroundLayer
//Author: 		huyan
//Description:	fight
/******************************************************************************/
#pragma once
#include "../GameConstant.h"
#include "../CCXMLLayer.h"
#include "../Sprite/CPet.h"

enum
{
	FIGHT_PLAYER_NONE,
	FIGHT_LEFT_PLAYER_POS,
	FIGHT_RIFHT_PLAYER_POS
};

class CFightGroundLayer : public CCXMLLayer
{
public:
	CFightGroundLayer();
	~CFightGroundLayer();

	virtual void onEnter();
	virtual void onExit();

	void upData( ccTime dt );

    void menuCallback(CCObject * pSender);
	
	void FightTurnOverCallBack( CCNode *object, void* d );
    void SheduleWin( ccTime tt );
    void ScheudleExit( ccTime tt );

	void CreatPlayer();
    
    void FightBegin();
    
    static int s_iWincount;

protected:
	int m_iFightTurnId;
	int m_iFightResult;

	CPet *m_pPetLeft;
	CPet *m_pPetRight;
/***********************hp&mp*****************************/
	CCSprite *m_pPlayer1Hp;
	CCSprite *m_pPlayer2Hp;

	CCSprite *m_pPlayer1Mp;
	CCSprite *m_pPlayer2Mp;

	int m_iMaxHpBar;
	int m_iMaxHpHeight;
	int m_iHpInitPos;

	int m_iMaxMpBar;
	int m_iMaxMpHeight;
	int m_iMpInitPos;

	bool m_bIsFightOver;
	bool m_bIsWin;
/*****************************************************/

	CCLabelTTF *m_pLeftPetName;
	CCLabelTTF *m_pRightPetName;
    
    AdvanceSprite *m_pBattlenow;
    
};
extern CFightGroundLayer *g_pFightGroundLayer;