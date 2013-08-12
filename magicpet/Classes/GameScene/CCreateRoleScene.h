//
//  CGameMenu.h
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once
#include "GameConstant.h"
#include "../loader/CClientLoader.h"
#include "CGameScene.h"

enum  
{
    CCREATEROLESCENE_BUTTON_STARGAME,
    CCREATEROLESCENE_BUTTON_BACK,
	CCREATEROLESCENE_BUTTON_ATTRIBUTE,
	CCREATEROLESCENE_BUTTON_PETNAME
};

enum
{
	TAG_INPUTTEXT_OBJECT = 100
};

static const char s_createroleLeveFile[] = "createrole.plist";
class CPet;
class CCXMLLayer;

class CCreateRoleScene : public CGameScene
{
    
public:
	CCreateRoleScene();
	~CCreateRoleScene();
    
	static void Show();
	virtual void onEnter();
	virtual void onExit();
    
    void menuCallback(CCObject * pSender);

	CBaseDataAnimation *m_pBaseDataAnimation;

	CPet *m_pPet;

	int m_iRandomPetId;

	void onInPutNameCallBack( CCNode *pNode, void* d );

	CCPoint m_iInputPos;

	CCLabelTTF *m_plabelName;

	CCLabelTTF *m_pLabelText[4];

	void SetButtonEnable( bool isEnable );

	CCMenu *m_pMenu[4];
    
    CCXMLLayer *m_pXMLLayer;
};
