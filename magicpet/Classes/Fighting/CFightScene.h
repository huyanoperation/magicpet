/*****************************************************************************/
//File name:	CFightScene
//Author: 		huyan
//Description:	fight¡¢
/******************************************************************************/
#pragma once
#include "GameConstant.h"
#include "../GameScene/CGameScene.h"

class CFightScene : public CGameScene
{

public:
	CFightScene();
	~CFightScene();

	static void Show();
	virtual void onEnter();
	virtual void onExit();

    void menuCallback(CCObject * pSender);

};