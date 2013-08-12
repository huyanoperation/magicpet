/*****************************************************************************/
//File name:	CFightGroundLayer
//Author: 		huyan
//Description:	fight
/******************************************************************************/
#include "CFightGroundLayer.h"
#include "../loader/CClientLoader.h"
#include "../Function/CExpressions.h"
#include "../GameScene/CGameWorldScene.h"
#include "../GameScene/CGameLoading.h"
#include "CSelectEnemyScene.h"
#include "../CGameText.h"
#include "../Pushui/CEventControl.h"
#include "../Pushui/CFightFinish.h"
#include "CFightScene.h"
#include "../Net/CurlTest.h"
#include "../Platform/CCNd91Mng.h"

CFightGroundLayer *g_pFightGroundLayer = NULL;

int CFightGroundLayer::s_iWincount = 0;

CFightGroundLayer::CFightGroundLayer()
{
	m_iFightTurnId = 0;
	m_iFightResult = 0;
	m_pPetLeft	   = NULL;
	m_pPetRight	   = NULL;

	m_pPlayer1Hp   = NULL;
	m_pPlayer2Hp   = NULL;

	m_pPlayer1Mp   = NULL;
	m_pPlayer2Mp   = NULL;
	m_pLeftPetName = NULL;
	m_pRightPetName= NULL;
	m_bIsFightOver = false;
	m_bIsWin	   = false;

	g_pFightGroundLayer = this;
}
//--------------------------------------------------------------------------
//
//
CFightGroundLayer::~CFightGroundLayer()
{
    CC_SAFE_DELETE( CSelectEnemyScene::s_pPetDataBlock );
}
//-----------------------------------------------------------------------
//
//
void CFightGroundLayer::onEnter()
{
	CCXMLLayer::onEnter();
    

    CCSprite* pSprite = ( CCSprite* )GetXMLNodeFromKey( "fightmap" );
    if( CCRANDOM_0_1() < 0.3f )
    {
        string path = GetGameImagesPath();
        path = path + "fightmap_mooncastle.png";
        CCSprite* pSpriteBG = CCSprite::spriteWithFile( path.c_str() );
        addChild( pSpriteBG );
        pSpriteBG->setPosition( pSprite->getPosition() );
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "music" ) )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("data/audio/fight_2.mp3", true);
        }
    }
    else if( CCRANDOM_0_1() < 0.6f )
    {
        string path = GetGameImagesPath();
        path = path + "fightmap_forest.png";

        CCSprite* pSpriteBG = CCSprite::spriteWithFile( path.c_str() );
        addChild( pSpriteBG );
        pSpriteBG->setPosition( pSprite->getPosition() );
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "music" ) )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("data/audio/fight_1.mp3", true);
        }
    }
    else if( CCRANDOM_0_1() < 1.0f )
    {
        string path = GetGameImagesPath();
        path = path + "fightmap_arena.png";

        CCSprite* pSpriteBG = CCSprite::spriteWithFile( path.c_str() );
        addChild( pSpriteBG );
        pSpriteBG->setPosition( pSprite->getPosition() );
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "music" ) )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("data/audio/fight_3.mp3", true);
        }
    }
    removeChild( pSprite, true );
}
//------------------------------------------------------------------------
//
//
void CFightGroundLayer::CreatPlayer()
{
	//CBaseDataPet *pBaseDataPet = g_pClientLoader->GetBaseDataPetFromId(1);

	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(g_pPetDataBlock->ResId);

	if( NULL != pAnimationData )
	{
		CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petid );

		m_pPetLeft = new CPet();
		m_pPetLeft->init();
		m_pPetLeft->autorelease();
		m_pPetLeft->setSitId( FIGHT_LEFT_PLAYER_POS );
		m_pPetLeft->setFlipX( true );
		
		//PetDatablock *petdata = new PetDatablock();    
		//petdata->petName		= "petName1";
		//petdata->Agility		= 10;
		//petdata->hp			= 100;
		//petdata->MaxHp		= 100;
		//petdata->Damage		= 10;
		//petdata->ResId		= g_pPetDataBlock->ResId;

		m_pPetLeft->setPetDataBlock(g_pPetDataBlock);
		m_pPetLeft->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
		m_pPetLeft->PlayAnimation( pAnimationData->m_stand, -1,NULL,this, false, false );

		m_pPetLeft->setTurnEndCallBack( this, callfuncND_selector( CFightGroundLayer::FightTurnOverCallBack ), NULL );

		CCNode* pNode = GetXMLNodeFromKey( "t2dSceneObject_player1_pos" );
		m_pPetLeft->setPosition( pNode->getPosition() );
		pNode = GetXMLNodeFromKey( "t2dSceneObject_player1_attack" );
		m_pPetLeft->setTargetPosition( pNode->getPosition() );
		addChild( m_pPetLeft, MAXTORQUELAYERNUM );
	}

    CBaseDataPet* pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( CSelectEnemyScene::s_pPetDataBlock->petid );
	CBaseDataAnimation *pAnimationData2 = g_pClientLoader->GetBaseDataAnimationFromId(pPetBaseData->animationid);

	if( NULL != pAnimationData2 )
	{
		m_pPetRight = new CPet();
		m_pPetRight->init();
		m_pPetRight->autorelease();
		m_pPetRight->setSitId( FIGHT_RIFHT_PLAYER_POS );

		m_pPetRight->setPetDataBlock(CSelectEnemyScene::s_pPetDataBlock);
    
		m_pPetRight->addFramesFromiT2D( pAnimationData2->m_strPlist.c_str(), pAnimationData2->m_iPlistType );
		m_pPetRight->PlayAnimation( pAnimationData2->m_stand, -1,NULL,this, false, false );

		m_pPetRight->setTurnEndCallBack( this, callfuncND_selector( CFightGroundLayer::FightTurnOverCallBack ), NULL );

		m_pPetRight->setFlipX( false );

		CCNode* pNode = GetXMLNodeFromKey( "t2dSceneObject_player2_pos" );
		m_pPetRight->setPosition( pNode->getPosition() );

		pNode = GetXMLNodeFromKey( "t2dSceneObject_player2_attack" );
		m_pPetRight->setTargetPosition( pNode->getPosition() );

		addChild( m_pPetRight, MAXTORQUELAYERNUM );
	}
	schedule(schedule_selector(CFightGroundLayer::upData),0.1f);

	m_pPetLeft->setLockTarget( m_pPetRight );
	m_pPetRight->setLockTarget( m_pPetLeft );

