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

enum
{
	LOADING_CGAME_SAVE,
    LOADING_CGAMEWORLD_SCENE,
    LOADING_CGAME_CREAT_ROLE_SCENE,
    LOADING_CGAME_MINI_GAME_ACCESSFOOD,
	LOADING_CGAME_MINI_GAME_GAMBLING,
	LOADING_CGAME_MINI_BIT_WOOD,
    LOADING_CGAME_FIGHT,
};

static const char* s_LoadingLeveFile = "loading.plist";

class CGameLoading : public CGameScene
{
    
public:
	CGameLoading();
	~CGameLoading();
    
	static void Show( int screen );
    static int   m_iCurScene;
	virtual void onEnter();
	virtual void onExit();
    
    void nextScreen( ccTime tt );
    
    void menuCallback(CCObject * pSender);

};

