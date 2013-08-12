//
//  CGameMenu.h
//  magicpet
//
//  Created by jia huyan on 11-11-28.
//  Copyright (c) 2011ƒÍ µ¿∑Êª•∂Ø£®±±æ©£©ø∆ºº”–œﬁ‘»Œπ´Àæ. All rights reserved.
//
#include "CGameWorldScene.h"
#include "../CCXMLLayer.h"
#include "CCreateRoleScene.h"
#include "CGameLoading.h"
#include "../MiniGame/CMiniGameScene.h"
#include "../Sprite/CPet.h"
#include "../loader/CClientLoader.h"
#include "../Fighting/CFightScene.h"
#include "../Pushui/CPetAttributeLayer.h"
#include "../Function/CExpressions.h"
#include "CSaveSence.h"
#include "../sql/CSqliteManager.h"
#include "../Pushui/CCFeedLayer.h"
#include "../Pushui/CTaskEventSubGui.h"
#include "../Pushui/CEventControl.h"
#include "../Pushui/CMessageBoxYes.h"
#include "../Pushui/CMessageBoxYesNo.h"
#include "../Pushui/CGameLetter.h"
#include "../Pushui/CLoginDay.h"
#include "../Pushui/CShopLayer.h"
#include "../Platform/CCNd91Mng.h"
#include "../Net/CurlTest.h"
#include "../Fighting/CSelectEnemyScene.h"
#include "CGameCenter_ios.h"
#include "CPet.h"

int CGameWorldScene::s_iPetAniId = 0;

AdvanceSprite *CGameWorldScene::m_pAdvanceSprite = NULL;

CGameWorldScene* g_pGameWorldScene = NULL;

