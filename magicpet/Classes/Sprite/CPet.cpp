/*****************************************************************************/
//File name:	CPet
//Author: 		huyan
//Description:	
/******************************************************************************/
#include "CPet.h"
#include "../loader/CClientLoader.h"
#include "../Fighting/CFightGroundLayer.h"
#include "../Function/CExpressions.h"

PetDatablock *g_pPetDataBlock = NULL;

PetDatablock::PetDatablock()
{
	pFeedPetData = NULL;
}

PetDatablock::~PetDatablock()
{
	CC_SAFE_DELETE( pFeedPetData );
}


void PetDatablock::addItem( int Id, int count )
{
	bool isHave = false;
	for( int i = 0; i < MAX_ITEM_HAVE_DFINE; i++ )
	{
		if( pFeedPetData->feedItem[i].itemId == Id )
		{
			pFeedPetData->feedItem[i].itemCount += count;
			isHave = true;
			break;
		}
	}
	if( !isHave )
	{
		for( int i = 0; i < MAX_ITEM_HAVE_DFINE; i++ )
		{
			if( pFeedPetData->feedItem[i].itemId == 0 )
			{
				pFeedPetData->feedItem[i].itemCount = count;
				pFeedPetData->feedItem[i].itemId = Id;
				itemnum++;
				break;
			}
		}
	}
}
SFeedDataItem *PetDatablock::GetSaveItemFromId( int ID )
{
	for( int i = 0; i < MAX_ITEM_HAVE_DFINE; i++ )
	{
		if( pFeedPetData->feedItem[i].itemId == ID )
		{
			return &pFeedPetData->feedItem[i];
		}
	}
	return NULL;
}
//-----------------------------------------------------------
//
//
bool PetDatablock::subPlayershoppingmallmoney( int num )
{
    if( playershoppingmallmoney >= num )
    {
        playershoppingmallmoney = playershoppingmallmoney - num;
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/zuanshi.mp3", false);
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------
//
//
bool PetDatablock::subPetMoney( int num )
{
    if( petmoney >= num )
    {
        petmoney = petmoney - num;
        return true;
    }
    return false;
}
//------------------------------------------------------------
//
//
bool PetDatablock::submaxstaminapoint( int num )
{
    if( maxstaminapoint >= num )
    {
        maxstaminapoint = maxstaminapoint - num;
        return true;
    }
    return false;
}
//-----------------------------------------------------------
//
//
void PetDatablock::addPetMoney( int data, CCPoint pos )
{
	petmoney += data;

	CCNode *pNode = CCNode::node();
	
	string path = GetGameImagesPath();
    path = path + "upmoney.png";
	CCSprite* pSpriteBG = CCSprite::spriteWithFile( path.c_str() );
	pSpriteBG->setPosition( pos );

	pNode->addChild( pSpriteBG, 100 );

	char buffer[128];
	sprintf( buffer,"+%d", data );
	CCLabelTTF *pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize );
	pLabelTTF->setPosition( ccp( pSpriteBG->getPosition().x + pSpriteBG->getTextureRect().size.width / 2, pSpriteBG->getPosition().y )  );
	pLabelTTF->setAnchorPoint( ccp( 0,0.5 ) );
	pNode->addChild( pLabelTTF,100 );

	CCDirector::sharedDirector()->getRunningScene()->addChild( pNode, 100 );

	pSpriteBG->runAction(CCFadeOut::actionWithDuration(3.0));
	pLabelTTF->runAction(CCFadeOut::actionWithDuration(3.0));

	CCMoveBy *pMoveBy = CCMoveBy::actionWithDuration( 2.0, ccp( 0, CCDirector::sharedDirector()->getWinSize().height / 3 ) );
    pNode->runAction( pMoveBy );

	if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/money.mp3", false);
	}
}
//-------------------------------------------------------
//
//
void PetDatablock::addGameMessage( string gameday, string message, int type, string name )
{
	bool isAddSuccess = false;
	for( int i = 0; i < MAX_PET_DATA_SAVE_MESSAGE; i ++ )
	{
		if( !recordsave.saveMessage[i].isHas )
		{
			strcpy( recordsave.saveMessage[i].strMessage , message.c_str() );
			strcpy( recordsave.saveMessage[i].gameDay	 , gameday.c_str() );
			recordsave.saveMessage[i].type = type;
			recordsave.saveMessage[i].isHas = true;
            strcpy( recordsave.saveMessage[i].name, name.c_str() );
			isAddSuccess = true;
			break;
		}
	}
	if( !isAddSuccess )
	{
		for( int i = 0; i < MAX_PET_DATA_SAVE_MESSAGE - 1; i ++ )
		{
			memcpy( &recordsave.saveMessage[i], &recordsave.saveMessage[i + 1], sizeof( SPetDataSaveMessage ) );
		}
		recordsave.saveMessage[MAX_PET_DATA_SAVE_MESSAGE - 1].isHas = false;

		addGameMessage( gameday, message, type, name );
	}

	recordsave.isHaveNewMessage = true;
}
//--------------------------------------------------------------
//
//
void PetDatablock::addPlayerMoney( int data, CCPoint pos )
{
	playershoppingmallmoney += data;

	CCNode *pNode = CCNode::node();
	
	string path = GetGameImagesPath();
    path = path + "upPlayerMoney.png";
	CCSprite* pSpriteBG = CCSprite::spriteWithFile( path.c_str() );
	pSpriteBG->setPosition( pos );

	pNode->addChild( pSpriteBG, 100 );

	char buffer[128];
	sprintf( buffer,"+%d", data );
	CCLabelTTF *pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize );
	pLabelTTF->setPosition( ccp( pSpriteBG->getPosition().x + pSpriteBG->getTextureRect().size.width / 2, pSpriteBG->getPosition().y )  );
	pLabelTTF->setAnchorPoint( ccp( 0,0.5 ) );
	pNode->addChild( pLabelTTF,100 );

	CCDirector::sharedDirector()->getRunningScene()->addChild( pNode, 100 );

	pSpriteBG->runAction(CCFadeOut::actionWithDuration(3.0));
	pLabelTTF->runAction(CCFadeOut::actionWithDuration(3.0));

	CCMoveBy *pMoveBy = CCMoveBy::actionWithDuration( 2.0, ccp( 0, CCDirector::sharedDirector()->getWinSize().height / 3 ) );
    pNode->runAction( pMoveBy );
}
//---------------------------------------------------------
//
//
void PetDatablock::addPetExp( int data, CCPoint pos )
{
	petexp += data;

	CCNode *pNode = CCNode::node();
	
	string path = GetGameImagesPath();
    path = path + "upexp.png";
	CCSprite* pSpriteBG = CCSprite::spriteWithFile( path.c_str() );
	pSpriteBG->setPosition( pos );

	pNode->addChild( pSpriteBG, 100 );

	char buffer[128];
	sprintf( buffer,"+%d", data );
	CCLabelTTF *pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize );
	pLabelTTF->setPosition( ccp( pSpriteBG->getPosition().x + pSpriteBG->getTextureRect().size.width / 2, pSpriteBG->getPosition().y )  );
	pLabelTTF->setAnchorPoint( ccp( 0,0.5 ) );
	pNode->addChild( pLabelTTF,100 );

	CCDirector::sharedDirector()->getRunningScene()->addChild( pNode, 100 );

	pSpriteBG->runAction(CCFadeOut::actionWithDuration(3.0));
	pLabelTTF->runAction(CCFadeOut::actionWithDuration(3.0));
	CCMoveBy *pMoveBy = CCMoveBy::actionWithDuration( 2.0, ccp( 0, CCDirector::sharedDirector()->getWinSize().height / 3 ) );
    pNode->runAction( pMoveBy );
}
//----------------------------------------------------------
//
//
void PetDatablock::addSkillExp( int skillExp, CCPoint pos )
{
	petskillexppond += skillExp;
	if( petskillexppond >= MAX_SKILL_EXP )
	{
		petskillexppond = MAX_SKILL_EXP;
	}

	CCNode *pNode = CCNode::node();
	
	string path = GetGameImagesPath();
    path = path + "upskillexp.png";
	CCSprite* pSpriteBG = CCSprite::spriteWithFile( path.c_str() );
	pSpriteBG->setPosition( pos );

	pNode->addChild( pSpriteBG, 100 );

	char buffer[128];
	sprintf( buffer,"+%d", skillExp );
	CCLabelTTF *pLabelTTF = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize );
	pLabelTTF->setPosition( ccp( pSpriteBG->getPosition().x + pSpriteBG->getTextureRect().size.width / 2, pSpriteBG->getPosition().y )  );
	pLabelTTF->setAnchorPoint( ccp( 0,0.5 ) );
	pNode->addChild( pLabelTTF,100 );

	CCDirector::sharedDirector()->getRunningScene()->addChild( pNode, 100 );

	pSpriteBG->runAction(CCFadeOut::actionWithDuration(3.0));
	pLabelTTF->runAction(CCFadeOut::actionWithDuration(3.0));
	CCMoveBy *pMoveBy = CCMoveBy::actionWithDuration( 2.0, ccp( 0, CCDirector::sharedDirector()->getWinSize().height / 3 ) );
    pNode->runAction( pMoveBy );

}
//------------------------------------------------------------
//
//
CPet::CPet()
{
	m_pLockTarget = NULL;
	m_iState      = CPET_STATE_NONE;
    m_bIsWin      = false;
}
//---------------------------
//
//
CPet::~CPet()
{
    int c = 0;
}
//-----------------------------------
//
//
void CPet::onEnter()
{
	AdvanceSprite::onEnter();

	schedule(schedule_selector(CPet::upMainData));

	//string path = GetGameImagesPath();
	//path = path + "shadow.png";
	//CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
	//addChild( pSprite,-10 );
	//pSprite->setPosition( ccp( getTextureRect().size.width / 2, getTextureRect().size.height / 2 ) );
}
//-----------------------------------------
//
//
void CPet::onExit()
{
	AdvanceSprite::onExit();
}
//-----------------------------------------------
//
//
void CPet::setTurnEndCallBack( SelectorProtocol* pSelectorTarget, SEL_CallFuncND selector, void* d )
{
	m_pCallFuncND = selector;
	m_pSelectorTarget = pSelectorTarget;
}
//-----------------------------------------------
//
//
void CPet::upMainData( ccTime dt )
{
	if( CPET_STATE_DISTANCE_ATTACK == m_iState )
	{
		//int index = GetCurentIndex();
		CBaseDataPet* pDataPet = g_pClientLoader->GetBaseDataPetFromId(m_PetData.petid);
		CBaseDataSkill* pDataSkill = g_pClientLoader->GetBaseDataSkillFromId(pDataPet->skillfield[m_PetData.skilllevel.skillsLevel[0]-1]);

		if( pDataSkill->attack_frame <= GetCurentIndex())
		{
			CCMoveTo *to1 = CCMoveTo::actionWithDuration( 1.0f, m_pLockTarget->getPosition() );

			SAttackType *pAttackType = new SAttackType();
			pAttackType->Type = 1;
			CCFiniteTimeAction *pCallBack = CCCallFuncND::actionWithTarget( this, callfuncND_selector( CPet::AttackBeginCallBack ), pAttackType );

			std::string effectpath = GetGameParticlePath();
			std::string filename = effectpath + pDataSkill->plist.c_str();
			CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
			//panda
            if( m_PetData.petid >= 41 && m_PetData.petid <= 50 )
            {
                pEmitter->setPositionType( kCCPositionTypeGrouped );
            }
			if( FIGHT_LEFT_PLAYER_POS == m_iSitId )
			{
				pEmitter->setPosition( ccp( getPosition().x + 32, getPosition().y ) );
			}
			else
			{
				pEmitter->setPosition( ccp( getPosition().x - 16, getPosition().y ) );
			}
            
            if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
            {
                string path = "data/audio/" + pDataSkill->skill_sound;
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(path.c_str(), false);
            }
            
			CCPoint pos = getPosition();
			g_pFightGroundLayer->addChild( pEmitter, 100, FIRE_BALL_TAG );

			pEmitter->runAction( CCSequence::actions( to1, pCallBack, NULL ) );

			m_iState = CPET_STATE_NONE;
		}
	}
}
//-----------------------------------------------
//
//
void CPet::Attack()
{
	//normal attack
	CBaseDataPet* pPetBaseData = g_pClientLoader->GetBaseDataPetFromId(m_PetData.petid);
	int skilldata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, m_PetData.petlevel );
	float pec = CExpressions::GetInstance()->GetSkillFrquency(skilldata, m_PetData.petlevel );

	//skill attack
    CBaseDataPet* pDataPet = g_pClientLoader->GetBaseDataPetFromId(m_PetData.petid);
    CBaseDataSkill* pDataSkill = g_pClientLoader->GetBaseDataSkillFromId(pDataPet->skillfield[m_PetData.skilllevel.skillsLevel[0]-1]);
	if( CCRANDOM_0_1() < pec && m_PetData.skilllevel.skillsLevel[0] > 0 && m_PetData.mp >= pDataSkill->skillmp )
	{
		m_iState = CPET_STATE_DISTANCE_ATTACK;
		CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
		PlayAnimation( pAnimationData->m_skill, 1, callfunc_selector( CPet::HurtEndCallBack ), this, false, false );
        
		m_PetData.mp = m_PetData.mp - pDataSkill->skillmp;
		if( m_PetData.mp <= 0 )
		{
			m_PetData.mp = 0;
		}
	}
	else
	{
		CCMoveTo *to1 = CCMoveTo::actionWithDuration( 0.4f, m_TargetPositon );
		SAttackType *pAttackType = new SAttackType();
		pAttackType->Type = 0;
		CCFiniteTimeAction *pCallBack = CCCallFuncND::actionWithTarget( this, callfuncND_selector( CPet::AttackBeginCallBack ), pAttackType );
		runAction( CCSequence::actions( to1, pCallBack, NULL ) );
		//CBaseDataPet *pBaseDataPet = g_pClientLoader->GetBaseDataPetFromId(getID());
		CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
		PlayAnimation( pAnimationData->m_move, -1, NULL, this, false, false );
	}
}
//--------------------------------------------------
//
//
void CPet::AttackBeginCallBack( CCNode *pNode, void *d )
{
	//CBaseDataPet *pBaseDataPet = g_pClientLoader->GetBaseDataPetFromId(getID());
	//¬∫¬∫∆í‚Ä?
	//CBaseDataSkill *pBaseDataSkill = g_pClientLoader->GetBaseDataSkillFromId(pBaseDataPet->m_arrSkillId[0]);
	//startAnimation( pBaseDataSkill->m_arrSkillId[0],pBaseDataSkill->m_arrSkillId[1],1,callfunc_selector(CPet::AttackEndCallBack),this,pBaseDataSkill->m_arrSkillId[2],false,false );

	SAttackType *pAttackType = ( ( SAttackType* )d);
	if( CPET_STATE_NONE == pAttackType->Type )
	{
		CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
		PlayAnimation( pAnimationData->m_attack, 1, callfunc_selector(CPet::AttackEndCallBack), this, false, false );

		CBaseDataPet* pPetBaseData = g_pClientLoader->GetBaseDataPetFromId(m_PetData.petid);
		//‚àÜ‚Äô√ï¬ÆœÄ‚Ä¢¬™Àú¬±¬©¬™À?
		int damage = CExpressions::GetInstance()->GetDamage( CExpressions::GetInstance()->GetAttribute( pPetBaseData->dexaptitude, pPetBaseData->updex, pPetBaseData->initdex, m_PetData.petlevel ) );
		
	
		int skilldata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, m_PetData.petlevel );
		float pec = CExpressions::GetInstance()->GetSkillFrquency(skilldata, m_PetData.petlevel );
		if( CCRANDOM_0_1() <( pec / 2.0f ) )
		{
			m_pLockTarget->ApplyHurt(  damage + damage / 2 + rand() % (damage / 2 ), true );
		}
		else if( CCRANDOM_0_1() < 0.9f )
		{
			m_pLockTarget->ApplyHurt(  damage  );
		}
		else
		{
			m_pLockTarget->ApplyHurt(  0  );
		}
        
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
        {
            string path = "data/audio/" + pPetBaseData->attack_sound;
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(path.c_str(), false);
        }

	}
	else
	{
		CCFiniteTimeAction *pTime = new CCFiniteTimeAction();
		pTime->setDuration( 1.2f );
		pTime->autorelease();

		CCFiniteTimeAction* pTimeAction = CCCallFuncN::actionWithTarget( this, callfuncN_selector(CPet::AttackOverCallBack) );

		runAction( CCSequence::actions( pTime, pTimeAction, NULL ) );
        
        g_pFightGroundLayer->removeChildByTag( FIRE_BALL_TAG, true );

		CBaseDataPet* pDataPet = g_pClientLoader->GetBaseDataPetFromId(m_PetData.petid);
		CBaseDataSkill* pDataSkill = g_pClientLoader->GetBaseDataSkillFromId(pDataPet->skillfield[m_PetData.skilllevel.skillsLevel[0]-1]);
		m_pLockTarget->ApplyHurt( pDataSkill->damage );

		if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/Hit1.mp3", false);
		}

	}
}
//---------------------------------------------------
//
//
void CPet::AttackEndCallBack()
{
	//comeback
	CCMoveTo *to2 = CCMoveTo::actionWithDuration( 0.4f, m_StandPositon);
	CCFiniteTimeAction *pCallBack2 = CCCallFuncN::actionWithTarget( this, callfuncN_selector( CPet::AttackOverCallBack ) );
	runAction( CCSequence::actions( to2, pCallBack2, NULL ) );

	//CBaseDataPet *pBaseDataPet = g_pClientLoader->GetBaseDataPetFromId(getID());
	//startAnimation( pBaseDataPet->m_arrRunAni[0],pBaseDataPet->m_arrRunAni[1],-1,NULL,NULL,pBaseDataPet->m_arrRunAni[2],false,false );
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
	PlayAnimation( pAnimationData->m_move, -1, NULL, this, false, false );
    
}
//--------------------------------------------------
//
//
void CPet::AttackOverCallBack( CCNode *pNode )
{
	//CBaseDataPet *pBaseDataPet = g_pClientLoader->GetBaseDataPetFromId(getID());
	//startAnimation( pBaseDataPet->m_arrStandAni[0],pBaseDataPet->m_arrStandAni[1],-1,NULL,NULL,pBaseDataPet->m_arrStandAni[2],false,false );

	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
    if( m_bIsWin )
    {
        PlayAnimation( pAnimationData->m_win, 1, NULL, this, false, false );
    }
    else
    {
        PlayAnimation( pAnimationData->m_stand, -1, NULL, this, false, false );
    }

	int sitId = m_pLockTarget->getSitId();
	if(m_pCallFuncND)
	{
		CCNode* pNode = new CCNode();
		pNode->setTag(sitId);
		(m_pSelectorTarget->*m_pCallFuncND)(NULL, pNode);
	}
}
//-----------------------------------------------------
//
//
void CPet::setTargetPosition( CCPoint pos )
{
	m_TargetPositon.x = pos.x;
	m_TargetPositon.y = pos.y;
	m_StandPositon = getPosition();
}
//---------------------------------------------------
//
//
void CPet::setLockTarget(  CPet* pet )
{
	m_pLockTarget = pet;
}
//---------------------------------------------------
//
void CPet::setPetDataBlock( void* pData )
{
     m_PetData.petid                    = ( ( PetDatablock* )pData)->petid;
     m_PetData.petsex                   = ( ( PetDatablock* )pData)->petsex;
     m_PetData.SkillFrequency           = ( ( PetDatablock* )pData)->SkillFrequency;
     m_PetData.petName                  = ( ( PetDatablock* )pData)->petName;
     m_PetData.petlevel                 = ( ( PetDatablock* )pData)->petlevel;
     m_PetData.petexp                   = ( ( PetDatablock* )pData)->petexp;
     m_PetData.skilllevel               = ( ( PetDatablock* )pData)->skilllevel;
     m_PetData.skillexp                 = ( ( PetDatablock* )pData)->skillexp;
     m_PetData.pettitle                 = ( ( PetDatablock* )pData)->pettitle;
     m_PetData.playershoppingmallmoney  = ( ( PetDatablock* )pData)->playershoppingmallmoney;
     m_PetData.maxstaminapoint          = ( ( PetDatablock* )pData)->maxstaminapoint;
     m_PetData.petskillexppond          = ( ( PetDatablock* )pData)->petskillexppond;
     m_PetData.itemnum                  = ( ( PetDatablock* )pData)->itemnum;
     m_PetData.petmoney                 = ( ( PetDatablock* )pData)->petmoney;
     m_PetData.ResId                    = ( ( PetDatablock* )pData)->ResId;
     m_PetData.playername               = ( ( PetDatablock* )pData)->playername;
     m_PetData.userid                   = ( ( PetDatablock* )pData)->userid;

	 CBaseDataPet *pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( m_PetData.petid  );
	 m_PetData.hp = pPetBaseData->inithp;
	 m_PetData.mp = pPetBaseData->initmp;
	 for( int i = 1; i <= m_PetData.petlevel; i++ )
	 {
		 m_PetData.hp += CExpressions::GetInstance()->GetAttribute( pPetBaseData->straptitude, pPetBaseData->upstr, pPetBaseData->initstr, i );
		 m_PetData.mp += CExpressions::GetInstance()->GetAttribute( pPetBaseData->intaptitude, pPetBaseData->upint, pPetBaseData->initint, i );
	 }
	 m_PetData.MaxHp                    = m_PetData.hp;
     m_PetData.MaxMp                    = m_PetData.mp;

     memcpy( m_PetData.rbg, ( ( PetDatablock* )pData)->rbg, sizeof( int  ) * 3  );
    
	ccColor3B ccRED={m_PetData.rbg[0],m_PetData.rbg[1],m_PetData.rbg[2]};	
	setColor( ccRED );
}
//--------------------------------------------------
//
//
void CPet::ApplyHurt( int damage, bool isHardAttack )
{
	CBaseDataPet* pPetBaseData = g_pClientLoader->GetBaseDataPetFromId(m_PetData.petid);
	int skilldata = CExpressions::GetInstance()->GetAttribute( pPetBaseData->skillaptitude, pPetBaseData->upskill, pPetBaseData->initskill, m_PetData.petlevel );
	float pec = CExpressions::GetInstance()->GetSkillFrquency(skilldata, m_PetData.petlevel );

    CBaseDataSkill* pDataSkill = g_pClientLoader->GetBaseDataSkillFromId(pPetBaseData->skillfield_defense[m_PetData.skilllevel.skillsLevel[1] - 1]);
	//skill attack
	if( CCRANDOM_0_1() < pec && m_PetData.skilllevel.skillsLevel[1] > 0 && m_PetData.mp >= pDataSkill->skillmp )
	{
		CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
		PlayAnimation( pAnimationData->m_defense, 1, callfunc_selector( CPet::HurtEndCallBack ), this, false, false );

		m_PetData.mp = m_PetData.mp - pDataSkill->skillmp;
		if( m_PetData.mp <= 0 )
		{
			m_PetData.mp = 0;
		}
        std::string effectpath = GetGameParticlePath();
        std::string filename = effectpath +  pDataSkill->plist;
        if( !pDataSkill->plist.empty() )
        {
            CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile(filename.c_str() );
            //m_emitter->setPositionType( kCCPositionTypeRelative );
            pEmitter->setPosition( getPosition() );
            g_pFightGroundLayer->addChild( pEmitter, 100 );
        }
        
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
        {
            string path = "data/audio/" + pDataSkill->skill_sound;
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(path.c_str(), false);
            
             CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/scoreCount2.wav", false);
        }

        
		return;
	}


	m_PetData.hp =  m_PetData.hp - damage;
    char strbuffer[32];
	if( 0 == damage )
	{
		sprintf( strbuffer, "%s",  "miss");
	}
	else
	{
		sprintf( strbuffer, "-%d",  damage);
	}
	CCLabelTTF *plabel = CCLabelTTF::labelWithString(strbuffer,  kFontSystem[FONT_GRAND].fontName,kFontSystem[FONT_GRAND].fontSize);
    const ccColor3B ccwhite      = {255,255,255};
    const ccColor3B ccYellow     = {255,255,0};
    const ccColor3B ccRed        = {255,0,0 };
    if( isHardAttack )
    {
        plabel->setColor( ccYellow );
    }
    else if( 0 == damage )
    {
         plabel->setColor( ccRed );
    }
    else
    {
        plabel->setColor( ccwhite );
    }
	plabel->setPosition( ccp( getPosition().x, getPosition().y + getTextureRect().size.height )  );
    g_pFightGroundLayer->addChild(plabel,100);
    plabel->runAction( CCFadeOut::actionWithDuration(1.5f) );
    plabel->runAction(CCMoveBy::actionWithDuration(0.2f, ccp( 0, 50  )));
    

	if( m_PetData.hp <= 0 )
	{
		CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
		PlayAnimation( pAnimationData->m_lost, 1, NULL, this, false, false );
        m_pLockTarget->m_bIsWin = true;
	}
	else
	{
		CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
		PlayAnimation( pAnimationData->m_injured, 1, callfunc_selector( CPet::HurtEndCallBack ), this, false, false );
	}
    
	if( 0 != damage )
	{
		std::string effectpath = GetGameParticlePath();
		std::string filename = effectpath + "hurteffect.plist";
		CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
		//m_emitter->setPositionType( kCCPositionTypeRelative );
		pEmitter->setPosition( getPosition() );
		CCPoint pos = getPosition();
		g_pFightGroundLayer->addChild( pEmitter, 100 );
	}
    if( isHardAttack )
    {
        std::string effectpath = GetGameParticlePath();
		std::string filename = effectpath + "bloodEffect.plist";
		CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
		//m_emitter->setPositionType( kCCPositionTypeRelative );
		pEmitter->setPosition( getPosition() );
		CCPoint pos = getPosition();
		g_pFightGroundLayer->addChild( pEmitter, 100 );
    }
    
}
//--------------------------------------
//
//
void CPet::HurtEndCallBack()
{
	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(m_PetData.ResId);
    PlayAnimation( pAnimationData->m_stand, -1, NULL, this, false, false );
}