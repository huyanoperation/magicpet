#include "CPetAttributeLayer.h"
#include "../GameScene/CGameWorldScene.h"
#include "../Sprite/CPet.h"
#include "../Function/CExpressions.h"
#include "../loader/CClientLoader.h"
#include "../sql/CSqliteManager.h"
#include "../CGameText.h"
#include "CMessageBoxYes.h"


CPetAttributeLayer::CPetAttributeLayer()
{
	m_iAddSkillExpIndex = -1;

	for( int i = 0; i < 2; i++ )
	{
		m_pButtonAddSkill[i] = NULL;
	}
}

CPetAttributeLayer::~CPetAttributeLayer()
{

}
//------------------------------------------------------
//
//
void CPetAttributeLayer::onEnter()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    setScale(0);
    runAction( CCScaleTo::actionWithDuration(0.5, 1) );
    
	ccColor4B color ={ 0,0,0,150 };
    CCLayerColor* pLayerColor = CCLayerColor::layerWithColor(color);
    addChild( pLayerColor );

	CCXMLLayer::onEnter();
	LoadPlist( "petattribute.plist" );

	const char *MenuButton[] =
	{
        "button_back", "button_evolution"
	};
    
	CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petid );

	for( int i = 0; i < 2; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CPetAttributeLayer::menuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
        removeChild( pSprite, true );

		if( i == 1 )
		{
			if( 0 != g_pPetDataBlock->petturning[0] )
			{
				CBaseDataPet *pBaseTurnData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petturning[0]);
				string path = GetGameImagesPath();
				path = path + pBaseTurnData->evolution_button;
				CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str());
				addChild( pSprite, 100 );
				pSprite->setPosition( pMenu->getPosition() );
			}
		}
	}

	CCNode *pNode =  GetXMLNodeFromKey( "t2dSceneObject_petname" );
	CCLabelTTF* pLabelTTF = CCLabelTTF::labelWithString( g_pPetDataBlock->petName.c_str(), kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    CCRenderTexture* pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );

	pNode =  GetXMLNodeFromKey( "t2dSceneObject_petlv" );
	char buffer[128];
	sprintf( buffer,"%d",g_pPetDataBlock->petlevel  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );


	int perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, g_pPetDataBlock->petlevel );
	pNode =  GetXMLNodeFromKey( "t2dSceneObject_pet_str" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );

	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, g_pPetDataBlock->petlevel );
	pNode =  GetXMLNodeFromKey( "t2dSceneObject_pet_init" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );

	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->dexaptitude, pPetBaseData->updex, pPetBaseData->initdex, g_pPetDataBlock->petlevel );
	pNode =  GetXMLNodeFromKey( "t2dSceneObject_pet_dex" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );

	perdata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, g_pPetDataBlock->petlevel );
	pNode =  GetXMLNodeFromKey( "t2dSceneObject_pet_skill" );
	sprintf( buffer,"%d",perdata  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_SMALL].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );

	pNode =  GetXMLNodeFromKey("t2dSceneObject_petshow");

	CPet *pPet = new CPet();
	pPet->autorelease();
	pPet->setPosition( pNode->getPosition() );
	addChild( pPet,100 );
	//
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(pPetBaseData->animationid);
	pPet->addFramesFromiT2D( pAnimationData->m_strPlist.c_str(), pAnimationData->m_iPlistType );
	pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	pPet->setPetDataBlock( g_pPetDataBlock );
	pPet->setPosition( pNode->getPosition() );
    pPet->setFlipX(true);

	CCSprite* pSprite=  ( CCSprite* )GetXMLNodeFromKey("pethp");
	//pSprite->setAnchorPoint( ccp( 0,0.5 ) );
	//perdata = CExpressions::GetInstance()->GetAddHP(  );
	perdata = pPet->getHp();
	sprintf( buffer,"%d/%d",perdata,pPet->getMaxHp()  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pSprite->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );


	pSprite =  ( CCSprite* )GetXMLNodeFromKey("petmp");
	//pSprite->setAnchorPoint( ccp( 0,0.5 ) );
	//perdata = CExpressions::GetInstance()->GetAddHP(  );
	perdata = pPet->getMp();
	sprintf( buffer,"%d/%d",perdata,pPet->getMaxMp()  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pSprite->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );
    
    
    pSprite =  ( CCSprite* )GetXMLNodeFromKey("petexp");
	//pSprite->setAnchorPoint( ccp( 0,0.5 ) );
	//perdata = CExpressions::GetInstance()->GetAddHP(  );
	
    int MaxExp = 0;
    int curExp = 0;
    if( 1 == g_pPetDataBlock->petlevel )
    {
        CBaseDataExp *pBaseDataExp = g_pClientLoader->GetBaseDataExpFromLevel( g_pPetDataBlock->petlevel );
        MaxExp = pBaseDataExp->exp;
        curExp = g_pPetDataBlock->petexp;
    }
    else
    {
        CBaseDataExp *pBaseDataExp1 = g_pClientLoader->GetBaseDataExpFromLevel( g_pPetDataBlock->petlevel );
        CBaseDataExp *pBaseDataExp2 = g_pClientLoader->GetBaseDataExpFromLevel( g_pPetDataBlock->petlevel - 1 );
        
        MaxExp = pBaseDataExp1->exp - pBaseDataExp2->exp;
        curExp = g_pPetDataBlock->petexp - pBaseDataExp2->exp;
    }
    
	sprintf( buffer,"%d/%d",curExp,MaxExp  );
	pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_TINY].fontSize );
	pLabelTTF->setPosition( pSprite->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );
    
    pSprite->setAnchorPoint(ccp( 0.0f,0.5f ));
    pSprite->setPosition( ccp( pSprite->getPosition().x - pSprite->getContentSize().width / 2, pSprite->getPosition().y) );
    
    pSprite->setTextureRect( CCRect( 0, 0, pSprite->getContentSize().width / MaxExp * curExp, pSprite->getContentSize().height ) );


	pSprite =  ( CCSprite* )GetXMLNodeFromKey("skillexp");
	pLabelTTF = CCLabelTTF::labelWithString( "", kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize );
	pLabelTTF->setPosition( pSprite->getPosition() );
	addChild( pLabelTTF,100 );
	m_pTTFSkillExp = pLabelTTF;

	m_pSpriteSkillExp = pSprite;
	m_pSpriteSkillExp->setAnchorPoint( ccp( 0,0.5 ) );
	m_pSpriteSkillExp->setPosition( ccp( m_pSpriteSkillExp->getPosition().x - m_pSpriteSkillExp->getTextureRect().size.width / 2, m_pSpriteSkillExp->getPosition().y ) );
	
	schedule( schedule_selector( CPetAttributeLayer::UpData ) );

	m_skillExpWidth = m_pSpriteSkillExp->getContentSize().width;
	
	setIsTouchEnabled( true );

    const char *arrSkillbutton[] = { "button_skill1","button_skill2","button_skill3" };

	for( int i = 0; i < 2; i++ )
	{
		if( g_pPetDataBlock->skilllevel.skillsLevel[i] > 0 )
		{
			int skillId = 0;

			if( 0 == i )
			{
				skillId = pPetBaseData->skillfield[g_pPetDataBlock->skilllevel.skillsLevel[i] - 1];
			}
			else if( 1 == i )
			{
				skillId = pPetBaseData->skillfield_defense[g_pPetDataBlock->skilllevel.skillsLevel[i] - 1];
			}
			CBaseDataSkill* pBaseDataSkill = g_pClientLoader->GetBaseDataSkillFromId( skillId );

			pSprite =  ( CCSprite* )GetXMLNodeFromKey(arrSkillbutton[i]);
			m_pButtonAddSkill[i] = pSprite;
			if( NULL != pBaseDataSkill )
			{
				string path = GetGameImagesPath();
				path = path + pBaseDataSkill->skillicon;
				CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
				pSprite->setPosition( m_pButtonAddSkill[i]->getPosition() );
				addChild( pSprite, 100 );

			}
            
            sprintf( buffer, TEXT_LEVEL[g_iCurLanguageIndex], g_pPetDataBlock->skilllevel.skillsLevel[i] );
            string utf8;
            CChineseCode::GB2312ToUTF_8( utf8, buffer, 32 );
            
            CCLabelTTF *pLabelTTF = CCLabelTTF::labelWithString( utf8.c_str(), kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_TINY].fontSize );
            pLabelTTF->setPosition( pSprite->getPosition() );
            pLabelTTF->setPosition( ccp( pSprite->getPosition().x, pSprite->getPosition().y + pSprite->getTextureRect().size.height / 2 ) );
            pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
            addChild( pstroke,100 );
            addChild( pLabelTTF,100 );
            
            m_pTTFSkillLevel[i] = pLabelTTF;
		}
	}

	const char *arrSkillText[] = { "skill_1", "skill_2", "skill_3" };

	for( int i = 0; i < 2; i++ )
	{
		if( g_pPetDataBlock->skilllevel.skillsLevel[i] > 0 )
		{
			int skillId = 0;

			if( 0 == i )
			{
				skillId = pPetBaseData->skillfield[g_pPetDataBlock->skilllevel.skillsLevel[i] - 1];
			}
			else if( 1 == i )
			{
				skillId = pPetBaseData->skillfield_defense[g_pPetDataBlock->skilllevel.skillsLevel[i] - 1];
			}
			CBaseDataSkill* pBaseDataSkill = g_pClientLoader->GetBaseDataSkillFromId( skillId );


			if( NULL != pBaseDataSkill )
			{
				pSprite =  ( CCSprite* )GetXMLNodeFromKey(arrSkillText[i]);
				char buffer[32];

				sprintf( buffer,"%d/%d", g_pPetDataBlock->skillexp.skillsExp[i],pBaseDataSkill->improveskilllevelExp );
				m_pTTFSkillItem[i] = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_TINY].fontName, kFontSystem[FONT_TINY].fontSize );
				m_pTTFSkillItem[i]->setPosition( pSprite->getPosition() );
				addChild( m_pTTFSkillItem[i],100 );

			}
		}
	}
    
    pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_7");
    pSprite->setIsVisible(false);
    
    pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_9");
    pSprite->setIsVisible(false);
    
    if(0 != g_pPetDataBlock->petturning[0] )
    {
        if( !g_pPetDataBlock->recordsave.isNotBeginner[6] )
        {
            string utf81,utf82;
            CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_7[g_iCurLanguageIndex], 256 );
            CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
            CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
            pMessage->autorelease();
            CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
            
            pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_7");
            pSprite->setIsVisible(true);
            
            g_pPetDataBlock->recordsave.isNotBeginner[6] = true;
        }
    }
    if( g_pPetDataBlock->skilllevel.skillsLevel[0] >= 1 )
    {
        if( !g_pPetDataBlock->recordsave.isNotBeginner[8] )
        {
            string utf81,utf82;
            CChineseCode::GB2312ToUTF_8( utf81, (char*)teacher_9[g_iCurLanguageIndex], 256 );
            CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
            CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
            pMessage->autorelease();
            CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 1000 );
            
            pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_9");
            pSprite->setIsVisible(true);
            
            g_pPetDataBlock->recordsave.isNotBeginner[8] = true;
        }
    }
	
}
//--------------------------------------------------------
//
//
void CPetAttributeLayer::UpData( ccTime tt )
{
	//MAX_SKILL_EXP
	CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( g_pPetDataBlock->petid );
	m_pSpriteSkillExp->setTextureRect( CCRect( 0, 0, m_skillExpWidth / MAX_SKILL_EXP * g_pPetDataBlock->petskillexppond, m_pSpriteSkillExp->getContentSize().height ) );
	
	char buffer[128];
	sprintf( buffer, "%d/%d", g_pPetDataBlock->petskillexppond , MAX_SKILL_EXP );
	m_pTTFSkillExp->setString( buffer );


	if( m_iAddSkillExpIndex >= 0 && g_pPetDataBlock->petskillexppond > 0  )
	{
		char buffer[32];
	
		if( g_pPetDataBlock->skilllevel.skillsLevel[m_iAddSkillExpIndex] >= 1 && g_pPetDataBlock->skilllevel.skillsLevel[m_iAddSkillExpIndex] <=3 )
		{
			int skillId = 0;

			if( 0 == m_iAddSkillExpIndex )
			{
				skillId = pPetBaseData->skillfield[g_pPetDataBlock->skilllevel.skillsLevel[m_iAddSkillExpIndex] - 1];
			}
			else if( 1 == m_iAddSkillExpIndex )
			{
				skillId = pPetBaseData->skillfield_defense[g_pPetDataBlock->skilllevel.skillsLevel[m_iAddSkillExpIndex] - 1];
			}
			CBaseDataSkill* pBaseDataSkill = g_pClientLoader->GetBaseDataSkillFromId( skillId );

			g_pPetDataBlock->petskillexppond--;
			g_pPetDataBlock->skillexp.skillsExp[m_iAddSkillExpIndex]++;

			char buffer[32];
			sprintf( buffer,"%d/%d", g_pPetDataBlock->skillexp.skillsExp[m_iAddSkillExpIndex],pBaseDataSkill->improveskilllevelExp );
			m_pTTFSkillItem[m_iAddSkillExpIndex]->setString(buffer);
			
			if( g_pPetDataBlock->skillexp.skillsExp[m_iAddSkillExpIndex] >= pBaseDataSkill->improveskilllevelExp )
			{
				g_pPetDataBlock->skilllevel.skillsLevel[m_iAddSkillExpIndex]++;
				g_pPetDataBlock->skillexp.skillsExp[m_iAddSkillExpIndex] = 0;

				sprintf( buffer, TEXT_LEVEL[g_iCurLanguageIndex], g_pPetDataBlock->skilllevel.skillsLevel[m_iAddSkillExpIndex] );
				string utf8;
				CChineseCode::GB2312ToUTF_8( utf8, buffer, 32 );
				m_pTTFSkillLevel[m_iAddSkillExpIndex]->setString( utf8.c_str() );

				if( g_pPetDataBlock->skilllevel.skillsLevel[m_iAddSkillExpIndex] >= 3 )
				{
                    string utf81,utf82;
                    CChineseCode::GB2312ToUTF_8( utf81, (char*)message_skillmaxlevel[g_iCurLanguageIndex], 256 );
                    CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                    CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                    pMessage->autorelease();
                    CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
				}
				else
				{
                    string utf81,utf82;
                    CChineseCode::GB2312ToUTF_8( utf81, (char*)message_skillnewlevel[g_iCurLanguageIndex], 256 );
                    CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                    CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                    pMessage->autorelease();
                    CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
				}
                m_iAddSkillExpIndex = -1;
			}
		}
	}
}
//------------------------------------------------------
//
//
void CPetAttributeLayer::onExit()
{
	CCXMLLayer::onExit();
}
//-------------------------------------------------------
//
//
void CPetAttributeLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->locationInView( touch->view() );	

	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );


	for( int i = 0; i < 2; i++ )
	{
		if( NULL != m_pButtonAddSkill[i] )
		{
            if(  g_pPetDataBlock->skilllevel.skillsLevel[i] >= 3)
            {
                continue;
            }
			CCPoint pos= m_pButtonAddSkill[i]->getPosition();

			if( CCRect::CCRectContainsPoint( CCRect(pos.x - m_pButtonAddSkill[i]->getContentSize().width / 2, pos.y - m_pButtonAddSkill[i]->getContentSize().height/ 2, m_pButtonAddSkill[i]->getContentSize().width, m_pButtonAddSkill[i]->getContentSize().height ), touchLocation ) )
			{
				m_iAddSkillExpIndex = i;
				break;
			}
		}
	}
}
//-------------------------------------------------------
//
//
void CPetAttributeLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}
//-------------------------------------------------------
//
//
void CPetAttributeLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	m_iAddSkillExpIndex = -1;
}
//----------------------------------------------------------
//	CPETATTRIBUTELAYER_BUTTON_BACK,
//	CPETATTRIBUTELAYER_BUTTON_EVOLUTION
//
void CPetAttributeLayer::menuCallback(CCObject * pSender)
{
	int tag = ((CCNode*)( pSender ))->getTag();
	switch( tag )
	{
	case CPETATTRIBUTELAYER_BUTTON_BACK:
		{
			CPetWorldLayer* petWorldLayer =  ( CPetWorldLayer* )getParent();
			( (CGameWorldScene*)petWorldLayer->getParent())->EnableAllWorldButton( true );
			getParent()->removeChild(this,true);
		}
		break;
	case CPETATTRIBUTELAYER_BUTTON_EVOLUTION:
		{
			if( 0 != g_pPetDataBlock->petturning[0] )
			{
                srand((unsigned)time( NULL) ); 
                
                int d = g_pPetDataBlock->petturning[1] - g_pPetDataBlock->petturning[0];
				g_pPetDataBlock->petid = g_pPetDataBlock->petturning[0] + rand()%d + 1;
                CGameWorldScene::Show( g_pPetDataBlock->petid );
				
				g_pPetDataBlock->petturning[0] = 0;
				g_pPetDataBlock->petturning[1] = 0;
                
                memset( &g_pPetDataBlock->skilllevel, 0, sizeof( SSkillLevel ) );
                memset( &g_pPetDataBlock->skillexp, 0, sizeof( SSkillExp ) );
			}
            else
            {
                string utf81,utf82;
                CChineseCode::GB2312ToUTF_8( utf81, (char*)message_noevolution[g_iCurLanguageIndex], 256 );
                CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
                CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
                pMessage->autorelease();
                CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
                
            }
            
			//CSqliteManager::
		}
	}
}