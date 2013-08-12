//
//  CGameMenu.h
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011ƒÍ µ¿∑Êª•∂Ø£®±±æ©£©ø∆ºº”–œﬁ‘»Œπ´Àæ. All rights reserved.
//

#include "CGameLoading.h"
#include "CGameMenu.h"
#include "CGameWorldScene.h"
#include "CCreateRoleScene.h"
#include "../CCXMLLayer.h"
#include "../MiniGame/CMiniGameScene.h"
#include "../Fighting/CSelectEnemyScene.h"
#include "../sql/CSqliteManager.h"
#include "CSaveSence.h"
#include "../CGameText.h"

int CGameLoading::m_iCurScene = 0;

CGameLoading::CGameLoading()
{

}
//-----------------------------------------------------------
//
//
CGameLoading::~CGameLoading()
{

}
//------------------------------------------------------------
//
//
void CGameLoading::Show( int screen )
{
    CGameLoading* ps = new CGameLoading();
	ps->init();
	ps->autorelease();
    
    m_iCurScene = screen;
    
	if ( CCDirector::sharedDirector()->getRunningScene())
	{
		CCDirector::sharedDirector()->replaceScene( ps );
	}
	else
	{
		CCDirector::sharedDirector()->runWithScene( ps );
	}
}
//-------------------------------------------------------------
//
//
void CGameLoading::onEnter()
{
	CCScene::onEnter();

	CCDirector::sharedDirector()->purgeCachedData();
    
    CCScene::onEnter();
    CCXMLLayer *pXMLLayer = new CCXMLLayer();
	pXMLLayer->init();
	pXMLLayer->autorelease();

	pXMLLayer->LoadPlist( s_LoadingLeveFile );
	addChild( pXMLLayer );
    
    schedule(schedule_selector(CGameLoading::nextScreen),2.0f);
    
    const char* GameTips[] = { gametip_1[g_iCurLanguageIndex],gametip_2[g_iCurLanguageIndex], gametip_3[g_iCurLanguageIndex],gametip_4[g_iCurLanguageIndex],gametip_5[g_iCurLanguageIndex],gametip_6[g_iCurLanguageIndex],gametip_7[g_iCurLanguageIndex],gametip_8[g_iCurLanguageIndex],gametip_9[g_iCurLanguageIndex],gametip_10[g_iCurLanguageIndex],gametip_11[g_iCurLanguageIndex]};

	int index = rand()%11;
	string utf8;
	CChineseCode::GB2312ToUTF_8( utf8, (char*)GameTips[index], 256 );

    CCNode *pNode = pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_tip" );
	CCLabelTTF *pLabel = CCLabelTTF::labelWithString( utf8.c_str(), kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize );
    pLabel->setPosition( pNode->getPosition() );
    CCRenderTexture *pstroke = createStroke(pLabel, 1, kFontSystem[FONT_MED].strokeColor);
    addChild( pstroke );
    addChild( pLabel );

}
//--------------------------------------------------------------
//
//
void CGameLoading::nextScreen( ccTime tt )
{
    CSqliteManager::GetInstance()->SaveSql();
    switch( m_iCurScene )
    {
        case LOADING_CGAMEWORLD_SCENE:
        {
            CGameWorldScene::Show();
        }
        break;
        case LOADING_CGAME_CREAT_ROLE_SCENE:
        {
            CCreateRoleScene::Show();
        }
        break;
        case LOADING_CGAME_MINI_GAME_ACCESSFOOD:
        {
            CMiniGameScene::Show( MINI_GAME_1_ACCESSFOOD );
        }
		break;
		case LOADING_CGAME_MINI_GAME_GAMBLING:
		{
			 CMiniGameScene::Show( MINI_GAME_2 );
		}
        break;
		case LOADING_CGAME_MINI_BIT_WOOD:
		{
			CMiniGameScene::Show( MINI_GAME_3 );
		}
        break;
        case LOADING_CGAME_FIGHT:
        {
            CSelectEnemyScene::Show();
        }
		break;
		case LOADING_CGAME_SAVE:
		{
			CSaveSence::Show();
		}
		break;

    }

}
//------------------------------------------------------------
//
//
void CGameLoading::onExit()
{
	CCScene::onExit();
}