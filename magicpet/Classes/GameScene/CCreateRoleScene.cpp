//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011ƒÍ µ¿∑Êª•∂Ø£®±±æ©£©ø∆ºº”–œﬁ‘»Œπ´Àæ. All rights reserved.
//
#include "CCreateRoleScene.h"
#include "CCXMLLayer.h"
#include "CGameWorldScene.h"
#include "CGameMenu.h"
#include "CGameLoading.h"
#include "../Sprite/CPet.h"
#include "../Inputbox/InputTextLayer.h"
#include "../sql/CSqliteManager.h"
#include "../Pushui/CMessageBoxYes.h"
#include "../Platform/CCNd91Mng.h"
#include "../Net/CurlTest.h"

CCreateRoleScene::CCreateRoleScene()
{
}
//-------------------------------------
//
//
CCreateRoleScene::~CCreateRoleScene()
{

}
//button_creat1 button_creat2 button_creat3 
//button_delete button_stargame button_back 
//button_achievement button_battlelog button_shopping button_system 
//-------------------------------------
//
//
void CCreateRoleScene::Show()
{
	CCreateRoleScene* ps = new CCreateRoleScene();
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
//------------------------------------------
//
//
void CCreateRoleScene::SetButtonEnable( bool isEnable )
{
	//m_pMenu[4]
}
//-------------------------------------
//
//
void CCreateRoleScene::onEnter()
{
	CGameScene::onEnter();

	CCXMLLayer *pXMLLayer = new CCXMLLayer();
	pXMLLayer->init();
	pXMLLayer->autorelease();
	//string strPath = GetGameLevelPath();
	//strPath = strPath + s_createroleLeveFile;
	pXMLLayer->LoadPlist( s_createroleLeveFile );
	addChild( pXMLLayer );
    
    m_pXMLLayer = pXMLLayer;
    const char *MenuButton[] =
	{
		"button_stargame", "button_back" ,"button_attribute","button_petname"
	};
	for( int i = 0; i < 4; i++ )
	{
		CCSprite *pSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CCreateRoleScene::menuCallback), i, this, pSprite->getPosition() );
		pXMLLayer->RemoveNode( pSprite );
		pXMLLayer->addChild( pMenu, pSprite->getZOrder() );
	}
	CCNode *pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_createrpetshow" );
	pNode->getPosition();
	m_pPet = new CPet();
	m_pPet->autorelease();
	m_pPet->setPosition( pNode->getPosition() );
	addChild( m_pPet );

	m_iRandomPetId = 1 + rand()%10;
	//
	CBaseDataPet *pPetData = g_pClientLoader->GetBaseDataPetFromId( m_iRandomPetId );
	//
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetData->animationid);
    m_pBaseDataAnimation = pAnimationData; 
	m_pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
	m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	memcpy( g_pPetDataBlock->rbg, pPetData->rgb, sizeof( int ) * 3 );

	g_pPetDataBlock->petid = m_iRandomPetId;
	g_pPetDataBlock->ResId = pPetData->animationid;

	pNode = pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petname" );
	m_iInputPos = pNode->getPosition();

	if( g_pPetDataBlock->petName == "0" )
	{
		m_plabelName = CCLabelTTF::labelWithString("  ",  kFontSystem[FONT_MED].fontName,kFontSystem[FONT_MED].fontSize);
		addChild(m_plabelName,100);
		m_plabelName->setIsVisible( false );
        m_plabelName->setColor( ccBlack );
	}
	else
	{
		m_plabelName = CCLabelTTF::labelWithString(g_pPetDataBlock->petName.c_str(),  kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize);
		addChild(m_plabelName, 100);
		m_plabelName->setIsVisible( true );
         m_plabelName->setColor( ccBlack );
	}
	m_plabelName->setPosition( m_iInputPos );

	//straptitude intaptitude dexaptitude skillaptitude 
	const char* SceneObject[] = { "t2dSceneObject_straptitude", "t2dSceneObject_intaptitude", "t2dSceneObject_dexaptitude", "t2dSceneObject_skillaptitude"  };
	int PetData[] = { pPetData->straptitude, pPetData->intaptitude, pPetData->dexaptitude, pPetData->skillaptitude };
	for( int i = 0; i < 4; i++ )
	{
		char buffer[128];
		sprintf( buffer, "%d",PetData[i] );
		m_pLabelText[i] = CCLabelTTF::labelWithString(buffer,  kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_SMALL].fontSize);
		pNode = pXMLLayer->GetXMLNodeFromKey( SceneObject[i] );
		m_pLabelText[i]->setPosition( pNode->getPosition() );
		addChild( m_pLabelText[i], 100 );
	}

	m_pPet->setPetDataBlock( g_pPetDataBlock ); 

