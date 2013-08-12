#include "CMiniGameBitWood.h"
#include "../Sprite/CPet.h"
#include "../GameScene/CGameLoading.h"
#include "../CGameText.h"
#include "CNd91Mng_ios.h"
//---------------------------------------------------------------------
//
//
const char* t2dSceneObjecstakes[] = { "t2dSceneObject_stakes_1","t2dSceneObject_stakes_2","t2dSceneObject_stakes_3","t2dSceneObject_stakes_4","t2dSceneObject_stakes_5","t2dSceneObject_stakes_6","t2dSceneObject_stakes_7","t2dSceneObject_stakes_8"};

CMiniGameBitWood::CMiniGameBitWood()
{
	m_pPet = NULL;
	m_iTime = 3.0f;
	m_iStartTime = GetCurrentSeconds();
    
    m_iTotalTime = 60.0f;
    m_iMaxTime = 60.0f;
    m_bIsAttackState = false;
	m_bIsFinish		 = false;
    m_iHitCount      = 0;

}
//-----------------------------------------------------------------------
//
//
CMiniGameBitWood::~CMiniGameBitWood()
{
	
}
//------------------------------------------------------------------------
//
//
void CMiniGameBitWood::onEnter()
{
    CNd91Mng::RemoveBanner();
    CNd91Mng::InitadBanner();
    
	CCXMLLayer::onEnter();
	LoadPlist( "minigame2_hitstakes.plist" );

	//role_position
	CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( "button_back" );
    CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CMiniGameBitWood::menuCallback), 0, this, pSprite->getPosition() );
    addChild( pMenu, pSprite->getZOrder());
	RemoveNode( pSprite );

	CBaseDataAnimation *pAnimationData = g_pClientLoader->GetBaseDataAnimationFromId(g_pPetDataBlock->ResId);
    m_pBaseDataAnimation = pAnimationData; 

	m_pPet = new CPet();
	m_pPet->setPetDataBlock( g_pPetDataBlock );
	m_pPet->autorelease();

	CCNode *pNode = GetXMLNodeFromKey( "t2dSceneObject_role_position" );

	string path = pAnimationData->m_strPlist;
	m_pPet->addFramesFromiT2D( path.c_str(), pAnimationData->m_iPlistType );
	m_pPet->setPosition( pNode->getPosition() );
	m_pPet->PlayAnimation(pAnimationData->m_stand, -1, NULL, this, false, false);
	addChild(m_pPet, 100);
	
	schedule( schedule_selector( CMiniGameBitWood::UpData ) );

	srand( m_iStartTime );

	setIsTouchEnabled( true );
    
    m_pTimeSprite= (CCSprite*)GetXMLNodeFromKey( "minigame2_time" );
    m_pTimeSprite->setAnchorPoint( ccp( 0.0f, 0.5f ) );
    
    m_pTimeSprite->setPosition( ccp( m_pTimeSprite->getPosition().x -  m_pTimeSprite->getTextureRect().size.width / 2, m_pTimeSprite->getPosition().y ) );
    
    m_iTimeBar = m_pTimeSprite->getTextureRect().size.width;
    
    schedule( schedule_selector( CMiniGameBitWood::TimeUpdata ), 1.0f );
    
    pNode = GetXMLNodeFromKey( "money" );
    m_plabelName = CCLabelTTF::labelWithString("00000",  kFontSystem[FONT_GRAND].fontName,kFontSystem[FONT_GRAND].fontSize);
	m_plabelName->setPosition(pNode->getPosition());
	m_plabelName->setColor( ccBlack );
	addChild(m_plabelName,1000);
}
//----------------------------------------------------------------------------------------
//
//
void CMiniGameBitWood::UpData( ccTime tt )
{
	if( GetCurrentSeconds() - m_iStartTime  >= m_iTime  )
	{
		m_iStartTime = GetCurrentSeconds();
		AddSprite();
	}
    
    if( m_iTotalTime <= (float)m_iMaxTime * 0.8f )
    {
        m_iTime = 2;
    }
    if( m_iTotalTime <= (float)m_iMaxTime * 0.6f )
    {
        m_iTime = 1.0f;
    }
    if( m_iTotalTime <= (float)m_iMaxTime * 0.4f )
    {
        m_iTime = 0.5f;
    }

    char buffer[32];
	sprintf( buffer, "%d", g_pPetDataBlock->petmoney );
	m_plabelName->setString( buffer );
}
//------------------------------------------------------------------------
//
//
void CMiniGameBitWood::TimeUpdata( ccTime tt )
{
    m_iTotalTime--;
    if( m_iTotalTime <= 0 )
    {
        m_iTotalTime = 0;
    }
    int height = m_pTimeSprite->getTextureRect().size.height;
    CCRect rect( 0, 0, ( (float)m_iTotalTime / (float)m_iMaxTime ) * m_iTimeBar, height );
    if( NULL != m_pTimeSprite )
    {
        m_pTimeSprite->setTextureRect( rect );
    }

    if( m_iTotalTime <= 0 && !m_bIsFinish )
    {
		m_bIsFinish = true;
		if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/time_up.mp3", false);
		}

		AdvanceSprite *pAdv = new AdvanceSprite();
		pAdv->addFramesFromiT2D( "time_up.plist" );
		pAdv->autorelease();
		addChild( pAdv, 100 );

		int timeup[] = { 0,3,2 };
		pAdv->PlayAnimation( timeup, 1, NULL, this, false, true );
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pAdv->setPosition( ccp( size.width/2, size.height/2 ) );
		schedule( schedule_selector( CMiniGameBitWood::Finsh ), 3.0f ); 
    }
}
void CMiniGameBitWood::Finsh( ccTime tt )
{
	CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
}
//-----------------------------------------------------------------------------------------
//
//
void CMiniGameBitWood::MoveToAttack( CCPoint pos, float time, int tag )
{
	CCMoveTo *to = CCMoveTo::actionWithDuration( time, pos );
	CCNode *pNode = new CCNode;
	pNode->setTag( tag );
	CCFiniteTimeAction *pCallBack = CCCallFuncND::actionWithTarget( this, callfuncND_selector( CMiniGameBitWood::AttackBeginCallBack ), pNode );
	CCFiniteTimeAction *pSqueAction = CCSequence::actions( to, pCallBack, NULL );
    pSqueAction->setTag( ACTION_TAG_ATTACK_WOOD );
    m_pPet->runAction( pSqueAction );
	m_pPet->PlayAnimation( m_pBaseDataAnimation->m_move, -1, NULL, this, false, false );

    m_bIsAttackState = true;
}
//----------------------------------------------------------------------------------------
//
//
void CMiniGameBitWood::AttackFinishCallBack()
{
	m_pPet->PlayAnimation(m_pBaseDataAnimation->m_stand, -1, NULL, this, false, false);
    m_bIsAttackState = false;
    
}
//--------------------------------------------------------------------------------------
//
//
void CMiniGameBitWood::AttackBeginCallBack( CCNode *pNode, void *d )
{
	int tag = ((CCNode*)d)->getTag();
	m_pPet->PlayAnimation( m_pBaseDataAnimation->m_attack, 1, callfunc_selector( CMiniGameBitWood::AttackFinishCallBack ), this, false, false );
	if( NULL != getChildByTag( tag ) )
	{
		int frame[] = { 0,5,10 };
		( ( CWood* )getChildByTag( tag ) )->unscheduleAllSelectors();
		( ( CWood* )getChildByTag( tag ) )->PlayAnimation( frame, 1, NULL, this, false, true );
        
        std::string effectpath = GetGameParticlePath();
        std::string filename = effectpath + "hurteffect.plist";
        CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
       
        CCPoint pos = ( ( CWood* )getChildByTag( tag ) )->getPosition();
        pEmitter->setPosition( pos );
        addChild( pEmitter, 100 );
        
        if( CCUserDefault::sharedUserDefault()->getBoolForKey( "sound" ) )
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("data/audio/Hit1.mp3", false);
        }
        
        int pec = rand()%4;
        
        if( 0 == pec )
        {

        }
        else if( 1 == pec )
        {
            int data =( 1 + rand()%100 );
            g_pPetDataBlock->addPetMoney(data, m_pPet->getPosition() );
        }
        else if( 2 == pec )
        {
            int data = ( 1 + rand()%100 );
            g_pPetDataBlock->addPetExp(data, m_pPet->getPosition() );
        }
        else if( 3 == pec )
        {
            int data = ( rand()%10 );
            g_pPetDataBlock->addSkillExp(data, m_pPet->getPosition() );
        }
        g_pPetDataBlock->recordsave.HitWoodCount ++;
        m_iHitCount++;
	} 
    else
    {

        CCLabelTTF *plabel = CCLabelTTF::labelWithString("miss",  kFontSystem[FONT_GRAND].fontName,kFontSystem[FONT_GRAND].fontSize);
        static const ccColor3B ccRed = {255,0,0};
        plabel->setColor( ccRed );
        plabel->setPosition( ccp( m_pPet->getPosition().x, m_pPet->getPosition().y + m_pPet->getTextureRect().size.height )  );
        plabel->runAction( CCFadeOut::actionWithDuration(1.5f) );
        plabel->runAction(CCMoveBy::actionWithDuration(0.2f, ccp( 0, 50  )));
        addChild(plabel, 100);
    }
	((CCNode*)d)->release();
}
//---------------------------------------------------------------------------------------
//
//
void CMiniGameBitWood::AddSprite()
{
	int index = rand()%8;
	if( NULL != getChildByTag( index + TAG_BEGIN_WOOD_SPRITE ) )
	{
		return;
	}

	CWood *pWood = new CWood();
	pWood->autorelease();
	pWood->addFramesFromiT2D( "stakes.plist",1 );
	pWood->SetTime( 3.0f );
    if( m_iTotalTime <= (float)m_iMaxTime * 0.5f )
    {
        pWood->SetTime( 2.0f );
    }

	//t2dSceneObjecstakes[index];
	CCNode *pNode = GetXMLNodeFromKey( t2dSceneObjecstakes[index] );
	pWood->setPosition( pNode->getPosition() );

	int frame[] = { 0,0,1 };
	pWood->PlayAnimation( frame, - 1, NULL,this,false,false );
	addChild( pWood, 99, TAG_BEGIN_WOOD_SPRITE + index );
    
    
    std::string effectpath = GetGameParticlePath();
    std::string filename = effectpath + "spawnWood.plist";
    CCParticleSystemQuad *pEmitter = CCParticleSystemQuad::particleWithFile( filename.c_str() );
    //m_emitter->setPositionType( kCCPositionTypeRelative );
    pEmitter->setPosition( ccp( pWood->getPosition().x, pWood->getPosition().y - pWood->getContentSize().height / 2) );
    addChild( pEmitter, 100 );
}
//----------------------------------------------------------------------
//
//
void CMiniGameBitWood::menuCallback( CCObject *pObject )
{
	CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
}
//----------------------------------------------------------------------
//
//
void CMiniGameBitWood::onExit()
{
	CCXMLLayer::onExit();
    
    string utf81;
	CChineseCode::GB2312ToUTF_8( utf81, (char*)message_minigame2[g_iCurLanguageIndex], 256 );

    char temp[128];
    sprintf( temp,utf81.c_str(), m_iHitCount);
    string rewcord = temp;
    g_pPetDataBlock->addGameMessage( GetGameDayAndTime().c_str(), rewcord.c_str() );
	
}
//-----------------------------------------------------------------------
//
//
void CMiniGameBitWood::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------------------------
//
//
void CMiniGameBitWood::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	
}
//------------------------------------------------------------------------
//
//
void CMiniGameBitWood::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    if( NULL != m_pPet->getActionByTag(ACTION_TAG_ATTACK_WOOD) || m_bIsAttackState)
    {
        return;
    }
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->locationInView( touch->view() );	

	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	
	for( int i = 0; i < 8; i++ )
	{
		CCNode *pNode = GetXMLNodeFromKey( t2dSceneObjecstakes[i] );

		CCPoint pos= pNode->getPosition();

		if( CCRect::CCRectContainsPoint( CCRect(pos.x - pNode->getContentSize().width / 2, pos.y - pNode->getContentSize().height/ 2, pNode->getContentSize().width, pNode->getContentSize().height ), touchLocation ) )
		{
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			int width	= size.width / 5.0f;
			int distance = abs( pos.x - m_pPet->getPosition().x );
            
            if( NULL != getChildByTag( i + TAG_BEGIN_WOOD_SPRITE ) )
            {
				string path = GetGameImagesPath();
				path = path + "sight.png";
				CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
				addChild( pSprite, 100 );
				pSprite->setScale(3);
				pSprite->setPosition( pos );
				pSprite->runAction(  CCScaleTo::actionWithDuration(0.2, 0.5));
				pSprite->runAction( CCFadeOut::actionWithDuration( 3.0f ) );

                if( pos.x > m_pPet->getPosition().x )
                {
                    MoveToAttack( ccp( pos.x - pNode->getContentSize().width / 2, pos.y ), (float)distance / (float)width * 0.5, TAG_BEGIN_WOOD_SPRITE + i );
                    m_pPet->setFlipX( true );
                }
                else
                {
                    MoveToAttack( ccp( pos.x + pNode->getContentSize().width / 2, pos.y ), (float)distance / (float)width * 0.5, TAG_BEGIN_WOOD_SPRITE + i );
                    m_pPet->setFlipX( false );
                }
                break;
            }
		}

	}
}
//------------------------------------------------------------------------
//
//
CWood::CWood()
{
	m_tTime = 3.0f;
}
//-----------------------------------------------------------------------
//
//
CWood::~CWood()
{

}
//-----------------------------------------------------------------------
//
//
void CWood::onEnter()
{
	AdvanceSprite::onEnter();
	schedule( schedule_selector( CWood::scheduleDelete ), m_tTime );
}
//-------------------------------------------------------------------
//
//
void CWood::onExit()
{
	AdvanceSprite::onExit();
}
//---------------------------------------------------------------------
//
//
void CWood::scheduleDelete( ccTime tt )
{
	removeFromParentAndCleanup( true );
}
