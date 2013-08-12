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
#include "../CCXMLLayer.h"

enum
{
	CGAMEMENU_BUTTON_ONLINE,
	CGAMEMENU_BUTTON_OFFLINE,
	CGAMEMENU_BUTTON_OPTION,
	CGAMEMENU_BUTTON_ABOUT,
    CGAMEMENU_BUTTON_91
};

class CAbout : public CCXMLLayer
{
public:
	CAbout();
	~CAbout();

	void onEnter();
	void onExit();
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};


enum
{
	COPTION_EXIT,
	COPTION_DELSAVE
};

class COption : public CCXMLLayer
{
public:
	COption();
	~COption();

	void onEnter();
	void onExit();
	void menuCallback( CCObject * pSender );
	void menuSound( CCObject * pSender );
	void UpData( ccTime tt );

	CCMenu *m_pControlMenu[4];
	//virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	//virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};

class CGameMenu : public CGameScene
{
    
public:
	CGameMenu();
	~CGameMenu();
    
	static void Show();
	virtual void onEnter();
	virtual void onExit();

	void EnableButton( bool enable );
    
    void menuCallback(CCObject * pSender);

	CCMenu *m_pMenu[8];
};

