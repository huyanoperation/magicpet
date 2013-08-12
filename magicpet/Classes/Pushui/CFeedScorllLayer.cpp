#include "CFeedScorllLayer.h"
#include "CCFeedLayer.h"
#include "../Sprite/CPet.h"
#include "../loader/CClientLoader.h"
#include "CCFeedLayer.h"
#include "CMessageBoxYesNo.h"
//#include "../CCScrollLayerButton.h"
#define MAX_ITEM_SIZE 2
//---------------------------------------------------------------------------------------
//
//
CFeedScorllLayer::CFeedScorllLayer()
{
	m_vScorllFeedItem.clear();
}
//---------------------------------------------------------------------------------------
//
//
CFeedScorllLayer::~CFeedScorllLayer()
{

}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::onEnter()
{
	CCLayer::onEnter();

	setIsTouchEnabled( true );
	
	m_pGroupNode = CCNode::node();
	addChild( m_pGroupNode, 0, CFEEDSCORLLLAYER_NODE_TAG );

    schedule( schedule_selector( CFeedScorllLayer::UpData ) );
    
	 CFEEDSCORLLLAYER_ITEM_EACH_DIS = 150;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if( CCDirector::sharedDirector()->getWinSize().width >= 1024 )
    {
       CFEEDSCORLLLAYER_ITEM_EACH_DIS = 300;
    }
#endif
    
    CollectItem();
    
	AddItemToShop();
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::AddItemToShop()
{
	CCNode *pNode2 = ((CCFeedLayer*)getParent())->GetXMLNodeFromKey( "t2dSceneObject_peteatslideframe" );

	for( int i = 0; i < m_vScorllFeedItem.size(); i ++ )
	{
		CCNode *pNode = ((CCFeedLayer*)getParent())->GetXMLNodeFromKey( "t2dSceneObject_peteat_foodbg" );
		CCPoint Pos( pNode->getPosition().x - pNode2->getPosition().x + pNode2->getContentSize().width / 2, pNode->getPosition().y - pNode2->getPosition().y + pNode2->getContentSize().height / 2 );

		string path = GetGameImagesPath();
		path = path + "peteat_feedbg.png";
		CCSprite *pSprite = NULL;

		CCMenu* pMenu = CreatGameUIButton( path, menu_selector( CFeedScorllLayer::ItemMenuCallBack ), m_vScorllFeedItem[i].ItemId, this, ccp( Pos.x + CFEEDSCORLLLAYER_ITEM_EACH_DIS * i, Pos.y) );
		m_pGroupNode->addChild( pMenu, 1);
		pMenu->setIsTouchEnabled( false );
		pMenu->setContentSize( pNode->getContentSize() );
		m_vScorllFeedItem[i].pMenu = pMenu;
        

		CBaseDataItem *pBaseDataItem = g_pClientLoader->GetBaseDataItemFromID( m_vScorllFeedItem[i].ItemId );
		path = GetGameImagesPath();
		path = path + pBaseDataItem->itemicon.c_str();
		pSprite = CCSprite::spriteWithFile( path.c_str() );
		pSprite->setPosition( pMenu->getPosition() );
		m_pGroupNode->addChild( pSprite, 1);
        
		char buffer[32];
		sprintf( buffer, "%d", m_vScorllFeedItem[i].ItemCount );
		CCLabelTTF *pLabel = CCLabelTTF::labelWithString( buffer,kFontSystem[FONT_TALL].fontName,kFontSystem[FONT_TALL].fontSize);
		pLabel->setPosition( ccp( pSprite->getPosition().x + pSprite->getTextureRect().size.width / 2, pSprite->getPosition().y - pSprite->getTextureRect().size.height / 2 ) );
		m_pGroupNode->addChild( pLabel, 1);
        
        m_vScorllFeedItem[i].pLabelItemCount = pLabel;

		pNode = ((CCFeedLayer*)getParent())->GetXMLNodeFromKey( "t2dSceneObject_peteat_completionfeed" );
		CCPoint Pos2( pNode->getPosition().x - pNode2->getPosition().x + pNode2->getContentSize().width / 2, pNode->getPosition().y - pNode2->getPosition().y + pNode2->getContentSize().height / 2 );
		path = GetGameImagesPath();
		path = path + "peteat_completionfeed.png";

		pMenu = CreatGameUIButton( path, menu_selector( CFeedScorllLayer::ItemCompleteCallBack ), m_vScorllFeedItem[i].ItemId, this, ccp( Pos2.x + CFEEDSCORLLLAYER_ITEM_EACH_DIS * i, Pos2.y ) );
		m_pGroupNode->addChild( pMenu,1,CFEEDSCORLLLAYER_ITEM_BUTTON_TAG_BEGIN + m_vScorllFeedItem[i].ItemId );
        if( !m_vScorllFeedItem[i].isActive )
        {
            pMenu->setIsVisible(false);
        }
		pNode = ((CCFeedLayer*)getParent())->GetXMLNodeFromKey( "t2dSceneObject_peteat_stopfeed" );
		CCPoint Pos3( pNode->getPosition().x - pNode2->getPosition().x + pNode2->getContentSize().width / 2, pNode->getPosition().y - pNode2->getPosition().y + pNode2->getContentSize().height / 2 );
		path = GetGameImagesPath();
		path = path + "peteat_stopfeed.png";

		pMenu = CreatGameUIButton( path, menu_selector( CFeedScorllLayer::ItemCancelCallBack ), m_vScorllFeedItem[i].ItemId, this, ccp( Pos3.x + CFEEDSCORLLLAYER_ITEM_EACH_DIS * i, Pos3.y) );
		m_pGroupNode->addChild( pMenu,1,CFEEDSCORLLLAYER_ITEM_BUTTON_TAG_BEGIN + m_vScorllFeedItem[i].ItemId );
		if( !m_vScorllFeedItem[i].isActive )
        {
            pMenu->setIsVisible(false);
        }
		pNode = ((CCFeedLayer*)getParent())->GetXMLNodeFromKey( "t2dSceneObject_peteat_cdtime_1" );
		CCPoint Pos4( pNode->getPosition().x - pNode2->getPosition().x + pNode2->getContentSize().width / 2, pNode->getPosition().y - pNode2->getPosition().y + pNode2->getContentSize().height / 2 );
		path = GetGameImagesPath();
		path = path + "peteat_completiontime.png";
		pSprite = CCSprite::spriteWithFile( path.c_str() );
		pSprite->setPosition( ccp( Pos4.x + CFEEDSCORLLLAYER_ITEM_EACH_DIS * i, Pos4.y) );
		if( !m_vScorllFeedItem[i].isActive )
        {
            pSprite->setIsVisible( false );
        }
		m_pGroupNode->addChild( pSprite,0,CFEEDSCORLLLAYER_ITEM_BUTTON_TAG_BEGIN + m_vScorllFeedItem[i].ItemId );
        
        pLabel  = CCLabelTTF::labelWithString( "    ", kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
        pLabel->setPosition( pSprite->getPosition() );
        m_pGroupNode->addChild( pLabel,1,CFEEDSCORLLLAYER_ITEM_BUTTON_TAG_BEGIN + m_vScorllFeedItem[i].ItemId );
        m_vScorllFeedItem[i].pLabel = pLabel;

	}

	m_iEachItemSize = getContentSize().width / 2;
	m_iInitPos = m_pGroupNode->getPosition().x;
}
//----------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::ItemMenuCallBack( CCObject * pSender )
{
	int tag = ( (CCNode*)pSender )->getTag();
	((CCFeedLayer*)getParent())->OnItemSelect( tag );
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::OnActiveItem( int ItemId )
{
    for( int i = 0; i < m_pGroupNode->getChildren()->count(); i++ )
    {
        if( ((CCNode*)m_pGroupNode->getChildren()->objectAtIndex(i))->getTag() == ItemId + CFEEDSCORLLLAYER_ITEM_BUTTON_TAG_BEGIN )
        {
            ((CCNode*)m_pGroupNode->getChildren()->objectAtIndex(i))->setIsVisible(true);
        }
    }
}
//--------------------------------------------------------------
//
//
void CFeedScorllLayer::UpData( ccTime tt )
{
    for( int i = 0; i < m_vScorllFeedItem.size(); i++ )
    {
        if( m_vScorllFeedItem[i].isActive)
        {
            CBaseDataItem *pDataItem = g_pClientLoader->GetBaseDataItemFromID( m_vScorllFeedItem[i].ItemId );
            int time = pDataItem->itemcd * m_vScorllFeedItem[i].activeCount - ( GetCurrentSeconds() - m_vScorllFeedItem[i].leaveTime );
            if( time < 0 )
            {
                m_vScorllFeedItem[i].isActive = false;
                ( ( CCFeedLayer* )getParent() )->close();
            }
                
            int h,m,s;
			GetHourMiniSecond( time, h, m, s );
			char buffer[64];
			sprintf( buffer, " %dh %dm %ds", h, m, s );
            
            m_vScorllFeedItem[i].pLabel->setString(buffer);
        }
    }
}
//--------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::ItemCompleteCallBack( CCObject * pSender )
{
    int itemId = ( (CCNode*)pSender )->getTag();
    
    string utf81;
    char buffer[256];
    sprintf( buffer, message_moneypoint_1[g_iCurLanguageIndex], 1 );
    CChineseCode::GB2312ToUTF_8( utf81, buffer, 256 );
    CMessageBoxYesNo *pMessage= new CMessageBoxYesNo( (char*)utf81.c_str(), callfuncN_selector( CCFeedLayer::ItemCompleteYes ), callfuncN_selector( CCFeedLayer::ItemCompleteNo ), getParent(), itemId );
    pMessage->autorelease();
    getParent()->addChild( pMessage, 100 );
    
}
//--------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::ItemCancelCallBack( CCObject * pSender )
{
    int ItemId = ( (CCNode*)pSender )->getTag();
    SFeedDataItem* pSaveData = g_pPetDataBlock->GetSaveItemFromId( ItemId );
    
    pSaveData->isActive = false;
    
    for( int i = 0; i < m_pGroupNode->getChildren()->count(); i++ )
    {
        if( ((CCNode*)m_pGroupNode->getChildren()->objectAtIndex(i))->getTag() == ItemId + CFEEDSCORLLLAYER_ITEM_BUTTON_TAG_BEGIN )
        {
            ((CCNode*)m_pGroupNode->getChildren()->objectAtIndex(i))->setIsVisible(false);
        }
    }
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::CollectItem()
{
	for( int i = 0; i < g_pPetDataBlock->itemnum; i++ )
	{
		SScorllFeedItem FeedItem;
		FeedItem.isActive	= g_pPetDataBlock->pFeedPetData->feedItem[i].isActive;
		FeedItem.ItemCount	= g_pPetDataBlock->pFeedPetData->feedItem[i].itemCount;
		FeedItem.ItemId		= g_pPetDataBlock->pFeedPetData->feedItem[i].itemId;
		FeedItem.leaveTime	= g_pPetDataBlock->pFeedPetData->feedItem[i].leaveTime;
        FeedItem.activeCount= g_pPetDataBlock->pFeedPetData->feedItem[i].activeCount;
		m_vScorllFeedItem.push_back( FeedItem );
	}

	if( m_vScorllFeedItem.size() > MAX_ITEM_SIZE )
	{
		m_pRightPoint->setIsVisible( true );
	}
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::onExit()
{
	CCLayer::onExit();
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}
//--------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::onButtonMove( int act )
{
	m_pRightPoint->setIsVisible( false );
	m_pLeftPoint->setIsVisible( false );

	CCNode *node = getChildByTag(CFEEDSCORLLLAYER_NODE_TAG);
	CCPoint currentPos = node->getPosition();

	if( 0 == act )
	{
		node->setPosition( ccp( currentPos.x + m_iEachItemSize, currentPos.y ) );
	}
	else
	{
		node->setPosition( ccp( currentPos.x - m_iEachItemSize, currentPos.y ) );
	}

	int temp = -m_iEachItemSize * ( m_vScorllFeedItem.size() - MAX_ITEM_SIZE );

	if( m_pGroupNode->getPosition().x >= 0 )
	{
		m_pGroupNode->setPosition( ccp( 0, m_pGroupNode->getPosition().y ) );
		m_pRightPoint->setIsVisible( true );
	}
	else if( m_pGroupNode->getPosition().x <= temp )
	{
		m_pGroupNode->setPosition( ccp( temp, m_pGroupNode->getPosition().y ) );
		m_pLeftPoint->setIsVisible( true );
	}
	else
	{
		m_pRightPoint->setIsVisible( true );
		m_pLeftPoint->setIsVisible( true );
	}
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	m_pRightPoint->setIsVisible( false );
	m_pLeftPoint->setIsVisible( false );

	if( m_vScorllFeedItem.size() <= MAX_ITEM_SIZE )
	{
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
	
	CCNode *node = getChildByTag(CFEEDSCORLLLAYER_NODE_TAG);
	CCPoint currentPos = node->getPosition();
	node->setPosition( ccp( ccpAdd(currentPos, diff).x, currentPos.y ) );

	int temp = -m_iEachItemSize * ( m_vScorllFeedItem.size() - MAX_ITEM_SIZE );

	if( m_pGroupNode->getPosition().x >= 0 )
	{
		m_pGroupNode->setPosition( ccp( 0, m_pGroupNode->getPosition().y ) );
		m_pRightPoint->setIsVisible( true );
	}
	else if( m_pGroupNode->getPosition().x <= temp )
	{
		m_pGroupNode->setPosition( ccp( temp, m_pGroupNode->getPosition().y ) );
		m_pLeftPoint->setIsVisible( true );
	}
	else
	{
		m_pRightPoint->setIsVisible( true );
		m_pLeftPoint->setIsVisible( true );
	}
	
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint touchLocation = touch->locationInView( touch->view() );	
	CCPoint prevLocation = touch->previousLocationInView( touch->view() );	
	
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	prevLocation = CCDirector::sharedDirector()->convertToGL( prevLocation );

	CCPoint pos = convertTouchToNodeSpace( touch );

    for( int i = 0; i < m_vScorllFeedItem.size(); i++ )
    {
         m_vScorllFeedItem[i].pMenu->setColor(ccc3( 255,255,255 ) );
    }
    
	for( int i = 0; i < m_vScorllFeedItem.size(); i++ )
	{
		CCPoint menuPos = m_vScorllFeedItem[i].pMenu->getPosition();

		menuPos = ccp( m_pGroupNode->getPosition().x + menuPos.x, m_pGroupNode->getPosition().y + menuPos.y );

		CCSize size = m_vScorllFeedItem[i].pMenu->getContentSize();

		if( CCRect::CCRectContainsPoint( CCRect( menuPos.x - size.width / 2, menuPos.y - size.height / 2, size.width, size.height ),  pos  ) )
		{
			((CCFeedLayer*)getParent())->OnItemSelect( m_vScorllFeedItem[i].ItemId );
            m_vScorllFeedItem[i].pMenu->setColor(ccc3( 150,150,150 ) );
			break;
		}
	}

  //m_vScorllFeedItem
}
//---------------------------------------------------------------------------------------
//
//
void CFeedScorllLayer::menuCallback(CCObject * pSender)
{

}
//---------------------------------------------------------------
//
//
void CFeedScorllLayer::visit() 
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
void CFeedScorllLayer::preVisitWithClippingRect(CCRect clipRect)
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
void CFeedScorllLayer::postVisit()
{
    glDisable(GL_SCISSOR_TEST);
}