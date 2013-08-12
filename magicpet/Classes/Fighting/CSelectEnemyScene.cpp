//
//  CSelectEnemyScene.cpp
//  magicpet
//
//  Created by jia huyan on 12-2-7.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include <iostream>
#include "CSelectEnemyScene.h"
#include "../CCXMLLayer.h"
#include "../loader/CClientLoader.h"
#include "../Function/CExpressions.h"
#include "CFightScene.h"
#include "../GameScene/CGameLoading.h"
#include "../Net/CurlTest.h"
#include "../sql/CSqliteManager.h"

static const char* s_SelectLeveFile = "fightfriends.plist";

PetDatablock *CSelectEnemyScene::s_pPetDataBlock = NULL;
CSelectEnemyScene::CSelectEnemyScene( bool isUser )
{
    m_bIsUser = isUser;
    
    m_pRandomPet1 = NULL;
    m_pRandomPet2 = NULL;
    m_pRandomPet3 = NULL;
    m_pRandomPet4 = NULL;
}
//------------------------------------------
//
//
CSelectEnemyScene::~CSelectEnemyScene()
{
    
}
//---------------------------------------------
//
//
void CSelectEnemyScene::Show( bool isOtherUser )
{
    CSelectEnemyScene* ps = new CSelectEnemyScene( isOtherUser );
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
//---------------------------------------------
//
//
PetDatablock *CSelectEnemyScene::GetNPC()
{
    PetDatablock *pRandomPet = new PetDatablock();
    pRandomPet->petlevel = g_pPetDataBlock->petlevel - 3  + rand()%5;
    pRandomPet->petid    =  11 + rand()%59;
    CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( pRandomPet->petid );
    pRandomPet->petName  = pPetBaseData->petname;
    pRandomPet->skilllevel = g_pPetDataBlock->skilllevel;
    pRandomPet->playername = "NPC";
    return pRandomPet;
}
//---------------------------------------------
//
//
PetDatablock *CSelectEnemyScene::GetOthers( string uin, string name )
{
    CCLOG("playername = %s", name.c_str());
    
    string otherbuffer = CurlTest::ShareInstance()->GetData(uin);
    
    PetDatablock *pRandomPet = NULL;
    
    pRandomPet = GetNPC();
    
    pRandomPet->skilllevel.skillsLevel[0] = rand()%3;
    pRandomPet->skilllevel.skillsLevel[1] = rand()%3;
    if( !name.empty() )
    {
        pRandomPet->playername = name;
        string NewName = name;
        pRandomPet->petName = NewName;
    }
//    PetDatablock *pRandomPet = NULL;
//    if( otherbuffer == "{0}" )
//    {
//        pRandomPet = GetNPC();
//    }
//    else if( otherbuffer == "{-1}" )
//    {
//        pRandomPet = GetNPC();
//    }
//    else
//    {
//        CCLOG("getdata = %s", otherbuffer.c_str());
//        Json::Value root;
//        if( parseAndSaveValueTree( otherbuffer, "", root ) )
//        { 
//            CCAssert( NULL,"Can not found json file!" );
//            return NULL;
//        }
//        //string format = "{\"playerid\":\"%s\",\"playershoppingmallmoney\":\"0\",\"petid\":\"%d\",\"petname\":\"%s\",\"petsex\":\"0\",\"pettitle\":\"0\",\"petlevel\":\"%d\",\"petexp\":\"0\",\"maxstaminapoint\":\"0\",\"petturning\":\"0\",\"petskillexppond\":\"0\",\"itemnum\":\"0\",\"petmoney\":\"0\",\"aechievepoint\":\"0\",\"aechieveflag\":\"0\",\"skilllevel\":\"%d,%d\",\"skillexp\":\"0\",\"gametime\":\"0\",\"feed\":\"0\",\"feedsave\":\"0\",\"gameday\":\"0\"}";
//        
//        pRandomPet = new PetDatablock();
//        string SkillLeveStr;
//        for (int i=0; i<root.size(); i++)  
//        {
//            pRandomPet->petid    =  atoi( root[i]["petid"].asString().c_str() );
//            
//            if( pRandomPet->petid <= 10 )
//            {
//                CC_SAFE_DELETE(pRandomPet);
//                pRandomPet = GetNPC();
//                return pRandomPet;
//            }
//            
//            pRandomPet->petName  =  root[i]["petname"].asString();
//            
//            pRandomPet->petlevel =  atoi(root[i]["petlevel"].asString().c_str());
//            SkillLeveStr     =   root[i]["skilllevel"].asString();
//            pRandomPet->playername = name;
//            pRandomPet->userid = uin;
//            
//            CCLOG("petid = %d", pRandomPet->petid);
//            CCLOG("petlevel = %d", pRandomPet->petlevel);
//            CCLOG("SkillLeveStr = %s", SkillLeveStr.c_str());
//        }
//        
//        char skillbuffer1[32];
//        char skillbuffer2[32];
//        
//        CSqliteManager::GetInstance()->getUnit(SkillLeveStr.c_str(), 0, ",", skillbuffer1, 32);
//        CSqliteManager::GetInstance()->getUnit(SkillLeveStr.c_str(), 1, ",", skillbuffer2, 32);
//        
//        pRandomPet->skilllevel.skillsLevel[0] = atoi(skillbuffer1);
//        pRandomPet->skilllevel.skillsLevel[1] = atoi(skillbuffer2);
//    }
    return pRandomPet;
}
//---------------------------------------------
//
//
void CSelectEnemyScene::onEnter()
{
    CGameScene::onEnter();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
	CCXMLLayer *pXMLLayer = new CCXMLLayer();
	pXMLLayer->init();
	pXMLLayer->autorelease();
	pXMLLayer->LoadPlist( s_SelectLeveFile );
	addChild( pXMLLayer );
    
    const char *MenuButton[] =
	{
		"button_fight_1", "button_fight_2", "button_fight_3" ,"button_fight_4"
	}; 
    
    for( int i = 0; i < 4; i++ )
    {
        CCSprite *pSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CSelectEnemyScene::menuCallback), i, this, pSprite->getPosition() );
		pXMLLayer->addChild( pMenu, pSprite->getZOrder() );
        pMenu->setTag( i );
        pXMLLayer->removeChild( pSprite, true );
    }
    
    CCSprite *pSprite= (CCSprite*)pXMLLayer->GetXMLNodeFromKey( "button_back" );
    CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CSelectEnemyScene::menuCallback), 4, this, pSprite->getPosition() );
    pXMLLayer->addChild( pMenu, pSprite->getZOrder() );
    pMenu->setTag( 4 );
    pXMLLayer->removeChild( pSprite, true );
    
    CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petid );
    
    CCNode *pNode =  pXMLLayer->GetXMLNodeFromKey("t2dSceneObject_petshow_self");
    
	CPet *pPet = new CPet();
	pPet->autorelease();
	pPet->setPosition( pNode->getPosition() );
	addChild( pPet,100 );
    pPet->setFlipX(true);
    
    pPet->setPetDataBlock( g_pPetDataBlock );
	//
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetBaseData->animationid);
	pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
	pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	pPet->setPosition( pNode->getPosition() );
    
    /***************************************************self******************************************************/
    char buffer[128];
    int perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, g_pPetDataBlock->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petself_str" );
	sprintf( buffer,"%d",perdata  );
	CCLabelTTF *pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, g_pPetDataBlock->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petself_int" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->dexaptitude, pPetBaseData->updex, pPetBaseData->initdex, g_pPetDataBlock->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petself_dex" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, g_pPetDataBlock->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petself_skill" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_playername" );
	pLabelTTF = CCLabelTTF::labelWithString( g_pPetDataBlock->petName.c_str(), kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    /**************************************************end*********************************************************/
    //random NPC1
    
    
    if( m_bIsUser )
    {
        m_pRandomPet1 = GetOthers( g_pPetDataBlock->otherUsers.otheruser[0].uin, g_pPetDataBlock->otherUsers.otheruser[0].name );
    }
    else
    {
        m_pRandomPet1 = GetNPC();
    }
    
   
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_playername_1" );
    pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet1->playername.c_str(), kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
    pLabelTTF->setPosition( pNode->getPosition() );
    CCRenderTexture* pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
    addChild( pLabelTTF,100 );
    pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( m_pRandomPet1->petid );
    
    m_pRandomPet1->ResId    = pPetBaseData->animationid;
    m_pRandomPet1->rbg[0]   = pPetBaseData->rgb[0];
    m_pRandomPet1->rbg[1]   = pPetBaseData->rgb[1];
    m_pRandomPet1->rbg[2]   = pPetBaseData->rgb[2];
	

    pNode =  pXMLLayer->GetXMLNodeFromKey("t2dSceneObject_petshow_friend_1");
    pPet = new CPet();
	pPet->autorelease();
	pPet->setPosition( pNode->getPosition() );
    pPet->setPetDataBlock( m_pRandomPet1 );
	addChild( pPet,100 );
    
    pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetBaseData->animationid);
	pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
	pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	pPet->setPosition( pNode->getPosition() );
    
    perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet1->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_str_1" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet1->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_int_1" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->dexaptitude, pPetBaseData->updex, pPetBaseData->initdex, m_pRandomPet1->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_dex_1" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, m_pRandomPet1->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_skill_1" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_friendname_1" );
	pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet1->petName.c_str(), kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
    //random NPC2
    if( m_bIsUser )
    {
        m_pRandomPet2 = GetOthers( g_pPetDataBlock->otherUsers.otheruser[1].uin, g_pPetDataBlock->otherUsers.otheruser[1].name );
    }
    else
    {
        m_pRandomPet2 = GetNPC();
    }
    
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_playername_2" );
    pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet2->playername.c_str(), kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
    pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
    addChild( pLabelTTF,100 );
    
    pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( m_pRandomPet2->petid );
    
    m_pRandomPet2->ResId    = pPetBaseData->animationid;
    m_pRandomPet2->rbg[0]   = pPetBaseData->rgb[0];
    m_pRandomPet2->rbg[1]   = pPetBaseData->rgb[1];
    m_pRandomPet2->rbg[2]   = pPetBaseData->rgb[2];
   	//m_pRandomPet2->hp = pPetBaseData->inithp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet1->petlevel );
	//m_pRandomPet2->MaxHp = pPetBaseData->inithp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet2->petlevel );
	//m_pRandomPet2->mp = pPetBaseData->initmp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet2->petlevel );
	//m_pRandomPet2->MaxMp = pPetBaseData->initmp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet2->petlevel );

    pNode =  pXMLLayer->GetXMLNodeFromKey("t2dSceneObject_petshow_friend_2");
    pPet = new CPet();
	pPet->autorelease();
	pPet->setPosition( pNode->getPosition() );
    pPet->setPetDataBlock( m_pRandomPet2 );
	addChild( pPet,100 );
    
    pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetBaseData->animationid);
	pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
	pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	pPet->setPosition( pNode->getPosition() );
    
    perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet2->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_str_2" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet2->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_int_2" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->dexaptitude, pPetBaseData->updex, pPetBaseData->initdex, m_pRandomPet2->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_dex_2" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, m_pRandomPet2->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_skill_2" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_friendname_2" );
	pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet2->petName.c_str(), kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
    //random NPC3
    if( m_bIsUser )
    {
        m_pRandomPet3 = GetOthers( g_pPetDataBlock->otherUsers.otheruser[2].uin, g_pPetDataBlock->otherUsers.otheruser[2].name );
    }
    else
    {
        m_pRandomPet3 = GetNPC();
    }
    
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_playername_3" );
    pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet3->playername.c_str(), kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
    pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
    addChild( pLabelTTF,100 );
    
    pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( m_pRandomPet3->petid );
    
    m_pRandomPet3->ResId    = pPetBaseData->animationid;
    m_pRandomPet3->rbg[0]   = pPetBaseData->rgb[0];
    m_pRandomPet3->rbg[1]   = pPetBaseData->rgb[1];
    m_pRandomPet3->rbg[2]   = pPetBaseData->rgb[2];
   	//m_pRandomPet3->hp = pPetBaseData->inithp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet3->petlevel );
	//m_pRandomPet3->MaxHp = pPetBaseData->inithp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet3->petlevel );
	//m_pRandomPet3->mp = pPetBaseData->initmp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet3->petlevel );
	//m_pRandomPet3->MaxMp = pPetBaseData->initmp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet3->petlevel );

    pNode =  pXMLLayer->GetXMLNodeFromKey("t2dSceneObject_petshow_friend_3");
    pPet = new CPet();
	pPet->autorelease();
	pPet->setPosition( pNode->getPosition() );
    pPet->setPetDataBlock( m_pRandomPet3 );
	addChild( pPet,100 );
    
    pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetBaseData->animationid);
	pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
	pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	pPet->setPosition( pNode->getPosition() );
    
    perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet3->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_str_3" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet3->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_int_3" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->dexaptitude, pPetBaseData->updex, pPetBaseData->initdex, m_pRandomPet3->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_dex_3" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, m_pRandomPet3->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_skill_3" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_friendname_3" );
	pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet3->petName.c_str(), kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    //random NPC4
    if( m_bIsUser )
    {
        m_pRandomPet4 = GetOthers( g_pPetDataBlock->otherUsers.otheruser[3].uin, g_pPetDataBlock->otherUsers.otheruser[3].name );
    }
    else
    {
        m_pRandomPet4 = GetNPC();
    }
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_playername_4" );
    pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet4->playername.c_str(), kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
    pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
    addChild( pLabelTTF,100 );
    
    pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( m_pRandomPet4->petid );
    
    m_pRandomPet4->ResId    = pPetBaseData->animationid;
    m_pRandomPet4->rbg[0]   = pPetBaseData->rgb[0];
    m_pRandomPet4->rbg[1]   = pPetBaseData->rgb[1];
    m_pRandomPet4->rbg[2]   = pPetBaseData->rgb[2];
   	//m_pRandomPet4->hp = pPetBaseData->inithp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet4->petlevel );
	//m_pRandomPet4->MaxHp = pPetBaseData->inithp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet4->petlevel );
	//m_pRandomPet4->mp = pPetBaseData->initmp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet4->petlevel );
	//m_pRandomPet4->MaxMp = pPetBaseData->initmp + CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet4->petlevel );

    pNode =  pXMLLayer->GetXMLNodeFromKey("t2dSceneObject_petshow_friend_4");
    pPet = new CPet();
	pPet->autorelease();
	pPet->setPosition( pNode->getPosition() );
    pPet->setPetDataBlock( m_pRandomPet4 );
	addChild( pPet,100 );
    
    pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetBaseData->animationid);
	pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
	pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	pPet->setPosition( pNode->getPosition() );
    
    perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, m_pRandomPet4->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_str_4" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, m_pRandomPet4->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_int_4" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->dexaptitude, pPetBaseData->updex, pPetBaseData->initdex, m_pRandomPet4->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_dex_4" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, m_pRandomPet4->petlevel );
	pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_petfriend_skill_4" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
    
    pNode =  pXMLLayer->GetXMLNodeFromKey( "t2dSceneObject_friendname_4" );
	pLabelTTF = CCLabelTTF::labelWithString( m_pRandomPet4->petName.c_str(), kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
	addChild( pLabelTTF,100 );
}
//-----------------------------------------------
//
//
void CSelectEnemyScene::onExit()
{
    CGameScene::onExit();
}
//-----------------------------------------------
//
//
void CSelectEnemyScene::menuCallback(CCObject * pSender)
{
    int tag = ((CCNode*)( pSender ))->getTag();
    
    switch(tag)
    {
    case CSELECTENEMYSCENE_BUTTON_FIGHT_1:
        {
            CC_SAFE_DELETE(m_pRandomPet2);
            CC_SAFE_DELETE(m_pRandomPet3);
            CC_SAFE_DELETE(m_pRandomPet4);
            
            s_pPetDataBlock = m_pRandomPet1;
        }
        break;
    case CSELECTENEMYSCENE_BUTTON_FIGHT_2:
        {
            CC_SAFE_DELETE(m_pRandomPet1);
            CC_SAFE_DELETE(m_pRandomPet3);
            CC_SAFE_DELETE(m_pRandomPet4);
            
            s_pPetDataBlock = m_pRandomPet2;
        }
        break;
    case CSELECTENEMYSCENE_BUTTON_FIGHT_3:
        {
            CC_SAFE_DELETE(m_pRandomPet1);
            CC_SAFE_DELETE(m_pRandomPet2);
            CC_SAFE_DELETE(m_pRandomPet4);
            
            s_pPetDataBlock = m_pRandomPet3;
        }
        break;
    case CSELECTENEMYSCENE_BUTTON_FIGHT_4:
        {
            CC_SAFE_DELETE(m_pRandomPet1);
            CC_SAFE_DELETE(m_pRandomPet2);
            CC_SAFE_DELETE(m_pRandomPet3);
            
            s_pPetDataBlock = m_pRandomPet4;
        }
        break;
        case 4:
        {
             CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
            return;
        }
        break;
    }
    
    if( s_pPetDataBlock->petid <= 10 )
    {
        CCMessageBox("You can not fight with this pet!!!", "message");
    }
    
	//if( 0 == s_pPetDataBlock->skilllevel.skillsLevel[0] )
	//{
	//	s_pPetDataBlock->skilllevel.skillsLevel[0] = 1;
	//}
    CFightScene::Show();
}
//----------------------------------------------
//
//
