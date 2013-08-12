//
//  CGameMenu.cpp
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include "CGameMenu.h"
#include "../CCXMLLayer.h"
#include "../Fighting/CFightScene.h"
#include "../MiniGame/CMiniGameScene.h"
#include "CCreateRoleScene.h"
#include "CSaveSence.h"
#include "../CGameText.h"
#include "../Platform/CCNd91Mng.h"
#include "../Net/CurlTest.h"

static const char* s_MenuLeveFile= "Menu.plist";
#define MAX_GAME_MENU_NUM 6

int g_iGameMode = 1;

CGameMenu::CGameMenu()
{

}
//----------------------------------------------------
//
//
CGameMenu::~CGameMenu()
{
	
}
//---------------------------------------------------
//
//
void CGameMenu::Show()
{
	CGameMenu* ps = new CGameMenu();
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
//-----------------------------------------------------
//
//
void CGameMenu::onEnter()
{
    CNd91Mng::RemoveBanner();
    CNd91Mng::InitadBanner();
	CGameScene::onEnter();
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	CCXMLLayer *pXMLLayer = new CCXMLLayer();
	pXMLLayer->init();
	pXMLLayer->autorelease();
	pXMLLayer->LoadPlist( s_MenuLeveFile );
	addChild( pXMLLayer );

	const char *MenuButton[] =
	{
		"button_online",
		"button_offline",
		"button_option",
		"button_about",
        "button_91",
        "button_elves",
	};

	for( int i = 0; i < MAX_GAME_MENU_NUM; i++ )
	{
		CCSprite *pSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CGameMenu::menuCallback), i, this, pSprite->getPosition() );
		pXMLLayer->removeChild( pSprite, true );
		addChild( pMenu );

		m_pMenu[i] = pMenu;
	}

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "music" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("data/audio/menubg.mp3", true);
	}
	//string strPath = GetGameImagesPath();
	//strPath = strPath + "UI/r1-hd.png";
	//CCMenu* pMenu = CreatGameUIButton( strPath,"", menu_selector( CGameMenu::menuCallback), 10, this, ccp( s.width / 2, s.height / 2 ) );
	//addChild(pMenu);
	//CCLabelTTF *label = CCLabelTTF::labelWithString("play", "Arial", 16);
	//label->setPosition( pMenu->getPosition() );
	//addChild( label, 0 );
    
    std::string effectpath = GetGameParticlePath();
    std::string filename = effectpath + "Glitter.plist";
    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    //m_emitter->setPositionType( kCCPositionTypeRelative );
    pEmitter->setPosition( getPosition() );
    CCPoint pos = getPosition();
    addChild( pEmitter, 100 );

    
    effectpath = GetGameParticlePath();
    filename = effectpath + "leaf.plist";
    pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    //m_emitter->setPositionType( kCCPositionTypeRelative );
    pEmitter->setPosition( ccp( 0, s.height ));
    pos = getPosition();
    addChild( pEmitter, 100 );
    
    //CurlTest *pCurlTest = new CurlTest();
    //addChild(pCurlTest); 
}
//----------------------------------------------------
//
//
void CGameMenu::EnableButton( bool enable )
{
	for( int i = 0; i < MAX_GAME_MENU_NUM; i++ )
	{
		m_pMenu[i]->setIsTouchEnabled( enable );
	}
}
//------------------------------------------------------
//
//
void CGameMenu::onExit()
{
	CCScene::onExit();
}
//---------------------------------------------------------
//
//
void CGameMenu::menuCallback(CCObject * pSender)
{
	int tag = ((CCNode*)( pSender ))->getTag();

	switch( tag )
	{
	case CGAMEMENU_BUTTON_ONLINE:
		{
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            if( 0 == CDevice::CheckNet() )
            {
                CCMessageBox("网络貌似有问题！", "提示");
                return;
            }
#endif
            
			CSaveSence::Show();
            g_iGameMode = 1;
		}
		break;
	case CGAMEMENU_BUTTON_OFFLINE:
		{
			CSaveSence::Show();
            g_iGameMode = 0;
		}
		break;
	case CGAMEMENU_BUTTON_OPTION:
		{
			COption *poption = new COption();
			poption->autorelease();
			addChild( poption );

			EnableButton( false );
		}
		break;
	case CGAMEMENU_BUTTON_ABOUT:
		{
			CAbout *pabout = new CAbout();
			pabout->autorelease();
			addChild( pabout );

			EnableButton( false );

		}
		break;
    case CGAMEMENU_BUTTON_91:
        {
            CNd91Mng::enter91Platform();
        }
        break;
    case 5:
        {
            CNd91Mng::OpenUrl( "https://itunes.apple.com/cn/app/zhao-huan-chuan-shuo-zhi-jing/id594107545");
        }
        break;
	}

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/buttton.mp3", false);
	}
	//CMiniGameScene::Show();
}
//-------------------------------------------------------------
//
//
CAbout::CAbout()
{

}
//-------------------------------------------------------------------------------------
//
//
CAbout::~CAbout()
{

}
//---------------------------------------------------------------
//
//
void CAbout::onEnter()
{
	CCXMLLayer::onEnter();
	LoadPlist("about.plist");

	CCNode *pNode = GetXMLNodeFromKey( "t2dSceneObject_Message" );

	string utf8;
	CChineseCode::GB2312ToUTF_8( utf8, (char*)the_loginmain_designer_text[g_iCurLanguageIndex], 512 );

	CCLabelTTF* pLabel = CCLabelTTF::labelWithString( utf8.c_str(),CCSize( 0,0 ), CCTextAlignmentLeft, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
	CCRenderTexture* pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
	addChild( pstroke,100 );
	addChild( pLabel,100 );

	setIsTouchEnabled( true );

}
//---------------------------------------------------------------------
//
//
void CAbout::onExit()
{
	CCXMLLayer::onExit();
}
//-------------------------------------------------------------------------------
//
//
bool CAbout::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	return true;
}
//--------------------------------------------------------------------------
//
//
void CAbout::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	((CGameMenu*)getParent())->EnableButton( true );
	this->removeFromParentAndCleanup( true );
}