/*************************************player1Hp&Mp****************************************/
	m_pPlayer1Hp = new CCSprite();
	m_pPlayer1Hp->init();
    
    string path = GetGameImagesPath();
    path = path + "fight_lefthp.png";
    m_pPlayer1Hp->initWithFile(path.c_str());
	//ccColor3B ccRED={255,0,0};
    //m_pPlayer1Hp->setColor( ccRED );
	m_pPlayer1Hp->autorelease();
	m_pPlayer1Hp->setAnchorPoint( ccp( 0,0 ) );
	addChild( m_pPlayer1Hp, MAXTORQUELAYERNUM );

	CCNode* pNode = GetXMLNodeFromKey( "t2dSceneObject_player1hpbar" );

	m_iMaxHpBar = m_pPlayer1Hp->getTextureRect().size.width;
	m_iMaxHpHeight = m_pPlayer1Hp->getTextureRect().size.height;
	m_pPlayer1Hp->setPosition( ccp( pNode->getPosition().x - m_iMaxHpBar / 2, pNode->getPosition().y - m_iMaxHpHeight / 2 ) );
	m_pPlayer1Hp->setTextureRect( CCRectMake(0, 0, m_iMaxHpBar,m_iMaxHpHeight) );
	m_iHpInitPos = m_pPlayer1Hp->getPosition().x;
	
	m_pPlayer1Mp = new CCSprite();
	m_pPlayer1Mp->init();
	//ccColor3B ccBlue={0,0,255};
	//m_pPlayer1Mp->setColor( ccBlue );
    path = GetGameImagesPath();
    path = path + "fight_leftmp.png";
    m_pPlayer1Mp->initWithFile(path.c_str());
    
	m_pPlayer1Mp->autorelease();
	m_pPlayer1Mp->setAnchorPoint( ccp( 0,0 ) );
	addChild( m_pPlayer1Mp, MAXTORQUELAYERNUM );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_player1MpBar" );

	m_iMaxMpBar		= m_pPlayer1Mp->getTextureRect().size.width;
	m_iMaxMpHeight	= m_pPlayer1Mp->getTextureRect().size.height;
	m_pPlayer1Mp->setPosition( ccp( pNode->getPosition().x - m_iMaxMpBar / 2, pNode->getPosition().y - m_iMaxMpHeight / 2 ) );
	m_pPlayer1Mp->setTextureRect( CCRectMake(0, 0, m_iMaxMpBar,m_iMaxMpHeight) );

	m_iMpInitPos = m_pPlayer1Mp->getPosition().x;

