#include "CMiniGameGamblingLayer.h"
#include "../GameScene/CGameLoading.h"
#include "../Sprite/CPet.h"
#include "../loader/CClientLoader.h"
#include "../Pushui/CMessageBoxYes.h"
#include "CNd91Mng_ios.h"

CMiniGameGamblingLayer::CMiniGameGamblingLayer()
{
	setIsTouchEnabled( true );
	m_bIsTouch = false;

	m_plabelMoney		= NULL;
	m_plabelWinMoney	= NULL;
	m_plabelLostMoney	= NULL;
	m_iWinMoney = 0;
    m_iRollIndex = 0;
}
//----------------------------------------------------------
//
//
CMiniGameGamblingLayer::~CMiniGameGamblingLayer()
{

}
//---------------------------------------------------------
//
//
void CMiniGameGamblingLayer::onEnter()
{
    CNd91Mng::RemoveBanner();
    CNd91Mng::InitadBanner();
    
	CCXMLLayer::onEnter();
    
    LoadPlist( "minigame3_gambling.plist");

	CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( "button_back" );
    CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CMiniGameGamblingLayer::menuCallback), 0, this, pSprite->getPosition() );
    addChild( pMenu, pSprite->getZOrder());
    removeChild( pSprite, true );

	pSprite= (CCSprite*)GetXMLNodeFromKey( "pushrod" );
	
	m_pPushBar = pSprite;

	m_pPushBar->setAnchorPoint( ccp( 0.5f,0.0f ) );

	m_pPushBar->setPosition( ccp( m_pPushBar->getPosition().x, m_pPushBar->getPosition().y - m_pPushBar->getTextureRect().size.height / 2 ) );


	m_pFood[0] = (AdvanceSprite*)GetXMLNodeFromKey( "t2dAnimatedSprite_food_1" );

	m_pFood[1] = (AdvanceSprite*)GetXMLNodeFromKey( "t2dAnimatedSprite_food_2" );

	m_pFood[2] = (AdvanceSprite*)GetXMLNodeFromKey( "t2dAnimatedSprite_food_3" );

	CCNode *pNode = GetXMLNodeFromKey( "t2dSceneObject_money" );
    m_plabelMoney = CCLabelTTF::labelWithString("0",  kFontSystem[FONT_GRAND].fontName,kFontSystem[FONT_GRAND].fontSize);
	m_plabelMoney->setPosition(pNode->getPosition());
	m_plabelMoney->setColor( ccBlack );
	addChild(m_plabelMoney,100);

	pNode = GetXMLNodeFromKey( "t2dSceneObject_winmoney" );
	m_plabelWinMoney = CCLabelTTF::labelWithString("0",  kFontSystem[FONT_SMALL].fontName,kFontSystem[FONT_SMALL].fontSize);
	m_plabelWinMoney->setPosition(pNode->getPosition());
	m_plabelWinMoney->setColor( ccBlack );
	addChild(m_plabelWinMoney,100);

	pNode = GetXMLNodeFromKey( "t2dSceneObject_lostmoney" );
	m_plabelLostMoney = CCLabelTTF::labelWithString("200",  kFontSystem[FONT_SMALL].fontName,kFontSystem[FONT_SMALL].fontSize);
	m_plabelLostMoney->setPosition(pNode->getPosition());
	m_plabelLostMoney->setColor( ccBlack );
	addChild(m_plabelLostMoney,100);

	schedule( schedule_selector( CMiniGameGamblingLayer::UpData ), 0.1f );


	CCSize size = CCDirector::sharedDirector()->getWinSize();


	string path = GetGameImagesPath();
	path = path + "lose.png";
	m_pSpriteLost = CCSprite::spriteWithFile( path.c_str() );
	addChild( m_pSpriteLost, 100 );
	m_pSpriteLost->setPosition( ccp( size.width/2, size.height/2 ) );
	m_pSpriteLost->setIsVisible( false );

	path = GetGameImagesPath();
	path = path + "win.png";
	m_pSpriteWin = CCSprite::spriteWithFile( path.c_str() );
	addChild( m_pSpriteWin, 100 );
	m_pSpriteWin->setPosition( ccp( size.width/2, size.height/2 ) );
	m_pSpriteWin->setIsVisible( false );

}

void CMiniGameGamblingLayer::UpData( ccTime tt )
{
	char buffer[32];
	sprintf( buffer, "%d", g_pPetDataBlock->petmoney );
	m_plabelMoney->setString( buffer );

	sprintf( buffer, "%d", m_iWinMoney );
	m_plabelWinMoney->setString( buffer );
}