//	CInputTextLayer *InputText = new CInputTextLayer();
//	InputText->init();
//	addChild( InputText, 100, TAG_INPUTTEXT_OBJECT );
//	InputText->release();
//	//InputText->setPosition( m_iInputPos );
//	InputText->SetFunction( this, callfuncND_selector( CCreateRoleScene::onInPutNameCallBack ) );
    
	m_plabelName->setIsVisible( false );
    
    g_pPetDataBlock->playershoppingmallmoney = 50;
    g_pPetDataBlock->petmoney = 99999;
    g_pPetDataBlock->maxstaminapoint = 100;
    

}
//-----------------------------------------
//
//
void CCreateRoleScene::onInPutNameCallBack( CCNode *pNode, void* d )
{
	m_plabelName->setIsVisible( true );
	char *buffer = ( char* ) d;
	m_plabelName->setString( buffer );
	g_pPetDataBlock->petName = buffer;
	removeChildByTag( TAG_INPUTTEXT_OBJECT, true );
    
    CSqliteManager::GetInstance()->SaveSql();
    if( !g_pPetDataBlock->recordsave.isNotBeginner[0] )
    {
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_1[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
        
    }
}
//-------------------------------------
//18729187981
//
void CCreateRoleScene::onExit()
{
    if( g_iGameMode )
    {
        string Myinfo =  CNd91Mng::NdGetMyInfo();
        if( !Myinfo.empty() )
        {
            char Buffer1[32];
            char Buffer2[32];
            CSqliteManager::GetInstance()->getUnit(Myinfo.c_str(), 0, ",", Buffer1, 32);
            CSqliteManager::GetInstance()->getUnit(Myinfo.c_str(), 1, ",", Buffer2, 32);
            g_pPetDataBlock->playername = Buffer2;
            g_pPetDataBlock->userid = Buffer1;
            
//            string format = "{\"playerid\":\"%s\",\"playershoppingmallmoney\":\"0\",\"petid\":\"%d\",\"petname\":\"%s\",\"petsex\":\"0\",\"pettitle\":\"0\",\"petlevel\":\"%d\",\"petexp\":\"0\",\"maxstaminapoint\":\"0\",\"petturning\":\"0\",\"petskillexppond\":\"0\",\"itemnum\":\"0\",\"petmoney\":\"0\",\"aechievepoint\":\"0\",\"aechieveflag\":\"0\",\"skilllevel\":\"0\",\"skillexp\":\"0\",\"gametime\":\"0\",\"feed\":\"0\",\"feedsave\":\"0\",\"gameday\":\"0\"}";
//        
//            char buffer[1024];
//            sprintf( buffer, format.c_str(), Buffer1, g_pPetDataBlock->petid, g_pPetDataBlock->petName.c_str(), g_pPetDataBlock->petlevel );
//            CurlTest::ShareInstance()->AddSelfData( buffer );
//            
//            CCLOG( buffer);
        
        }
    }
    
	CCScene::onExit();
}
//-------------------------------------
//
//
void CCreateRoleScene::menuCallback(CCObject * pSender)
{
    int tag = ((CCNode*)( pSender ))->getTag();
    
    switch(tag)
    {
        case CCREATEROLESCENE_BUTTON_STARGAME:
        {
            CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
        }
        break;
        case CCREATEROLESCENE_BUTTON_BACK:
        {
            CGameMenu::Show();
        }
        break;
		case CCREATEROLESCENE_BUTTON_ATTRIBUTE:
		{
            CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
            
			m_iRandomPetId = 1 + rand()%10;

			CBaseDataPet *pPetData = g_pClientLoader->GetBaseDataPetFromId( m_iRandomPetId );
	//
			CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetData->animationid);
			m_pBaseDataAnimation = pAnimationData; 
			m_pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
			m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
			g_pPetDataBlock->petid = m_iRandomPetId;
			g_pPetDataBlock->ResId = pPetData->animationid;
			memcpy( g_pPetDataBlock->rbg, pPetData->rgb, sizeof( int ) * 3 );
			ccColor3B color={pPetData->rgb[0],pPetData->rgb[1],pPetData->rgb[2]};
			m_pPet->setColor( color );

			m_pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
			m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);

				//straptitude intaptitude dexaptitude skillaptitude 
			const char* SceneObject[] = { "t2dSceneObject_straptitude", "t2dSceneObject_intaptitude", "t2dSceneObject_dexaptitude", "t2dSceneObject_skillaptitude"  };
			int PetData[] = { pPetData->straptitude, pPetData->intaptitude, pPetData->dexaptitude, pPetData->skillaptitude };
			for( int i = 0; i < 4; i++ )
			{
				char buffer[128];
				sprintf( buffer, "%d",PetData[i] );
				//m_pLabelText[i] = CCLabelTTF::labelWithString(buffer,  kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_SMALL].fontSize);
				//pNode = pXMLLayer->GetXMLNodeFromKey( SceneObject[i] );
				//m_pLabelText[i]->setPosition( pNode->getPosition() );
				m_pLabelText[i]->setString( buffer );
			}

			m_pPet->setPetDataBlock( g_pPetDataBlock );
            
            CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_1");
            pSprite->setIsVisible(false);
		}
		break;
		case CCREATEROLESCENE_BUTTON_PETNAME:
		{
			removeChildByTag( TAG_INPUTTEXT_OBJECT, true );

//			CInputTextLayer *InputText = new CInputTextLayer();
//			InputText->init();
//			addChild( InputText, 100, TAG_INPUTTEXT_OBJECT );
//			InputText->release();
//			//InputText->setPosition( m_iInputPos );
//			InputText->SetFunction( this, callfuncND_selector( CCreateRoleScene::onInPutNameCallBack ) );

			m_plabelName->setIsVisible( false );
		}
		break;
    }

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/buttton.mp3", false);
	}
}