COption::COption()
{
}
//-------------------------------------------------------------
//
//
COption::~COption()
{

}
//--------------------------------------------------------------
//
//
void COption::onEnter()
{
	CCXMLLayer::onEnter();
	LoadPlist("option.plist");

	const char *MenuButton[] =
	{
        "button_back", "button_delsave", 
	};
    
	for( int i = 0; i < 2; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(COption::menuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
        pMenu->setTag( i );
        removeChild( pSprite, true );
	}

	const char *MenuButton2[] =
	{
        "button_close1", "button_close2", "button_open1", "button_open2" 
	};

	for( int i = 0; i < 4; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton2[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(COption::menuSound), i%2, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
        pMenu->setTag( i );
		pMenu->setIsVisible( false );
        removeChild( pSprite, true );

		m_pControlMenu[i] = pMenu; 
	}
	//CCUserDefault::sharedUserDefault()->setBoolForKey("music", true );
	//CCUserDefault::sharedUserDefault()->setBoolForKey("sound", true );
	
	schedule( schedule_selector( COption::UpData ), 0.1f  );
}
//--------------------------------------------------------------------
//
//
void COption::UpData( ccTime tt )
{
	for( int i = 0; i < 4; i++ )
	{
		m_pControlMenu[i]->setIsVisible( false );
	}

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "music" ) )
	{
		m_pControlMenu[2]->setIsVisible( true );
	}
	else
	{
		m_pControlMenu[0]->setIsVisible( true );
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound"  ) )
	{
		m_pControlMenu[3]->setIsVisible( true );
	}
	else
	{
		m_pControlMenu[1]->setIsVisible( true );
	}

}
//---------------------------------------------------------------------
//
//
void COption::menuSound( CCObject * pSender )
{
	int tag = ((CCNode*)pSender)->getTag();
	switch( tag )
	{
	case 0:
		{
		 bool music = CCUserDefault::sharedUserDefault()->getBoolForKey( "music" );
		 CCUserDefault::sharedUserDefault()->setBoolForKey( "music", !music );
		}
		break;
	case 1:
		{
		 bool sound = CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" );
		 CCUserDefault::sharedUserDefault()->setBoolForKey( "sound", !sound );
		}
		break;		

	}
}
//----------------------------------------------------------------------
//
//
void COption::menuCallback( CCObject * pSender )
{
	int tag = ((CCNode*)pSender)->getTag();
	switch( tag )
	{
	case COPTION_EXIT:
		{
			(( CGameMenu* )getParent())->EnableButton( true );
			removeFromParentAndCleanup( true );
		}
		break;
	case COPTION_DELSAVE:
		{
			CSaveSence::Show( true );
		}
		
	}
}
//-----------------------------------------------------------------------
//
//
void COption::onExit()
{
	CCXMLLayer::onExit();
}