/*************************************player2Hp&Mp****************************************/
	m_pPlayer2Hp = new CCSprite();
	m_pPlayer2Hp->init();
    
    path = GetGameImagesPath();
    path = path + "fight_righthp.png";
    m_pPlayer2Hp->initWithFile(path.c_str());
    
	m_pPlayer2Hp->autorelease();
	m_pPlayer2Hp->setAnchorPoint( ccp( 0,0 ) );
	addChild( m_pPlayer2Hp, MAXTORQUELAYERNUM );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_player2hpbar" );

	m_iMaxHpBar =  m_pPlayer2Hp->getTextureRect().size.width;
	m_iMaxHpHeight = m_pPlayer2Hp->getTextureRect().size.height;
	m_pPlayer2Hp->setPosition( ccp( pNode->getPosition().x - m_iMaxHpBar / 2, pNode->getPosition().y - m_iMaxHpHeight / 2 ) );
	m_pPlayer2Hp->setTextureRect( CCRectMake(0, 0, m_iMaxHpBar,m_iMaxHpHeight) );

	m_pPlayer2Mp = new CCSprite();
	m_pPlayer2Mp->init();
    path = GetGameImagesPath();
    path = path + "fight_rightmp.png";
    m_pPlayer2Mp->initWithFile(path.c_str());
	m_pPlayer2Mp->setAnchorPoint( ccp( 0,0 ) );
	addChild( m_pPlayer2Mp, MAXTORQUELAYERNUM );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_player2MpBar" );

	m_iMaxMpBar		=  m_pPlayer2Mp->getTextureRect().size.width;
	m_iMaxMpHeight	=  m_pPlayer2Mp->getTextureRect().size.height;
	m_pPlayer2Mp->setPosition( ccp( pNode->getPosition().x - m_iMaxMpBar / 2, pNode->getPosition().y - m_iMaxMpHeight / 2 ) );
	m_pPlayer2Mp->setTextureRect( CCRectMake(0, 0, m_iMaxMpBar,m_iMaxMpHeight) );