CGameWorldScene::CGameWorldScene()
{
   m_pPet = NULL;
   m_iGameBeginTime = GetCurrentSeconds();
   m_bIsShopJump = false;
   m_bIsJingliJump = false;
    
    memset( &g_pPetDataBlock->otherUsers, 0, sizeof( SOtherUsers )  );
    
    g_pGameWorldScene = this;
    
}
//---------------------------------------------------------------
//
//
CGameWorldScene::~CGameWorldScene()
{
   //CCSpriteFrameCache::sharedSpriteFrameCache();
}
//---------------------------------------------------------------
//
//
void CGameWorldScene::Show( int turnpetid  )
{
    CGameWorldScene* ps = new CGameWorldScene();
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
    
    ps->SetTurnPetId( turnpetid );
}
//------------------------------------------------------------------
//button_achievement button_battlelog button_shopping button_system 
//
void CGameWorldScene::SetTurnPetId( int Id )
{
    m_iTurnId = Id;
}
//-------------------------------
//
//
void CGameWorldScene::TurnPetCallback()
{
     m_pPet->setIsVisible(true);
    
    ccColor4B color ={ 255,255,255,255 };
    CCLayerColor* pLayerColor = CCLayerColor::layerWithColor(color);
    addChild( pLayerColor );
    
    pLayerColor->runAction( CCFadeOut::actionWithDuration(0.5) );
    
    if( !g_pPetDataBlock->recordsave.isNotBeginner[7] && 0 != m_iTurnId )
    {        
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_8[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
        
        CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_8");
        pSprite->setIsVisible(true);
        m_pXMLLayer->reorderChild(pSprite, 1000);
        
        g_pPetDataBlock->recordsave.isNotBeginner[7] = true;
        
    }
}   
//---------------------------
//
//
void CGameWorldScene::onEnter()
{
    CNd91Mng::RemoveBanner();
//#if  COCOS2D_DEBUG
    
//#else
//    if( CSqliteManager::GetInstance()->m_bIsPlayerCheat || g_pClientLoader->m_bIsCheat )
//    {
//        g_pPetDataBlock->petmoney = 0;
//        g_pPetDataBlock->playershoppingmallmoney = 0;
//        g_pPetDataBlock->maxstaminapoint = 0;
//        
//        CSqliteManager::GetInstance()->ReSetSql(0);
//        CSqliteManager::GetInstance()->ReSetSql(1);
//        CSqliteManager::GetInstance()->ReSetSql(2);
//    }
//#endif
    
    string Myinfo =  CNd91Mng::NdGetMyInfo();
    if( !Myinfo.empty() )
    {
        
        
        CCLog("Myinfo = %s",Myinfo.c_str());
        
        char Buffer1[32];
        char Buffer2[32];
        //sscanf(  Myinfo.c_str(), "%s,%s", Buffer1, Buffer2  );
        
        CSqliteManager::GetInstance()->getUnit(Myinfo.c_str(), 0, ",", Buffer1, 32);
        CSqliteManager::GetInstance()->getUnit(Myinfo.c_str(), 1, ",", Buffer2, 32);
        g_pPetDataBlock->playername = Buffer2;
        g_pPetDataBlock->userid = Buffer1;
        
    }
    
	CGameScene::onEnter();
    CCXMLLayer *pXMLLayer = new CCXMLLayer();
	m_pXMLLayer = pXMLLayer;
	pXMLLayer->init();
	pXMLLayer->autorelease();
	pXMLLayer->LoadPlist( s_WorldLeveFile );
	addChild( pXMLLayer );
    
    if( CCUserDefault::sharedUserDefault()->getBoolForKey( "music" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("data/audio/ingame.mp3", true);
	}
    
    const char *MenuButton[] =
	{
        "button_achievement", "button_battlelog", "button_shopping",
        "button_system","button_gameroom", "button_gamblingroom", "button_fight", 
        "button_accessfood", "button_hitstakes","button_slotmachine","button_battle","button_eat","button_91battle"
	};
    
	for( int i = 0; i < 13; i++ )
	{
		CCSprite *pSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CGameWorldScene::menuCallback), i, this, pSprite->getPosition() );
		pXMLLayer->addChild( pMenu, pSprite->getZOrder() );
        pMenu->setTag( i );
        pXMLLayer->RemoveNode( pSprite );

		if( 0 == i )
		{
			m_pEventButton  = pMenu;
		}
		if( 1 == i )
		{
			m_pLetterButton = pMenu;
		}
        if( 2 == i)
        {
            m_pShop = pMenu;
        }
	}
    
    CCNode *pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_ACCESSFOOD );
    pNode->setIsVisible( false );

    pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_HITSTAKES );
    pNode->setIsVisible( false );
    
    pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_SLOTMACHINE );
    pNode->setIsVisible( false );

	pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BATTLE );
    pNode->setIsVisible( false );
    
    pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_FRIEND_BATTLE );
    pNode->setIsVisible( false );

	CBaseDataPet *pPetData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petid );
	g_pPetDataBlock->ResId = pPetData->animationid;

	CGameWorldScene::s_iPetAniId = g_pPetDataBlock->ResId;

	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId( CGameWorldScene::s_iPetAniId );

	pNode = pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_pet_activearea" );
    
    CPet *pet = new CPet();
	pet->autorelease();
	pet->setPetDataBlock( g_pPetDataBlock );
    
	m_pPet = new CPet();
	m_pPet->autorelease();
	memcpy( g_pPetDataBlock->rbg, pPetData->rgb, sizeof( int ) * 3 );
	m_pPet->setPetDataBlock( g_pPetDataBlock );
    if( 0 != m_iTurnId )
    {
        m_pPet->setIsVisible( false );
        
        CBaseDataPet *pPetTurnData = g_pClientLoader->GetBaseDataPetFromId( m_iTurnId );
        pet->addFramesFromiT2D(  pPetTurnData->evolutionpist.c_str() , 1 );
        pet->PlayAnimation( pPetTurnData->animation, 1, callfunc_selector(CGameWorldScene::TurnPetCallback), this, false, true );
        pet->setPosition( pNode->getPosition() );
        addChild(pet, 100);
        
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/jinhua.mp3", false);
        }
        
    }

	CPetWorldLayer *pLayer = new CPetWorldLayer();
	pLayer->autorelease();
	addChild( pLayer,100,CPETWORLDLAYER_TAG );

	pLayer->SetPetPosNode( pNode );

	string path = pAnimationData->m_strPlist;
	m_pPet->addFramesFromiT2D( path.c_str(),pAnimationData->m_iPlistType );
    m_pPet->setPosition( pNode->getPosition() );
    m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	m_pPet->setPosition( pNode->getPosition() );
	pLayer->addChild( m_pPet );
	pLayer->m_pPet = m_pPet;

    std::string effectpath = GetGameParticlePath();
    std::string filename = effectpath + "Glitter.plist";
    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    pEmitter->setPosition( getPosition() );
    CCPoint pos = getPosition();
    addChild( pEmitter, 100 );

	pNode = pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_headicon" );
	path = GetGameImagesPath();
	path = path + pPetData->petshow;
	CCSprite *pSpriteHeadIcon =  CCSprite::spriteWithFile( path.c_str() );
	addChild( pSpriteHeadIcon,100 );
	pSpriteHeadIcon->setPosition( pNode->getPosition() );

	CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petid );
	g_pPetDataBlock->hp = pPetBaseData->inithp;
	g_pPetDataBlock->mp = pPetBaseData->initmp;
	
	for( int i = 1; i <= g_pPetDataBlock->petlevel; i++ )
	{
		g_pPetDataBlock->hp += CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, i );
		g_pPetDataBlock->mp += CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, i );
	}

	g_pPetDataBlock->MaxHp = g_pPetDataBlock->hp;
	g_pPetDataBlock->MaxMp = g_pPetDataBlock->mp;

    schedule(schedule_selector(CGameWorldScene::UpData), 0.1f);
	
	pNode = pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petmoney" );
		
	char strbuffer[32];
	sprintf( strbuffer, "%d",  g_pPetDataBlock->petmoney );
	CCLabelTTF *plabelName = CCLabelTTF::labelWithString(strbuffer,  kFontSystem[FONT_THIN].fontName,kFontSystem[FONT_THIN].fontSize);
	addChild(plabelName,99);
    plabelName->setColor( ccBlack );
	plabelName->setPosition(pNode->getPosition() );

	m_pPetMoney = plabelName;

	pNode = pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_moneypoint" );
	path = GetGameImagesPath();

	sprintf( strbuffer, "%d",  g_pPetDataBlock->playershoppingmallmoney );
	plabelName = CCLabelTTF::labelWithString(strbuffer,  kFontSystem[FONT_THIN].fontName,kFontSystem[FONT_THIN].fontSize);
	addChild(plabelName,99);
    plabelName->setColor( ccBlack );
	plabelName->setPosition( pNode->getPosition() );

	m_pPlayerMoney = plabelName;

	pNode = pXMLLayer->GetXMLNodeFromKey( "button_petstaminapoint" );

	sprintf( strbuffer, "%d",  g_pPetDataBlock->maxstaminapoint );
	plabelName = CCLabelTTF::labelWithString(strbuffer,  kFontSystem[FONT_THIN].fontName,kFontSystem[FONT_THIN].fontSize);
	addChild(plabelName,99);
    plabelName->setColor( ccBlack );
	plabelName->setPosition( pNode->getPosition() );
	m_pMaxstaminapoint = plabelName;

	//CGAMEWORLDSCENE_BUTTON_PETSTAMINAPOINT
	CCSprite *pSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( "button_petstaminapoint" );
	CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CGameWorldScene::menuCallback), CGAMEWORLDSCENE_BUTTON_PETSTAMINAPOINT, this, pSprite->getPosition() );
	pXMLLayer->addChild( pMenu, pSprite->getZOrder() );
    pMenu->setTag( CGAMEWORLDSCENE_BUTTON_PETSTAMINAPOINT );
    pXMLLayer->RemoveNode( pSprite );
    m_petstaminapoint = pMenu;


	pNode = pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_chest" );
	 
	m_pAdvanceSprite = new AdvanceSprite();
	m_pAdvanceSprite->addFramesFromiT2D( "chest.plist", 1 );
	int frame[] = { 0,1,2 };
	m_pAdvanceSprite->PlayAnimation( frame, -1, NULL, NULL, false, false );
	m_pAdvanceSprite->setPosition( pNode->getPosition() );
	m_pAdvanceSprite->setIsVisible( false );
	addChild( m_pAdvanceSprite, 100 );
    
