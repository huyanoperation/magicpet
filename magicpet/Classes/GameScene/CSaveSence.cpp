//---------------------------------------------------------------------
//
//
#include "CSaveSence.h"
#include "../CCXMLLayer.h"
#include "../sql/CSqliteManager.h"
#include "platform/platform.h"
#include "../Sprite/CPet.h"
#include "CGameLoading.h"
#include "CGameMenu.h"
//#include "../CCScrollLayer.h"
//#include "../CCScrollLayerExt.h"

static const char* s_saveSencefile = "save.plist";

CSaveSence::CSaveSence()
{
    m_bIsDelete = false;
}
//---------------------------------------------------------------------
//
//
CSaveSence::~CSaveSence()
{

}
//---------------------------------------------------------------------
//
//
void CSaveSence::Show( bool isDelete )
{
	CSaveSence* ps = new CSaveSence();
	ps->init();
	ps->autorelease();
    ps->setIsDeleteSave( isDelete );

	if ( CCDirector::sharedDirector()->getRunningScene())
	{
		CCDirector::sharedDirector()->replaceScene( ps );
	}
	else
	{
		CCDirector::sharedDirector()->runWithScene( ps );
	}
}
//---------------------------------------------------------------------
//
//
void CSaveSence::onEnter()
{
	CGameScene::onEnter();

	CCXMLLayer *pXMLLayer = new CCXMLLayer();
	pXMLLayer->init();
	pXMLLayer->autorelease();
	pXMLLayer->LoadPlist( s_saveSencefile );
	addChild( pXMLLayer );

	const char *MenuButton[] =
	{
		"button_save1",
		"button_save2",
		"button_save3",
		"button_back",
	};

	for( int i = 0; i < 4; i++ )
	{
		CCSprite *pSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CSaveSence::menuCallback), i, this, pSprite->getPosition() );
		pXMLLayer->addChild( pMenu, pSprite->getZOrder() );
		pXMLLayer->removeChild( pSprite, true );
	}


	const char* NewGame[] = { "newgame_1", "newgame_2", "newgame_3" };
	const char* Savemessage[] = { "savemessage_1","savemessage_2","savemessage_3" };
	const char* t2dSceneObject_gametime[] = { "t2dSceneObject_gametime_1", "t2dSceneObject_gametime_2", "t2dSceneObject_gametime_3" };
	const char* t2dSceneObject_petname[] = { "t2dSceneObject_petname_1", "t2dSceneObject_petname_2", "t2dSceneObject_petname_3" };
	const char* t2dSceneObject_petlv[] = { "t2dSceneObject_petlv_1", "t2dSceneObject_petlv_2", "t2dSceneObject_petlv_3" };
	const char* t2dSceneObject_moneypoint[] = { "t2dSceneObject_moneypoint_1", "t2dSceneObject_moneypoint_2", "t2dSceneObject_moneypoint_3" };
	const char* t2dSceneObject_money[] = { "t2dSceneObject_money_1", "t2dSceneObject_money_2", "t2dSceneObject_money_3" };

	for( int i = 0; i < 3; i++ )
	{
		CSqliteManager::GetInstance()->m_iCurPos = i;

		CCSprite *pNewGameSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( NewGame[i] );
		CCSprite *pSavemessage = (CCSprite*)pXMLLayer->GetXMLNodeFromKey( Savemessage[i] );

		if( CSqliteManager::GetInstance()->CheckIsEmpty() )
		{
			pNewGameSprite->setIsVisible( true );
			pSavemessage->setIsVisible( false );
			m_bIsSave[i] = false;
			
		}
		else
		{
			pNewGameSprite->setIsVisible( false );
			pSavemessage->setIsVisible( true );

			CCNode *pNode = pXMLLayer->GetXMLNodeFromKey( t2dSceneObject_gametime[i] );

			int h,m,s;
			GetHourMiniSecond( g_pPetDataBlock->gametime.gameRunTime, h, m, s );
			char buffer[64];
			sprintf( buffer, " %dh %dm %ds", h, m, s );

			CCLabelTTF* pLabel = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
			pLabel->setPosition( pNode->getPosition() );
            CCRenderTexture* pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
            addChild( pstroke );
			addChild( pLabel );

			pNode = pXMLLayer->GetXMLNodeFromKey( t2dSceneObject_petname[i] );
			pLabel = CCLabelTTF::labelWithString( g_pPetDataBlock->petName.c_str(), kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
			pLabel->setPosition( pNode->getPosition() );
            pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
            addChild( pstroke );
			addChild( pLabel );

			sprintf( buffer, "%d", g_pPetDataBlock->petlevel );

			pNode = pXMLLayer->GetXMLNodeFromKey( t2dSceneObject_petlv[i] );
			pLabel = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
			pLabel->setPosition( pNode->getPosition() );
            pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
            addChild( pstroke );
			addChild( pLabel );

			sprintf( buffer, "%d", g_pPetDataBlock->playershoppingmallmoney);
			pNode = pXMLLayer->GetXMLNodeFromKey( t2dSceneObject_moneypoint[i] );
			pLabel = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
			pLabel->setPosition( pNode->getPosition() );
            pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
            addChild( pstroke );
			addChild( pLabel );

			sprintf( buffer, "%d", g_pPetDataBlock->petmoney );
			pNode = pXMLLayer->GetXMLNodeFromKey( t2dSceneObject_money[i] );
			pLabel = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
			pLabel->setPosition( pNode->getPosition() );
            pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
            addChild( pstroke );
			addChild( pLabel );

			m_bIsSave[i] = true;
		}
	}
}

void CSaveSence::onExit()
{
	CGameScene::onExit();
}
 
void CSaveSence::menuCallback(CCObject * pSender)
{
	int tag = ((CCNode*)( pSender ))->getTag();

	switch( tag )
	{
	case CSAVESENCE_BUTTON_SAVE1:
		{
            if( m_bIsDelete )
            {
                CSqliteManager::GetInstance()->ReSetSql(0);
                CSaveSence::Show( true );
                return;
            }
            
			CSqliteManager::GetInstance()->m_iCurPos = 0;
			CSqliteManager::GetInstance()->ReadSql();
		}
		break;
	case CSAVESENCE_BUTTON_SAVE2:
		{
            if( m_bIsDelete )
            {
              CSqliteManager::GetInstance()->ReSetSql(1);
              CSaveSence::Show( true );
              return;
            }
			CSqliteManager::GetInstance()->m_iCurPos = 1;
			CSqliteManager::GetInstance()->ReadSql();
		}
		break;
	case CSAVESENCE_BUTTON_SAVE3:
		{
            if( m_bIsDelete )
            {
               CSqliteManager::GetInstance()->ReSetSql(2);
               CSaveSence::Show( true );
               return;
            }
            
			CSqliteManager::GetInstance()->m_iCurPos = 2;
			CSqliteManager::GetInstance()->ReadSql();
		}
		break;
	case CSAVESENCE_BUTTON_BACK:
		{
			CSqliteManager::GetInstance()->m_iCurPos = -1;
			CGameMenu::Show();
			return;
		}
		break;
	}

	if( m_bIsSave[CSqliteManager::GetInstance()->m_iCurPos] )
	{
		CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
	}
	else
	{
		CGameLoading::Show( LOADING_CGAME_CREAT_ROLE_SCENE );
	}

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/buttton.mp3", false);
	}
}


