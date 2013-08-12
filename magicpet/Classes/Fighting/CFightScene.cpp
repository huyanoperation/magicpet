/*****************************************************************************/
//File name:	CFightScene
//Author: 		huyan
//Description:	fight¡¢
/******************************************************************************/
#include "CFightScene.h"
#include "CFightGroundLayer.h"

CFightScene::CFightScene()
{

}
//--------------------------------------------------------------------------------
//
//
CFightScene::~CFightScene()
{
	
}
//------------------------------------------------------------------------------------
//
//
void CFightScene::onEnter()
{
	char szBuffer[256];
	memset( szBuffer,0,256 );
	CGameScene::onEnter();
	CFightGroundLayer * pLayer = new CFightGroundLayer();
	pLayer->init();
	sprintf( szBuffer, "%s%s",GetGameLevelPath().c_str(), "fightLevel.plist" );
	pLayer->LoadPlist( "fightLevel.plist" );
	pLayer->CreatPlayer();
	pLayer->autorelease();
	
	addChild( pLayer );
}
//---------------------------------------------------------------------------------
//
//
void CFightScene::Show()
{
	CFightScene* ps = new CFightScene();
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
}
//------------------------------------------------------------------------------------
//
//
void CFightScene::onExit()
{
	CCScene::onExit();
}