#if COCOS2D_DEBUG || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   
    CCMenuItemFont *pCheatButtonItem = CCMenuItemFont::itemFromString( "addPetExp",this, menu_selector(CGameWorldScene::cheatMenuCallBack)  );
    pCheatButtonItem->setTag(1);
    pCheatButtonItem->setScale(0.4f);
    CCMenuItemFont *pCheatButtonItem2 = CCMenuItemFont::itemFromString( "addItem",this, menu_selector(CGameWorldScene::cheatMenuCallBack)  );
    pCheatButtonItem2->setTag(2);
    pCheatButtonItem2->setScale(0.4f);
    
    CCMenuItemFont *pCheatButtonItem3 = CCMenuItemFont::itemFromString( "addMoney",this, menu_selector(CGameWorldScene::cheatMenuCallBack)  );
    pCheatButtonItem3->setTag(3);
    pCheatButtonItem3->setScale(0.4f);

    CCMenu *pCheatMenu = CCMenu::menuWithItems( pCheatButtonItem, pCheatButtonItem2, NULL );
    pCheatMenu->alignItemsVertically();
    addChild( pCheatMenu,1000 );
    
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    pCheatMenu->setPosition( ccp(size.width / 2, size.height / 3) );
#endif
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    effectpath = GetGameParticlePath();
    filename = effectpath + "leaf.plist";
    pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    //m_emitter->setPositionType( kCCPositionTypeRelative );
    pEmitter->setPosition( ccp( 0, s.height ));
    pos = getPosition();
    pXMLLayer->addChild( pEmitter, 99 );
    
    
    const char *teacher[] = { "t2dAnimatedSprite_teacher_2","t2dAnimatedSprite_teacher_3","t2dAnimatedSprite_teacher_4","t2dAnimatedSprite_teacher_5","t2dAnimatedSprite_teacher_6","t2dAnimatedSprite_teacher_8" };
    for( int i = 0; i < 6; i++ )
    {
        CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey(teacher[i]);
        pSprite->setIsVisible(false);
    }
    
    if( !g_pPetDataBlock->recordsave.isNotBeginner[1] )
    {
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_2[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
        
        CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_2");
        pSprite->setIsVisible(true);
        m_pXMLLayer->reorderChild(pSprite, 1000);
        
        g_pPetDataBlock->recordsave.isNotBeginner[1] = true;
    }
    
    if( g_pPetDataBlock->recordsave.isNotBeginner[2] && !g_pPetDataBlock->recordsave.isNotBeginner[3] )
    {        
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_4[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
        
        CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4");
        pSprite->setIsVisible(true);
        m_pXMLLayer->reorderChild(pSprite, 1000);
        
    }
    
    string bufferdata = CurlTest::ShareInstance()->GetMailData( g_pPetDataBlock->userid );
    
    if( !bufferdata.empty() && bufferdata != "{0}" && bufferdata != "{-1}" )
    {
        Json::Value root;
        if( parseAndSaveValueTree( bufferdata.c_str(), "", root ) )
        { 
            CCAssert( NULL,"Can not found json file!" );
        }
        
        for (int i=root.size() - 1; i >= 0; i--)  
        {
            string mail = root[i]["mail"].asString();
            string sid = root[i]["sid"].asString();
            
            char Buffer1[32];
            char Buffer2[32];
            //sscanf(  Myinfo.c_str(), "%s,%s", Buffer1, Buffer2  );
            
            CSqliteManager::GetInstance()->getUnit(mail.c_str(), 0, ",", Buffer1, 32);
            CSqliteManager::GetInstance()->getUnit(mail.c_str(), 1, ",", Buffer2, 32);
            
            char buffer[64];
            string utf81;
            CChineseCode::GB2312ToUTF_8( utf81, (char*)fightplayermessage_lost[g_iCurLanguageIndex], 256 );
            sprintf( buffer, utf81.c_str(), Buffer2 );
            
            g_pPetDataBlock->addGameMessage( Buffer1, buffer, atoi( sid.c_str() ), Buffer2 );
        }
    }
    
    if( g_pPetDataBlock->recordsave.winBattleCount <= 0 )
    {
        g_pPetDataBlock->recordsave.winBattleCount = 1;
    }
    CNd91Mng::upLoadLeaderBoard( g_pPetDataBlock->recordsave.winBattleCount,204 );
 
    
}
//--------------------------------------------------
//
//
void CGameWorldScene::cheatMenuCallBack( CCObject * pSender  )
{
    CCNode *pNode = (CCNode*)pSender;
    if( 1 == pNode->getTag() )
    {
		g_pPetDataBlock->addPetExp( 500, m_pPet->getPosition() );
    }
    else if( 2 == pNode->getTag() )
    {
        for( int i = 0; i < 20; i++ )
        {
            CBaseDataItem *pDataItem= g_pClientLoader->GetBaseDataItemIndex( i + 1 );
            if( NULL != pDataItem )
            {
                g_pPetDataBlock->addItem(pDataItem->itemid,5 );
            }
        }
    }
    else if( 3 == pNode->getTag() )
    {
		g_pPetDataBlock->addPetMoney( 100, m_pPet->getPosition()  );
		g_pPetDataBlock->addPlayerMoney( 1, m_pPet->getPosition() );
    }
    
}
//--------------------------------------------------
//
//
void CGameWorldScene::SaveItemRecord( int ItemId )
{
	SFeedDataItem* pSaveData = g_pPetDataBlock->GetSaveItemFromId( ItemId );
	bool isFlag = false;
	for( int i = 0; i < MAX_ITEM_HAVE_DFINE; i++ )
	{
		if( g_pPetDataBlock->recordsave.Item[i].itemId == ItemId )
		{
			g_pPetDataBlock->recordsave.Item[i].itemCount += pSaveData->activeCount;
			isFlag = true;
			break;
		}
	}
	if( !isFlag )
	{
		for( int i = 0; i < MAX_ITEM_HAVE_DFINE; i++ )
		{
			if( g_pPetDataBlock->recordsave.Item[i].itemId == 0 )
			{
				g_pPetDataBlock->recordsave.Item[i].itemId= ItemId;
				g_pPetDataBlock->recordsave.Item[i].itemCount += pSaveData->activeCount;
				break;
			}
		}
	}

	
}
//-------------------------------------------------
//
//
void CGameWorldScene::UpData( ccTime tt )
{
	char strbuffer[32];
	sprintf( strbuffer, "%d",  g_pPetDataBlock->petmoney );
	m_pPetMoney->setString( strbuffer );
	
	sprintf( strbuffer, "%d",  g_pPetDataBlock->playershoppingmallmoney );
	m_pPlayerMoney->setString(strbuffer);

    sprintf( strbuffer, "%d",  g_pPetDataBlock->maxstaminapoint );
	m_pMaxstaminapoint->setString(strbuffer);

    g_pPetDataBlock->gametime.lastGameTime = GetCurrentSeconds();
//-------------------------------------------------------game feed updata----------------------------------------------------------------
    for( int i = 0; i < MAX_ITEM_HAVE_DFINE; i++ )
	{
		if( g_pPetDataBlock->pFeedPetData->feedItem[i].itemId != 0 )
		{
			if( g_pPetDataBlock->pFeedPetData->feedItem[i].isActive )
            {
                CBaseDataItem *pDataItem = g_pClientLoader->GetBaseDataItemFromID( g_pPetDataBlock->pFeedPetData->feedItem[i].itemId );
                if( GetCurrentSeconds() -  g_pPetDataBlock->pFeedPetData->feedItem[i].leaveTime > pDataItem->itemcd * g_pPetDataBlock->pFeedPetData->feedItem[i].activeCount )
                {
                    g_pPetDataBlock->pFeedPetData->feedItem[i].isActive = false;
                    std::string effectpath = GetGameParticlePath();
                    std::string filename = effectpath + "petThanks.plist";
                    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
                    pEmitter->setPosition( m_pPet->getPosition() );
                    CCPoint pos = getPosition();
                    addChild( pEmitter, 100 );
                    
                    int exp = g_pPetDataBlock->pFeedPetData->feedItem[i].activeCount * pDataItem->itemexp;
                    int skillexp = g_pPetDataBlock->pFeedPetData->feedItem[i].activeCount * pDataItem->itemskillexp;
                   
                    if( 0 != exp )
                    {
						g_pPetDataBlock->addPetExp( exp,m_pPet->getPosition() );
                    }
                    else if( 0 !=skillexp )
                    {

						g_pPetDataBlock->addSkillExp( skillexp,m_pPet->getPosition() );
                    }
		
					SaveItemRecord(  g_pPetDataBlock->pFeedPetData->feedItem[i].itemId );
                }
            }
		}
	}
//--------------------------------------------------------pet leve up updata---------------------------------------------------------------
    
    CBaseDataExp *pDataExp = g_pClientLoader->GetBaseDataExpFromLevel( g_pPetDataBlock->petlevel );
    
    if( g_pPetDataBlock->petexp >= pDataExp->exp )
    {
        g_pPetDataBlock->petlevel++;
        
		if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/petlevelup.mp3", false);
		}
        
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        string path = GetGameImagesPath();
        path = path + "levelup.png";
        CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
        pSprite->setPosition( ccp( size.width / 2, size.height/ 2 ) );
        pSprite->runAction( CCFadeOut::actionWithDuration( 3.0f ) );
        addChild(pSprite,100);

		CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petid );
		g_pPetDataBlock->hp = pPetBaseData->inithp;
		g_pPetDataBlock->mp = pPetBaseData->initmp;

		for( int i = 1; i <= g_pPetDataBlock->petlevel; i++ )
		{
			g_pPetDataBlock->hp += CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, i );
			g_pPetDataBlock->mp += CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, i );
		}
        g_pPetDataBlock->MaxHp = g_pPetDataBlock->hp;
		g_pPetDataBlock->MaxMp = g_pPetDataBlock->mp;
    }
    
    //
