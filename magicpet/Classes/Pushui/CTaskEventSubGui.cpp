/*****************************************************************************/
//File name:	CTaskEventSubGui.h
//Author: 		huyan
//Description:	任务界面、
/******************************************************************************/
#include "CTaskEventSubGui.h"
#include "../GameScene/CGameWorldScene.h"
#include "CEventControl.h"
#include "../loader/CClientLoader.h"
#include "../CGameMessage.h"
#include "../CChineseCode.h"


CTaskEventSubGui::CTaskEventSubGui()
{

}
//-------------------------------------------------------------
//
//
CTaskEventSubGui::~CTaskEventSubGui()
{

}
//-----------------------------------------------------------------
//
//
void CTaskEventSubGui::onEnter()
{
    ccColor4B color ={ 0,0,0,150 };
    CCLayerColor* pLayerColor = CCLayerColor::layerWithColor(color);
    addChild( pLayerColor );
    
	CCXMLLayer::onEnter();

	LoadPlist( "achievement.plist" );

	const char *MenuButton[] =
	{
		"button_back" 
	};

	for( int i = 0; i < 1; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CTaskEventSubGui::menuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
	}

	m_vTaskEvent.clear();

	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		CBaseDataEvent* pBaseDataEvent =  g_pClientLoader->GetEventDataFromId(  g_pPlayerEventCtrl->m_TaskData[i].EventId  );
		if( NULL != pBaseDataEvent )
		{
			if( 0 == g_pPlayerEventCtrl->GetEventFlag( g_pPlayerEventCtrl->m_TaskData[i].EventId ) )
			{
				STaskEvent taskEvent;
				taskEvent.eventId	= g_pPlayerEventCtrl->m_TaskData[i].EventId;
				taskEvent.eventLog	= CEventLog[pBaseDataEvent->m_SubEvent.m_wSubEventLogID][g_iCurLanguageIndex]; 
				taskEvent.eventIcon = pBaseDataEvent->m_SubEvent.eventicon;
				
				CBaseFunRunData *pBaseFunRunData = g_pClientLoader->GetFunRunDataFromId( pBaseDataEvent->m_EventPointInfo.m_iPointRunArray[0] );
				if( NULL != pBaseFunRunData )
				{
					taskEvent.targetNum = pBaseFunRunData->m_arrParam[0];
					taskEvent.RunIcon = pBaseFunRunData->m_strIcon;
				}
                m_vTaskEvent.push_back( taskEvent );
			}
		}

	}

	CTaskMoveLayer *pGroup = new CTaskMoveLayer();
	pGroup->init();
	pGroup->autorelease();
	
	CCNode* pClip = GetXMLNodeFromKey( "t2dSceneObject_achievement_show" );

	pGroup->setContentSize( pClip->getContentSize() );
	pGroup->setIsRelativeAnchorPoint( true );
	pGroup->setPosition( pClip->getPosition() );

	m_pGroupNode = CCNode::node();
	addChild( pGroup,100 );
	pGroup->addChild( m_pGroupNode );

    CCSize size = CCDirector::sharedDirector()->getWinSize();
	m_iEachSizeDis = pClip->getContentSize().height / 4;
    
    if( CCDirector::sharedDirector()->getWinSize().width >= 1024 )
    {
        m_iEachSizeHeight = size.height / 8;
    }
    else
    {
        m_iEachSizeHeight = size.height / 6;
    }