/****************************************************************************************/
   
	pNode = GetXMLNodeFromKey( "player1NameBg" );

	m_pLeftPetName  = CCLabelTTF::labelWithString( "10000",kFontSystem[FONT_TINY].fontName,kFontSystem[FONT_TINY].fontSize);
	m_pLeftPetName->setColor( ccWhite );
    
	m_pLeftPetName->setString( m_pPetLeft->GetPetData()->petName.c_str() );
	m_pLeftPetName->setPosition( pNode->getPosition());
    addChild( m_pLeftPetName,MAXTORQUELAYERNUM );

	pNode = GetXMLNodeFromKey( "player2NameBG" );

	m_pRightPetName = CCLabelTTF::labelWithString( "10000",kFontSystem[FONT_SMALL].fontName,kFontSystem[FONT_TINY].fontSize);
	m_pRightPetName->setColor( ccWhite );
	m_pRightPetName->setString( m_pPetRight->GetPetData()->petName.c_str() );
	m_pRightPetName->setPosition( pNode->getPosition());
	addChild( m_pRightPetName,MAXTORQUELAYERNUM );
    
    CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( "button_back" );
    CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CFightGroundLayer::menuCallback), 0, this, pSprite->getPosition() );
    removeChild( pSprite, true );
    addChild( pMenu, pSprite->getZOrder() );
    
    std::string effectpath = GetGameParticlePath();
    std::string filename = effectpath + "Glitter.plist";
    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    //m_emitter->setPositionType( kCCPositionTypeRelative );
    pEmitter->setPosition( getPosition() );
    CCPoint pos = getPosition();
    addChild( pEmitter, 100 );
    
    AdvanceSprite *pAdv = new AdvanceSprite();
    pAdv->addFramesFromiT2D( "battlenow.plist" );
    pAdv->autorelease();
    addChild( pAdv, 100 );
    
    m_pBattlenow = pAdv;
    
    int battleNow[] = { 0,2,1 };
    pAdv->PlayAnimation( battleNow, 1, callfunc_selector(CFightGroundLayer::FightBegin), this, false, true );
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    pAdv->setPosition( ccp( size.width/2, size.height/2 ) );

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/Battlenow.mp3", false);
	}
}
//--------------------------
//
//
void CFightGroundLayer::FightBegin()
{
    m_iFightTurnId = m_pPetLeft->getSitId();
   // m_pBattlenow->setIsVisible(false);
}
//------------------------------------------------------------------------
//
//
void CFightGroundLayer::onExit()
{
	CCXMLLayer::onExit();
}
//------------------------------------------------------------------------------
//
//
void CFightGroundLayer::FightTurnOverCallBack( CCNode *object, void* d )
{
	m_iFightTurnId = ((CCNode*)d)->getTag();
	((CCNode*)d)->release();
}
//-----------------------------------------------------------------------------
//
//
void CFightGroundLayer::upData( ccTime dt )
{
	if( m_bIsFightOver )
	{
		g_pPlayerEventCtrl->AddCounterNum( 1, SPECIAL_COUNTER_BATTLE, 0 );
		return;
	}
	if( m_iFightTurnId == m_pPetLeft->getSitId() )
	{
		m_iFightTurnId = 0;
		m_pPetLeft->Attack();
	}
	else if( m_iFightTurnId == m_pPetRight->getSitId() )
	{
		m_iFightTurnId = 0;
		m_pPetRight->Attack();
	}

    //if( m_pPetLeft->getHp() <= 0 )
    //{
    //    m_pPetLeft->setHp(10);
    //}
	if( m_pPetLeft->getHp() <= 0 )
	{
		m_pPlayer1Hp->setIsVisible(false);
		m_bIsFightOver = true;
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        
        string path = GetGameImagesPath();
        path = path + "lose.png";
        CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
        addChild( pSprite, 100 );
        pSprite->setPosition( ccp( size.width/2, size.height/2 ) );
		g_pPlayerEventCtrl->AddCounterNum( 1, SPECIAL_COUNTER_BATTLE_LOSE, 0 );

		g_pPetDataBlock->recordsave.loseBattleCount++;

//		string utf81;
//		CChineseCode::GB2312ToUTF_8( utf81, (char*)fightmessage_lost[g_iCurLanguageIndex], 256 );
//
//		char buffer[256];
//		sprintf( buffer, utf81.c_str(), m_pPetRight->GetPetData()->playername.c_str(), m_pPetLeft->getName().c_str() );
//		g_pPetDataBlock->addGameMessage( GetGameDayAndTime().c_str(), buffer );

		//CFightFinish *pFightFinish = new CFightFinish( false );
		//pFightFinish->init();
		//pFightFinish->autorelease();
		//addChild( pFightFinish, 100 );
        schedule( schedule_selector(CFightGroundLayer::ScheudleExit),3.0f );
        
        s_iWincount = 0;
		return;
	}
	if( m_pPetRight->getHp() <= 0 )
	{
		m_pPlayer2Hp->setIsVisible(false);
		m_bIsFightOver = true;
		m_bIsWin = true;
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        string path = GetGameImagesPath();
        path = path + "win.png";
        CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
        addChild( pSprite, 100 );
        pSprite->setPosition( ccp( size.width/2, size.height/2 ) );
		g_pPlayerEventCtrl->AddCounterNum( 1, SPECIAL_COUNTER_BATTLE_WIN, 0 );

		g_pPetDataBlock->recordsave.winBattleCount++;

		string utf81;
		CChineseCode::GB2312ToUTF_8( utf81, (char*)fightmessage_win[g_iCurLanguageIndex], 256 );

		char buffer[256];
		sprintf( buffer, utf81.c_str(), m_pPetRight->GetPetData()->playername.c_str(), m_pPetRight->getName().c_str() );
		g_pPetDataBlock->addGameMessage( GetGameDayAndTime().c_str(), buffer );
        
        schedule( schedule_selector( CFightGroundLayer::SheduleWin ),3.0f );
        
		//CChineseCode::GB2312ToUTF_8( utf81, (char*)fightplayermessage_lost[g_iCurLanguageIndex], 256 );
        //sprintf( buffer, utf81.c_str(), m_pPetLeft->GetPetData()->playername.c_str() );

        
		s_iWincount++;
        if( s_iWincount >= 2 )
        {
            string utf8;
            CChineseCode::GB2312ToUTF_8( utf8, (char*)message_kill[g_iCurLanguageIndex], 256 );
            
            sprintf( buffer, utf8.c_str(), m_pPetLeft->getName().c_str(), s_iWincount );
            g_pPetDataBlock->addGameMessage( GetGameDayAndTime().c_str(), buffer );
            
        }
		if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
		{
            if( s_iWincount >= 5 )
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/rampage.mp3", false);
			}
            else if( s_iWincount >= 4 )
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/ultrakill.mp3", false);
			}
            else if( s_iWincount >= 3 )
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/triple_kill.mp3", false);
			}
            else if( s_iWincount >= 2 )
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/Double_Kill.mp3", false);
			}
		}
		return;
	}

	
	CCRect rect( 0,0, (float)(m_iMaxHpBar) / (float)(m_pPetLeft->getMaxHp()) * m_pPetLeft->getHp(), m_iMaxHpHeight  );
	m_pPlayer1Hp->setTextureRect( rect );
	m_pPlayer1Hp->setPosition( ccp( m_iHpInitPos + ( m_iMaxHpBar - (float)(m_iMaxHpBar) / (float)(m_pPetLeft->getMaxHp()) * m_pPetLeft->getHp() ), m_pPlayer1Hp->getPosition().y ) );

	CCRect rect2( 0,0, (float)(m_iMaxHpBar) / (float)(m_pPetRight->getMaxHp()) * m_pPetRight->getHp(), m_iMaxHpHeight );
	m_pPlayer2Hp->setTextureRect( rect2 );


	CCRect rectMp( 0, 0, (float)(m_iMaxMpBar) / (float)(m_pPetLeft->getMaxMp()) * m_pPetLeft->getMp(), m_iMaxMpHeight  );
	m_pPlayer1Mp->setTextureRect( rectMp );
	m_pPlayer1Mp->setPosition( ccp( m_iMpInitPos + ( m_iMaxMpBar - (float)(m_iMaxMpBar) / (float)(m_pPetLeft->getMaxMp()) * m_pPetLeft->getMp() ), m_pPlayer1Mp->getPosition().y ) );

	CCRect rect2Mp( 0,0, (float)(m_iMaxMpBar) / (float)(m_pPetRight->getMaxMp()) * m_pPetRight->getMp(), m_iMaxMpHeight );
	m_pPlayer2Mp->setTextureRect( rect2Mp );
}
//-----------------------------------------------
//
//
void CFightGroundLayer::SheduleWin( ccTime tt )
{
    CFightFinish *pFightFinish = new CFightFinish( true );
    pFightFinish->init();
    pFightFinish->autorelease();
    addChild( pFightFinish, 100 );
    unscheduleAllSelectors();
    schedule( schedule_selector(CFightGroundLayer::ScheudleExit),2.0f );
    
    string strSendMail = GetGameDayAndTime() + ",";
    strSendMail = strSendMail + m_pPetLeft->GetPetData()->playername;
    
    CurlTest::ShareInstance()->SendMail( m_pPetLeft->GetPetData()->userid, m_pPetRight->GetPetData()->userid, strSendMail.c_str()  );
}
void CFightGroundLayer::ScheudleExit( ccTime tt )
{
    CGameLoading::Show(LOADING_CGAMEWORLD_SCENE);
}
//-----------------------------------------------------------------------------
//
//
void CFightGroundLayer::menuCallback(CCObject * pSender)
{
    CGameWorldScene::Show();
}