//----------------------------------------------------updata event--------------------------------------------------------------------------
    
	if( NULL != g_pPlayerEventCtrl )
	{
		g_pPlayerEventCtrl->upData( 0 );
	}

	if( EVENT_STATE_NEW == g_pPlayerEventCtrl->GetEventState() )
	{
		if( NULL == m_pEventButton->getActionByTag( ACTION_EVENTBUTTONJUMP ) )
		{
			CCSize s= CCDirector::sharedDirector()->getWinSize();
			g_pPlayerEventCtrl->SetEventState( EVENT_STATE_NONE );
			CCActionInterval*  jump = CCJumpBy::actionWithDuration(5, CCPointMake(0,0), s.height / 20, 10);
			jump->setTag( ACTION_EVENTBUTTONJUMP );
			m_pEventButton->runAction(jump);

			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/shake.wav");
            
            if( !g_pPetDataBlock->recordsave.isNotBeginner[4] && g_pPetDataBlock->petlevel > 1 )
            {
                string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_5[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
                
                g_pPetDataBlock->recordsave.isNotBeginner[4] = true;
                CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_5");
                pSprite->setIsVisible(true);
                //teacher_5
            }
		}
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/event.mp3", false);
        }
	}

	if( g_pPetDataBlock->recordsave.isHaveNewMessage )
	{
		if( NULL == m_pLetterButton->getActionByTag( ACTION_LETTERBUTTONJUMP ) )
		{
			CCSize s= CCDirector::sharedDirector()->getWinSize();
			CCActionInterval*  jump = CCJumpBy::actionWithDuration(5, CCPointMake(0,0), s.height / 20, 10);
			jump->setTag( ACTION_LETTERBUTTONJUMP );
			m_pLetterButton->runAction(jump);
			g_pPetDataBlock->recordsave.isHaveNewMessage = false;

			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/shake.wav");
		}
	}
    
    if( m_bIsShopJump )
    {
        m_bIsShopJump = false;
        if( NULL == m_pShop->getActionByTag( ACTION_SH0P_JUMP ) )
		{
			CCSize s= CCDirector::sharedDirector()->getWinSize();
			CCActionInterval*  jump = CCJumpBy::actionWithDuration(5, CCPointMake(0,0), s.height / 20, 10);
			jump->setTag( ACTION_SH0P_JUMP );
			m_pShop->runAction(jump);
			g_pPetDataBlock->recordsave.isHaveNewMessage = false;
            
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/shake.wav");
		}
    }
    
    if( m_bIsJingliJump )
    {
        m_bIsJingliJump = false;
        if( NULL == m_petstaminapoint->getActionByTag( ACTION_JINGLI_JUMP ) )
		{
			CCSize s= CCDirector::sharedDirector()->getWinSize();
			CCActionInterval*  jump = CCJumpBy::actionWithDuration(5, CCPointMake(0,0), s.height / 20, 10);
            CCActionInterval*  jump2 = (CCActionInterval*)jump->copy();
            
			jump->setTag( ACTION_JINGLI_JUMP );
			m_petstaminapoint->runAction(jump);
            m_pMaxstaminapoint->runAction(jump2);
			g_pPetDataBlock->recordsave.isHaveNewMessage = false;
            
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/shake.wav");
		}
    }
    
	//-------------------------------------------------------updata gameData------------------------------------------------------------------------------