//
	for( int i = 0; i < m_vTaskEvent.size(); i++ )
	{
		CCNode* pNode = GetXMLNodeFromKey( "t2dSceneObject_achievement_icon" );
		CCPoint CConverpos = pGroup->convertToNodeSpace( pNode->getPosition() );

		string path = GetGameImagesPath();
        path = path + m_vTaskEvent[i].eventIcon;
		CCSprite* pSprite = CCSprite::spriteWithFile( path.c_str() );
		pSprite->setPosition( ccp( CConverpos.x, CConverpos.y - i* m_iEachSizeHeight ) );
		m_pGroupNode->addChild( pSprite );

		pNode = GetXMLNodeFromKey( "t2dSceneObject_achievement_bottom" );
		CConverpos = pGroup->convertToNodeSpace( pNode->getPosition() );


		path = GetGameImagesPath();
        path = path + "Achievement_ytiao.png";
		pSprite = CCSprite::spriteWithFile( path.c_str() );
		pSprite->setPosition( ccp( CConverpos.x, CConverpos.y - i* m_iEachSizeHeight ) );
		m_pGroupNode->addChild( pSprite );

		string utf8;
		CChineseCode::GB2312ToUTF_8( utf8, ( char* )m_vTaskEvent[i].eventLog.c_str(), m_vTaskEvent[i].eventLog.length() );
		CCLabelTTF *pLabel  = CCLabelTTF::labelWithString( utf8.c_str(), kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_SMALL].fontSize );
		pLabel->setPosition( ccp( CConverpos.x, CConverpos.y - i* m_iEachSizeHeight ) );
		CCRenderTexture *pstroke = createStroke(pLabel, 1, kFontSystem[FONT_SMALL].strokeColor);
		m_pGroupNode->addChild( pstroke );
		m_pGroupNode->addChild( pLabel );

		pNode = GetXMLNodeFromKey( "t2dSceneObject_achievement_reward" );
		CConverpos = pGroup->convertToNodeSpace( pNode->getPosition() );

		path = GetGameImagesPath();
		path = path + m_vTaskEvent[i].RunIcon;
		if( !m_vTaskEvent[i].RunIcon.empty() )
		{
			pSprite = CCSprite::spriteWithFile( path.c_str() );
			pSprite->setPosition( ccp( CConverpos.x, CConverpos.y - i* m_iEachSizeHeight ) );
			m_pGroupNode->addChild( pSprite );
		}

		char buffer[32];
		sprintf( buffer, "+%d", m_vTaskEvent[i].targetNum );
		pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_SMALL].fontName, kFontSystem[FONT_SMALL].fontSize );
		pLabel->setPosition( ccp( CConverpos.x + pSprite->getContentSize().width / 2, CConverpos.y - i* m_iEachSizeHeight) );
		pLabel->setAnchorPoint( ccp( 0, 0.5 ) );
		pstroke = createStroke(pLabel, 1, kFontSystem[FONT_SMALL].strokeColor);
		m_pGroupNode->addChild( pstroke );
		m_pGroupNode->addChild( pLabel );
		
	}
	
	
	m_pUpScorl	= ( CCSprite* )GetXMLNodeFromKey( "scorlup" );
	m_pDownScorl= ( CCSprite* )GetXMLNodeFromKey( "scorldown" );

	if( m_vTaskEvent.size() <= 4 )
	{
		m_pUpScorl->setIsVisible( false );
		m_pDownScorl->setIsVisible( false );
	}
	else
	{
		m_pDownScorl->setIsVisible( true );
	}

	setIsTouchEnabled( true );
}
//------------------------------------------------------------
//
//
void CTaskEventSubGui::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}
//-------------------------------------------------------------------------
//
//
void CTaskEventSubGui::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if( m_vTaskEvent.size() <= 4 )
	{
		m_pUpScorl->setIsVisible( false );
		m_pDownScorl->setIsVisible( false );
		return;
	}

	CCSize s = CCDirector::sharedDirector()->getWinSize();

    CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->locationInView( touch->view() );	
	CCPoint prevLocation = touch->previousLocationInView( touch->view() );	
	
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	prevLocation = CCDirector::sharedDirector()->convertToGL( prevLocation );
	
	CCPoint diff = ccpSub(touchLocation, prevLocation);
	CCPoint currentPos = m_pGroupNode->getPosition();
	m_pGroupNode->setPosition( ccp( currentPos.x, ccpAdd(currentPos, diff).y ) );


	m_pUpScorl->setIsVisible( true );
	m_pDownScorl->setIsVisible( true );
	if( m_pGroupNode->getPosition().y <= 0 )
	{
		m_pGroupNode->setPosition( ccp( m_pGroupNode->getPosition().x, 0 ) );
		m_pUpScorl->setIsVisible( false );

	}

	if( m_pGroupNode->getPosition().y >= m_iEachSizeDis * ( m_vTaskEvent.size() - 4) )
	{
		m_pGroupNode->setPosition( ccp( m_pGroupNode->getPosition().x, m_iEachSizeDis * ( m_vTaskEvent.size() - 4) ) );
		m_pDownScorl->setIsVisible( false );

	}


}
//-------------------------------------------------------------------------
//
//
void CTaskEventSubGui::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}
//-----------------------------------------------
//
//
void CTaskEventSubGui::menuCallback(CCObject * pSender)
{
	CGameWorldScene* pWorldLayer =  ( CGameWorldScene* )getParent();
    pWorldLayer->EnableAllWorldButton( true );
    getParent()->removeChild(this,true);
}
//-------------------------------------------------------------
//
//
void CTaskEventSubGui::onExit()
{
	CCXMLLayer::onExit();
}
//-------------------------------------------------------------
//
//
CTaskMoveLayer::CTaskMoveLayer()
{

}
//------------------------------------------------------------
//
//
CTaskMoveLayer::~CTaskMoveLayer()
{

}
//---------------------------------------------------------------
//
//
void CTaskMoveLayer::visit() 
{
	this->preVisitWithClippingRect(CCRectMake(-this->getContentSize().width * this->getAnchorPoint().x,
		-this->getContentSize().height* this->getAnchorPoint().y,
		this->getContentSize().width * this->getScaleX(),
		this->getContentSize().height*this->getScaleY()));

	CCLayer::visit();

	this->postVisit();

}
//---------------------------------------------------------------
//
//
void CTaskMoveLayer::preVisitWithClippingRect(CCRect clipRect)
{
    if (!this->getIsVisible())
        return;
	
    glEnable(GL_SCISSOR_TEST);
	
    CCDirector *director = CCDirector::sharedDirector();
    CCSize size =director->getWinSize();
    CCPoint origin = this->convertToWorldSpaceAR(clipRect.origin);
    CCPoint topRight =this->convertToWorldSpaceAR(ccpAdd(clipRect.origin, ccp(clipRect.size.width, clipRect.size.height)));
    CCRect scissorRect = CCRectMake(origin.x, origin.y, topRight.x-origin.x, topRight.y-origin.y);
	
    // transform the clipping rectangle to adjust to the current screen
    // orientation: the rectangle that has to be passed into glScissor is
    // always based on the coordinate system as if the device was held with the
    // home button at the bottom. the transformations account for different
    // device orientations and adjust the clipping rectangle to what the user
    // expects to happen.
    ccDeviceOrientation orientation =director->getDeviceOrientation();
    switch (orientation) {
        case kCCDeviceOrientationPortrait:
            break;
        case kCCDeviceOrientationPortraitUpsideDown:
            scissorRect.origin.x = size.width-scissorRect.size.width-scissorRect.origin.x;
            scissorRect.origin.y = size.height-scissorRect.size.height-scissorRect.origin.y;
            break;
        case kCCDeviceOrientationLandscapeLeft:
        {
            float tmp = scissorRect.origin.x;
            scissorRect.origin.x = scissorRect.origin.y;
            scissorRect.origin.y = size.width-scissorRect.size.width-tmp;
            tmp = scissorRect.size.width;
            scissorRect.size.width = scissorRect.size.height;
            scissorRect.size.height = tmp;
        }
            break;
        case kCCDeviceOrientationLandscapeRight:
        {
            float tmp = scissorRect.origin.y;
            scissorRect.origin.y = scissorRect.origin.x;
            scissorRect.origin.x = size.height-scissorRect.size.height-tmp;
            tmp = scissorRect.size.width;
            scissorRect.size.width = scissorRect.size.height;
            scissorRect.size.height = tmp;
        }
            break;
    }
	
    // Handle Retina
    scissorRect = CC_RECT_POINTS_TO_PIXELS(scissorRect);
	
    glScissor((GLint) scissorRect.origin.x, (GLint) scissorRect.origin.y,
              (GLint) scissorRect.size.width, (GLint) scissorRect.size.height);
}
//---------------------------------------------------------------
//
//
void CTaskMoveLayer::postVisit()
{
    glDisable(GL_SCISSOR_TEST);
}