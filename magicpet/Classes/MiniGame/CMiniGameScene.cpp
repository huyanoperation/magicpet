//
//  CMiniGameScene
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once
#include "CMiniGameScene.h"
#include "CMiniGameCatchLayer.h"
#include "CMiniGameGamblingLayer.h"
#include "CMiniGameBitWood.h"

CMiniGameScene::CMiniGameScene()
{

}
//----------------------------------------------------
//
//
CMiniGameScene::~CMiniGameScene()
{

}
//------------------------------------------------------
//
//
void CMiniGameScene::Show( int MiniGameID )
{
	CMiniGameScene* ps = new CMiniGameScene();
	ps->init();
	ps->autorelease();

	if ( CCDirector::sharedDirector()->getRunningScene())
	{
		CCDirector::sharedDirector()->replaceScene( ps );
	}
	else
	{
		CCDirector::sharedDirector()->runWithScene( ps );
	}

	CCLayer *pLayer = ps->CreatMiniGame( MiniGameID );
	ps->addChild( pLayer );
}
//-------------------------------------------------------
//
//
CCLayer* CMiniGameScene::CreatMiniGame( int MiniGameID )
{
	CCLayer *pLayer = NULL;
	switch( MiniGameID )
	{
	case MINI_GAME_1_ACCESSFOOD:
        {
            pLayer = new CMiniGameCatchLayer();
            pLayer->autorelease(); 
        }
		break;
	case MINI_GAME_2:
		{
			pLayer = new CMiniGameGamblingLayer();
			pLayer->autorelease();
		}
		break;
	case MINI_GAME_3:
			pLayer = new CMiniGameBitWood();
			pLayer->autorelease();
		break;
	case MINI_GAME_4:
		break;
	}
	return pLayer;
}
//---------------------------------------------------
//
//
void CMiniGameScene::onEnter()
{
	CGameScene::onEnter();
}
//-----------------------------------------------------
//
//
void CMiniGameScene::onExit()
{
	CCScene::onExit();
}
//-----------------------------------------------------
//
//
void CMiniGameScene::menuCallback(CCObject * pSender)
{
	
}