//	SYSTEMTIME wtm;
//	GetLocalTime(&wtm);
//	struct tm tTm;
//
	char strDayBuffer[64];
//	
//	string year;
//	if( wtm.wYear / 10 == 0 )
//	{
//		sprintf( strDayBuffer, "0%d", wtm.wYear );
//		year		= strDayBuffer;
//	}
//	else
//	{
//		sprintf( strDayBuffer, "%d", wtm.wYear );
//		year		= strDayBuffer;
//	}
//
//	string month;
//	if( wtm.wMonth / 10 == 0 )
//	{
//		sprintf( strDayBuffer, "0%d", wtm.wMonth );
//		month		= strDayBuffer;
//	}
//	else
//	{
//		sprintf( strDayBuffer, "%d", wtm.wMonth );
//		month		= strDayBuffer;
//	}
//
//	string day;
//	if( wtm.wDay / 10 == 0 )
//	{
//		sprintf( strDayBuffer, "0%d", wtm.wDay );
//		day		= strDayBuffer;
//	}
//	else
//	{
//		sprintf( strDayBuffer, "%d", wtm.wDay );
//		day		= strDayBuffer;
//	}
    
//sprintf( strDayBuffer, "%s%s%s", year.c_str(), month.c_str(), day.c_str());
    
    time_t timer; 
    struct tm *t_tm; 
    time(&timer); 
    t_tm = localtime(&timer); 
    sprintf(strDayBuffer,"%4d%02d%02d\n", t_tm->tm_year+1900, 
           t_tm->tm_mon+1, t_tm->tm_mday); 
    

	int gameday = atoi( strDayBuffer );
	
	if( gameday - g_pPetDataBlock->gameday >= 1 )
	{
		g_pPetDataBlock->gameday = gameday;
		//g_pPetDataBlock->maxstaminapoint = 100;
		m_pAdvanceSprite->setIsVisible( true );
        g_pPetDataBlock->recordsave.weiboGetMoneyCount= 5;
	}

	else if( gameday - g_pPetDataBlock->gameday < 0 )
	{
		CCMessageBox( "your system time has been changed!", "alert" );
		g_pPetDataBlock->maxstaminapoint = 0;
		g_pPetDataBlock->petmoney = 0;
		g_pPetDataBlock->gameday = gameday;
		m_pAdvanceSprite->setIsVisible( false );
	}