void CMiniGameGamblingLayer::BeginRote( ccTime tt )
{
	unschedule(schedule_selector(CMiniGameGamblingLayer::BeginRote));

	m_pFood[0]->resumeAnimation();
	m_pFood[1]->resumeAnimation();
	m_pFood[2]->resumeAnimation();

	m_bIsTouch = false;

	m_pSpriteLost->setIsVisible( false );
	m_pSpriteWin->setIsVisible( false );
}
//----------------------------------------------------------
//
//
void CMiniGameGamblingLayer::onExit()
{
	CCXMLLayer::onExit();
}
//-----------------------------------------------------------
//
//
void CMiniGameGamblingLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if( g_pPetDataBlock->petmoney < 200 )
	{
		string utf81,utf82;
		CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoney[g_iCurLanguageIndex], 256 );
		CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
		CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
		pMessage->autorelease();
		CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
		return;
	}

	if( m_bIsTouch )
	{
		return;
	}

	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->locationInView( touch->view() );	
	CCPoint prevLocation = touch->previousLocationInView( touch->view() );	
	
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	prevLocation = CCDirector::sharedDirector()->convertToGL( prevLocation );

	CCPoint pos = convertTouchToNodeSpace( touch );

	CCPoint menuPos = m_pPushBar->getPosition();

	if( CCRect::CCRectContainsPoint( CCRect( menuPos.x - 32, menuPos.y, 480 / 2, 320 / 2 ),  pos  ) )
	{
		CCActionInterval *pInterval = CCRotateBy::actionWithDuration( 0.5, 60 );
		CCActionInterval *pInterval2 = CCRotateBy::actionWithDuration( 0.5, -60 );
		m_pPushBar->runAction( CCSequence::actions( pInterval, pInterval2, CCCallFuncND::actionWithTarget( this, callfuncND_selector( CMiniGameGamblingLayer::TouchCallBack ), NULL ),  NULL ) );

		m_bIsTouch = true;
        schedule(schedule_selector(CMiniGameGamblingLayer::scheculePauseData), 0.1 );
	}


}

void CMiniGameGamblingLayer::scheculePauseData( ccTime tt )
{
    unschedule(schedule_selector(CMiniGameGamblingLayer::scheculePauseData) );
    m_pFood[m_iRollIndex]->pauseAnimation();
    m_Code[m_iRollIndex] = m_pFood[m_iRollIndex]->GetCurentIndex(); 
    m_iRollIndex++;
    
    if( m_iRollIndex >= 3 )
    {
        for( int i = 0; i < 3; i++ )
        {
            for( int j = 0; j < 3 - i - 1; j++ )
            {
                if( m_Code[j] > m_Code[j + 1] )
                {
                    int temp = m_Code[j];
                    m_Code[j] = m_Code[j + 1];
                    m_Code[j + 1] = temp;
                }
            }
        }
        
        if( CheckIsWin() )
        {
            m_pSpriteWin->setIsVisible( true );
        }
        else
        {
            g_pPetDataBlock->petmoney -=200;
            m_pSpriteLost->setIsVisible( true );
        }
        
        CCLOG( "%d,%d,%d" ,m_Code[0],m_Code[1],m_Code[2] );
    }
}
//-------------------------------------------------------------
//
//
bool CMiniGameGamblingLayer::CheckIsWin()
{
	for( int i = 1;;i++ )
	{
		CBaseDataScore* pBaseData = g_pClientLoader->GetBaseDataScore( i );
		if( NULL == pBaseData )
		{
			break;
		}
		if( pBaseData->index[0] == m_Code[0] && pBaseData->index[1] == m_Code[1] && pBaseData->index[2] == m_Code[2] )
		{
			CCSize size = CCDirector::sharedDirector()->getWinSize();
			g_pPetDataBlock->addPetMoney( ( 200 * pBaseData->score ), ccp( size.width / 2, size.height / 2 )  );
			m_iWinMoney += 200 * pBaseData->score;
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------
//
//
void CMiniGameGamblingLayer::TouchCallBack( CCNode *pNode, void *d )
{
    if( m_iRollIndex >= 3 )
    {
        schedule(schedule_selector(CMiniGameGamblingLayer::BeginRote),3.0f);
        m_iRollIndex = 0;
    }
    else
    {
        m_bIsTouch = false;
    }
}
//-------------------------------------------------------------
//
//
void CMiniGameGamblingLayer::menuCallback(CCObject * pSender)
{
	CGameLoading::Show( LOADING_CGAMEWORLD_SCENE );
}