//----------------------------------------------------------------------------------------------------------------------------

    if(0 != g_pPetDataBlock->petturning[0] )
    {
        if( !g_pPetDataBlock->recordsave.isNotBeginner[5] )
        {
            string utf81,utf82;
            CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_6[g_iCurLanguageIndex], 256 );
            CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
            CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
            pMessage->autorelease();
            CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
            
            g_pPetDataBlock->recordsave.isNotBeginner[5] = true;
            
            CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_6");
            pSprite->setIsVisible(true);
            reorderChild(pSprite, 1000);
            
        }
    }

}
//------------------------------------------------------------------
//
//
void CGameWorldScene::EnableAllWorldButton( bool isEnable )
{
	for( int i = 0; i < 13; i++ )
	{
		CCMenu* pMenu = (CCMenu*)m_pXMLLayer->getChildByTag( i );
		pMenu->setIsTouchEnabled( isEnable );
	}
	CCLayer* pLayer = ( CCLayer* )getChildByTag( CPETWORLDLAYER_TAG );
	pLayer->setIsTouchEnabled( isEnable );
}
//------------------------------------------------------------------
//
//
void CGameWorldScene::scheduleShowGameRoom( ccTime tt )
{
    //CCNode *pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_ACCESSFOOD );
    //pNode->setIsVisible( false );
    //pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_HITSTAKES );
    //pNode->setIsVisible( false );
}
//--------------------------------------------------------------------
//
//
void CGameWorldScene::scheduleShowGamblingroom( ccTime tt )
{
    //CCNode *pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_SLOTMACHINE );
   // pNode->setIsVisible( false );
}
//--------------------------------------------------------------------
//
//
void CGameWorldScene::onExit()
{
	CCScene::onExit();
	g_pPetDataBlock->gametime.gameRunTime = g_pPetDataBlock->gametime.gameRunTime + ( GetCurrentSeconds() - m_iGameBeginTime );
    
    
    if( g_pPetDataBlock->userid.empty() )
    {
        return;
    }
    
    string format = "{\"playerid\":\"%s\",\"playershoppingmallmoney\":\"0\",\"petid\":\"%d\",\"petname\":\"%s\",\"petsex\":\"0\",\"pettitle\":\"0\",\"petlevel\":\"%d\",\"petexp\":\"0\",\"maxstaminapoint\":\"0\",\"petturning\":\"0\",\"petskillexppond\":\"0\",\"itemnum\":\"0\",\"petmoney\":\"0\",\"aechievepoint\":\"0\",\"aechieveflag\":\"0\",\"skilllevel\":\"%d,%d\",\"skillexp\":\"0\",\"gametime\":\"0\",\"feed\":\"0\",\"feedsave\":\"0\",\"gameday\":\"0\"}";
        
    char buffer[1024];
    sprintf( buffer, format.c_str(), g_pPetDataBlock->userid.c_str(), g_pPetDataBlock->petid, g_pPetDataBlock->petName.c_str(),g_pPetDataBlock->petlevel, g_pPetDataBlock->skilllevel.skillsLevel[0], g_pPetDataBlock->skilllevel.skillsLevel[1] );
        
    CurlTest::ShareInstance()->PostUpData( buffer );
    
    CCLOG("PostUpData = %s", buffer );
}
//-----------------------------------------------------------------
//
//
void CGameWorldScene::menuCallback(CCObject * pSender)
{
	int tag = ((CCNode*)( pSender ))->getTag();
   
	switch(tag)
	{
	case CGAMEWORLDSCENE_BUTTON_ACHIEVEMENT:
		{
			EnableAllWorldButton(false);
			CTaskEventSubGui* pEventGui = new CTaskEventSubGui();
			pEventGui->init();
			pEventGui->autorelease();
			addChild( pEventGui,100 );
            
            CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_5");
            pSprite->setIsVisible(false);
		}
    break;
	case CGAMEWORLDSCENE_BUTTON_BATTLELOG:
		{
			//CCNode *pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BATTLE );
			//pNode->setIsVisible( true );
			EnableAllWorldButton(false);
			CGameLetter* pGui = new CGameLetter();
			pGui->init();
			pGui->autorelease();
			addChild( pGui,100 );
		}
    break;
	case CGAMEWORLDSCENE_BUTTON_SHOPPING:
		{
			EnableAllWorldButton(false);
			CShopLayer *pLayer = new CShopLayer();
			pLayer->autorelease();
			addChild( pLayer,100 );
		}
    break;
	case CGAMEWORLDSCENE_BUTTON_SYSTEM:
		{
			CGameLoading::Show( LOADING_CGAME_SAVE );
		}
    break;
    case CGAMEWORLDSCENE_BUTTON_BUTTON_GAMEROOM:
        {
            unschedule( schedule_selector( CGameWorldScene::scheduleShowGameRoom ) );
            CCNode *pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_ACCESSFOOD );
            pNode->setIsVisible( true );
            pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_HITSTAKES );
            pNode->setIsVisible( true );
            schedule( schedule_selector( CGameWorldScene::scheduleShowGameRoom ) , 5.0f);
            
            
            CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_2");
            pSprite->setIsVisible(false);
            if( !g_pPetDataBlock->recordsave.isNotBeginner[2] )
            {
                string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_3[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
                
                CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_3");
                pSprite->setIsVisible(true);
                
            }
        }
    break;
    case CGAMEWORLDSCENE_BUTTON_BUTTON_GAMBLINGROOM:
        {
            unschedule( schedule_selector( CGameWorldScene::scheduleShowGamblingroom ));
            CCNode *pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BUTTON_SLOTMACHINE );
            pNode->setIsVisible( true );
            schedule( schedule_selector( CGameWorldScene::scheduleShowGamblingroom ) , 5.0f);
        }
    break;
    case CGAMEWORLDSCENE_BUTTON_BUTTON_FIGHT:
        {
            CCNode	*pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_BATTLE );
			pNode->setIsVisible( true );
            
            if(g_iGameMode)
            {
                pNode = m_pXMLLayer->getChildByTag( CGAMEWORLDSCENE_BUTTON_FRIEND_BATTLE );
                pNode->setIsVisible( true );
            }
            
            CCSprite *pSprite = ( CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_8");
            pSprite->setIsVisible(false);
        }
	break;
	case CGAMEWORLDSCENE_BUTTON_EAT:
		{
			EnableAllWorldButton(false);
			CCFeedLayer *pFeedLayer = new CCFeedLayer();
			pFeedLayer->autorelease();
			addChild( pFeedLayer,100 );
            
            CCSprite *pSprite = (CCSprite*)m_pXMLLayer->GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4");
            pSprite->setIsVisible(false);
		}
    break;
    case CGAMEWORLDSCENE_BUTTON_BUTTON_ACCESSFOOD:
        {
			if( g_pPetDataBlock->maxstaminapoint < MINIGAME_SUB_STAMINAPOINT )
			{
				string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nostaminapoint[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
                
                m_bIsJingliJump = true;
				return;
			}
			if( g_pPetDataBlock->petmoney < MINIGAME_SUB_MOENY ) 
			{
				string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoney[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );

                 m_bIsShopJump = true;
				return;
			}

			g_pPetDataBlock->submaxstaminapoint( MINIGAME_SUB_STAMINAPOINT );
			g_pPetDataBlock->subPetMoney( MINIGAME_SUB_MOENY );


			 CGameLoading::Show( LOADING_CGAME_MINI_GAME_ACCESSFOOD );
        }
    break;
    case CGAMEWORLDSCENE_BUTTON_BUTTON_HITSTAKES:
        {
			if( m_pPet->getID() <= 10 )
			{
                string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nominigame[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
				
                return;
			}

			if(  g_pPetDataBlock->maxstaminapoint < MINIGAME_SUB_STAMINAPOINT )
			{
				string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nostaminapoint[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );

                 m_bIsJingliJump = true;
				return;
			}

			if( g_pPetDataBlock->petmoney < MINIGAME_SUB_MOENY )
			{
				string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoney[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );

                 m_bIsShopJump = true;
				return;
			}

			g_pPetDataBlock->submaxstaminapoint( MINIGAME_SUB_STAMINAPOINT );
			g_pPetDataBlock->subPetMoney( MINIGAME_SUB_MOENY );

             CGameLoading::Show( LOADING_CGAME_MINI_BIT_WOOD );
        }
    break;
    case CGAMEWORLDSCENE_BUTTON_BUTTON_SLOTMACHINE:
        {
            CGameLoading::Show( LOADING_CGAME_MINI_GAME_GAMBLING );
        }
    break;
	case CGAMEWORLDSCENE_BUTTON_PETSTAMINAPOINT:
		{
			string utf81;
			char buffer[256];
			sprintf( buffer, message_moneypoint_2[g_iCurLanguageIndex], 1 );
			CChineseCode::GB2312ToUTF_8( utf81, buffer, 256 );
			CMessageBoxYesNo *pMessage= new CMessageBoxYesNo( (char*)utf81.c_str(), callfuncN_selector( CGameWorldScene::AddstaminapointYes ), NULL, this, 0, true );
			pMessage->autorelease();
			addChild( pMessage, 100 );
		}
	break;
	case CGAMEWORLDSCENE_BUTTON_BATTLE:
		{
			if( m_pPet->getID() <= 10 )
			{
                string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nofight[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
				return;
			}
			if( g_pPetDataBlock->maxstaminapoint < FIGHT_SUB_STAMINAPOINT )
			{
				string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nostaminapoint[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );

                 m_bIsJingliJump = true;
				return;
			}

			if( g_pPetDataBlock->petmoney < FIGHT_SUB_MONEY  )
			{
				string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoney[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );

                 m_bIsShopJump = true;
				return;
			}

			g_pPetDataBlock->submaxstaminapoint( FIGHT_SUB_STAMINAPOINT );
			g_pPetDataBlock->subPetMoney( FIGHT_SUB_MONEY );

            CGameLoading::Show( LOADING_CGAME_FIGHT );
            
            
		}
        break;
        case CGAMEWORLDSCENE_BUTTON_FRIEND_BATTLE:
        {
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            if( 0 == CDevice::CheckNet() )
            {
                CCMessageBox("网络貌似有问题！", "提示");
                return;
            }
#endif
            if( m_pPet->getID() <= 10 )
			{
                string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nofight[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
				return;
			}
            CGameCenter::FindPlayer();
            
//            if( !CNd91Mng::NdGetUserLeaderBoardInfoList( g_pPetDataBlock->userid, 204 ) )
//            {
//                return;
//            }
//            
//            string path = GetGameImagesPath();
//            path = path + "loading.png";
//            CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
//            CCSize s = CCDirector::sharedDirector()->getWinSize();
//            pSprite->setPosition( ccp( s.width / 2, s.height / 2));
//            addChild( pSprite, 100, NET_PLAYER_LOADING_TAG );
//            
//            g_pPetDataBlock->otherUsers.count = 0;
        }

	}

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/buttton.mp3", false);
	}
}

void CGameWorldScene::OnUserCallBack( string uin, string name )
{
    if( NULL == g_pPetDataBlock )
    {
        return;
    }
    
    if(  g_pPetDataBlock->otherUsers.count >= OTHERUSERS_NUM - 240 )
    {
        return;
    }
    
    if( g_pPetDataBlock->petid <= 10 )
    {
        return;
    }
    
    if( rand()%100 < 20 )
    {
        char buffer[1024];
        string utf81;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)fightplayermessage_lost[g_iCurLanguageIndex], 256 );
        sprintf( buffer, utf81.c_str(), name.c_str() );
        g_pPetDataBlock->addGameMessage( GetGameDayAndTime().c_str(), buffer, 10, name );
    }
    
    strcpy( g_pPetDataBlock->otherUsers.otheruser[ g_pPetDataBlock->otherUsers.count ].uin, uin.c_str() );
    strcpy( g_pPetDataBlock->otherUsers.otheruser[ g_pPetDataBlock->otherUsers.count ].name, name.c_str() );
    g_pPetDataBlock->otherUsers.count ++;
    
    CCLOG( "%s  %s", uin.c_str(), name.c_str() );
}
//--------------------------------------------------------------
//
//
void CGameWorldScene::OnUserFinishCallBack()
{
    g_pGameWorldScene->removeChildByTag(NET_PLAYER_LOADING_TAG, true);
    
    for( int i = 0; i < g_pPetDataBlock->otherUsers.count; i++ )
    {
        int tempindex = rand() % g_pPetDataBlock->otherUsers.count;
        SOtherUser tempOtheruser = g_pPetDataBlock->otherUsers.otheruser[i];
        g_pPetDataBlock->otherUsers.otheruser[i] = g_pPetDataBlock->otherUsers.otheruser[tempindex];
        g_pPetDataBlock->otherUsers.otheruser[tempindex] = tempOtheruser;
    }
    
    if( g_pGameWorldScene->m_pPet->getID() <= 10 )
    {
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nofight[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
        return;
    }
    if( g_pPetDataBlock->maxstaminapoint < FIGHT_SUB_STAMINAPOINT )
    {
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nostaminapoint[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
        
        g_pGameWorldScene->m_bIsJingliJump = true;
        return;
    }
    
    if( g_pPetDataBlock->petmoney < FIGHT_SUB_MONEY  )
    {
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoney[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
        
        g_pGameWorldScene->m_bIsShopJump = true;
        return;
    }
    CCDirector::sharedDirector()->purgeCachedData();
    g_pPetDataBlock->submaxstaminapoint( FIGHT_SUB_STAMINAPOINT );
    g_pPetDataBlock->subPetMoney( FIGHT_SUB_MONEY );
    CSelectEnemyScene::Show( true );
    
    CSqliteManager::GetInstance()->SaveSql();
}
//----------------------------------------------------------------------------------------------
//
//
void CGameWorldScene::AddstaminapointYes( CCNode *pNode )
{
	if( !g_pPetDataBlock->subPlayershoppingmallmoney( 1 ) )
	{
		string utf81,utf82;
		CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoneypoint[g_iCurLanguageIndex], 256 );
		CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
		CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
		pMessage->autorelease();
		CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
		return;
	}
	g_pPetDataBlock->maxstaminapoint = 100;
}

CPetWorldLayer::CPetWorldLayer()
{
	m_bIsMove = false;
}
//---------------------------------------------------------------------------------------
//
//
CPetWorldLayer::~CPetWorldLayer()
{

}
void CPetWorldLayer::SetPetPosNode( CCNode *pNode )
{
	m_pPetPosNode = pNode; 
}

//---------------------------------------------------------------------------------------
//
//
void CPetWorldLayer::Show()
{
	
}
//---------------------------------------------------------------------------------------
//
//
void CPetWorldLayer::onEnter()
{
	CCLayer::onEnter();
	setIsTouchEnabled( true );

	schedule(schedule_selector(CPetWorldLayer::PetAiSchedule),5.0f);

}
//---------------------------------------------------------------------
//
//
void CPetWorldLayer::PetMoveEndCallBank( CCNode *pNode, void* d )
{
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(CGameWorldScene::s_iPetAniId );
	m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	m_bIsMove = false;
}
//---------------------------------------------------------------------------------------
//
//
void CPetWorldLayer::animationEndCallBank()
{
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(CGameWorldScene::s_iPetAniId );
	m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	m_bIsMove = false;
}
//----------------------------------------------------------------------
//
//
void CPetWorldLayer::PetAiSchedule( ccTime tt )
{
    if( !m_pPet->getIsVisible() )
    {
        return;
    }
	if( !m_bIsMove )
	{
		if( CCRANDOM_0_1() < 0.6 )
		{
			//left
			if( CCRANDOM_0_1() < 0.5 )
			{
				if( m_pPet->getPosition().x > m_pPetPosNode->getPosition().x - m_pPetPosNode->getContentSize().width / 2 )
				{			
					CCActionInterval* pInterval = CCMoveBy::actionWithDuration( 1.0f, ccp( -m_pPet->getTextureRect().size.width / 2, 0 ) );
					CCActionInstant *pInstance = CCCallFuncND::actionWithTarget( this, callfuncND_selector( CPetWorldLayer::PetMoveEndCallBank ), NULL  );
					m_pPet->runAction( CCSequence::actions( pInterval, pInstance, NULL ) );
					m_pPet->setFlipX( false );
					
					CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(CGameWorldScene::s_iPetAniId );
					m_pPet->PlayAnimation(pAnimationData->m_move, -1, NULL, this, false, false);
					m_bIsMove = true;

				}
			}
			//right
			else
			{
				if( m_pPet->getPosition().x < m_pPetPosNode->getPosition().x + m_pPetPosNode->getContentSize().width / 2 )
				{
					CCActionInterval* pInterval = CCMoveBy::actionWithDuration( 1.0f, ccp( m_pPet->getTextureRect().size.width / 2, 0 ) );
					CCActionInstant *pInstance = CCCallFuncND::actionWithTarget( this, callfuncND_selector( CPetWorldLayer::PetMoveEndCallBank ), NULL  );
					m_pPet->runAction( CCSequence::actions( pInterval, pInstance, NULL ) );
					m_pPet->setFlipX( true );

					CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(CGameWorldScene::s_iPetAniId );
					m_pPet->PlayAnimation(pAnimationData->m_move, -1, NULL, this, false, false);
					m_bIsMove = true;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------------
//
//
void CPetWorldLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
	CCPoint touchLocation = touch->locationInView( touch->view() );
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );

	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(CGameWorldScene::s_iPetAniId );
    
    std::string effectpath = GetGameParticlePath();
    std::string filename = effectpath + "touchDownEffect.plist";
    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    pEmitter->setPosition( touchLocation );
    CCPoint pos = getPosition();
    addChild( pEmitter, 100 );

	if( CCRect::CCRectContainsPoint( CCRect( m_pPet->getPosition().x - m_pPet->getTextureRect().size.width / 2, m_pPet->getPosition().y - m_pPet->getTextureRect().size.height / 2, m_pPet->getTextureRect().size.width, m_pPet->getTextureRect().size.height ), touchLocation ) )
	{
		m_pPet->stopAllActions();

		if( CCRANDOM_0_1() < 0.3 )
		{
			m_pPet->PlayAnimation(pAnimationData->m_specialaction_1, 1, callfunc_selector( CPetWorldLayer::animationEndCallBank ), this, false, false);
		}
		else if( CCRANDOM_0_1() < 0.6 )
		{
			m_pPet->PlayAnimation(pAnimationData->m_specialaction_2, 1, callfunc_selector( CPetWorldLayer::animationEndCallBank ), this, false, false);
		}
		else
		{
			m_pPet->PlayAnimation(pAnimationData->m_specialaction_3, 1, callfunc_selector( CPetWorldLayer::animationEndCallBank ), this, false, false);
		}
		m_bIsMove = true;
        
	}

	if( CCRect::CCRectContainsPoint( CCRect( 0 ,0, s.width / 5, s.height / 4 ), touchLocation ) )
	{
             
        removeChildByTag(CGAMEWORLDSCENE_PUSH_ATTRIBUTE, true);
        
		CPetAttributeLayer *pAttributeLayer = new CPetAttributeLayer();
		pAttributeLayer->autorelease();
		addChild( pAttributeLayer, 0, CGAMEWORLDSCENE_PUSH_ATTRIBUTE );
		
		((CGameWorldScene*)getParent())->EnableAllWorldButton(false);
	}

	if( CGameWorldScene::m_pAdvanceSprite->getIsVisible() )
	{
		if( CCRect::CCRectContainsPoint( CCRect( CGameWorldScene::m_pAdvanceSprite->getPosition().x - CGameWorldScene::m_pAdvanceSprite->getTextureRect().size.width / 2 , CGameWorldScene::m_pAdvanceSprite->getPosition().y - CGameWorldScene::m_pAdvanceSprite->getTextureRect().size.height / 2 , CGameWorldScene::m_pAdvanceSprite->getTextureRect().size.width, CGameWorldScene::m_pAdvanceSprite->getTextureRect().size.height ), touchLocation ) )
		{
			CGameWorldScene::m_pAdvanceSprite->setIsVisible( false );

			CLoginDay *pDataLogin = new CLoginDay();
			pDataLogin->init();
			pDataLogin->autorelease();
			addChild( pDataLogin );

			pDataLogin->FadeOut();
            
            if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/event.mp3", false);
            }

		}
	}

}
    
