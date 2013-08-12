#include "CGameLetter.h"
#include "../GameScene/CGameWorldScene.h"
#include "../Sprite/CPet.h"
#include "../Platform/CCNd91Mng.h"
#include "../CGameText.h"
#include "../Pushui/CMessageBoxYes.h"
#include "../Pushui/CMessageBoxYesNo.h"
#include "../Fighting/CSelectEnemyScene.h"
#include "../Fighting/CFightScene.h"
#include "../loader/CClientLoader.h"
//---------------------------------------------------------------------------------
//
//
CGameLetter::CGameLetter()
{

}
//---------------------------------------------------------------------------------
//
//
CGameLetter::~CGameLetter()
{

}
//---------------------------------------------------------------------------------
//
//
void CGameLetter::onEnter()
{
	CCXMLLayer::onEnter();
	LoadPlist("message.plist");
	m_vMessages.clear();

    ccColor4B color ={ 0,0,0,150 };
    CCLayerColor* pLayerColor = CCLayerColor::layerWithColor(color);
    addChild( pLayerColor );
    
    const char *MenuButton[] =
	{
        "button_back"
	};

	for( int i = 0; i < 1; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CGameLetter::menuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
        RemoveNode(pSprite);
	}

	//AddMessage( "03/09 11;30" ,"xxxxxxgetmeeeesdgfasdgasdg");
	//AddMessage( "03/09 11;30" ,"xxxxxxgetmeeeesdgfasdgasdg");
	//AddMessage( "03/09 11;30" ,"xxxxxxgetmeeeesdgfasdgasdg");
	//AddMessage( "03/09 11;30" ,"xxxxxxgetmeeeesdgfasdgasdg");
	//AddMessage( "03/09 11;30" ,"xxxxxxgetmeeeesdgfasdgasdg");
	//AddMessage( "03/09 11;30" ,"xxxxxxgetmeeeesdgfasdgasdg");
	//AddMessage( "03/09 11;30" ,"xxxxxxgetmeeeesdgfasdgasdg");

	for( int i = MAX_PET_DATA_SAVE_MESSAGE - 1; i >= 0; i-- )
	{
		if( g_pPetDataBlock->recordsave.saveMessage[i].isHas )
		{
			AddMessage( g_pPetDataBlock->recordsave.saveMessage[i].gameDay, g_pPetDataBlock->recordsave.saveMessage[i].strMessage, g_pPetDataBlock->recordsave.saveMessage[i].type, g_pPetDataBlock->recordsave.saveMessage[i].name  );
		}
	}

	
	CLetterMoveLayer *pMoveLayer = new CLetterMoveLayer();
	pMoveLayer->init();
	pMoveLayer->autorelease();
	pMoveLayer->setIsRelativeAnchorPoint( true );

	CCNode *pNode= (CCNode*)GetXMLNodeFromKey( "t2dSceneObject_gamemessageclip" );
	pMoveLayer->setContentSize( pNode->getContentSize() );
	pMoveLayer->setPosition( pNode->getPosition() );
	addChild( pMoveLayer,100 );

	m_pGroupNode = CCNode::node();
	
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	for( int i = 0; i < m_vMessages.size(); i++ )
	{
		CCNode *pNode= (CCNode*)GetXMLNodeFromKey( "t2dSceneObject_messagebgpos" );
		string path = GetGameImagesPath();
		path = path + "message_bar.png";
		CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
		CCPoint pos = pMoveLayer->convertToNodeSpace( pNode->getPosition() );
		pSprite->setPosition( ccp( pos.x, pos.y - s.height / 9 * i ) );
		m_pGroupNode->addChild( pSprite );

		pNode= (CCNode*)GetXMLNodeFromKey( "t2dSceneObject_messagetime" );

		char buffer[512];
		sprintf( buffer, "%s", m_vMessages[i].strTime.c_str() );
		CCLabelTTF *pLabel = CCLabelTTF::labelWithString( buffer,kFontSystem[FONT_TINY].fontName,kFontSystem[FONT_TINY].fontSize);
		pos = pMoveLayer->convertToNodeSpace( pNode->getPosition() );
		pLabel->setPosition( ccp( pos.x, pos.y - s.height / 9 * i ) );
		CCRenderTexture *pstroke = createStroke(pLabel, 1, kFontSystem[FONT_TINY].strokeColor);
		m_pGroupNode->addChild( pstroke );
		m_pGroupNode->addChild( pLabel, 1);


		pNode= (CCNode*)GetXMLNodeFromKey( "t2dSceneObject_gamemessage" );

		sprintf( buffer, "%s", m_vMessages[i].strMessage.c_str() );
		pLabel = CCLabelTTF::labelWithString( buffer,kFontSystem[FONT_SMALL].fontName,kFontSystem[FONT_SMALL].fontSize);
		pos = pMoveLayer->convertToNodeSpace( pNode->getPosition() );
		pLabel->setPosition( ccp( pos.x, pos.y - s.height / 9 * i ) );
		pstroke = createStroke(pLabel, 1, kFontSystem[FONT_SMALL].strokeColor);
		m_pGroupNode->addChild( pstroke );
		m_pGroupNode->addChild( pLabel, 1);
        
        pNode= (CCNode*)GetXMLNodeFromKey( "t2dSceneObject_releasemessage" );
        

        pos = pMoveLayer->convertToNodeSpace( pNode->getPosition() );
        if( m_vMessages[i].messageType == 0 || g_iGameMode == 0 )
        {
//            string pathTemp = GetGameImagesPath();
//            pathTemp = pathTemp + "message_fabu.png";
//            CCMenu *pMenu = CreatGameUIButton( pathTemp.c_str(), menu_selector( CGameLetter::weiboCallback), i, this,  ccp( pos.x, pos.y - s.height / 9 * i ) );
//            m_pGroupNode->addChild( pMenu,100 );
        }
        else
        {
            string pathTemp = GetGameImagesPath();
            pathTemp = pathTemp + "message_fanji.png";
            CCMenu *pMenu = CreatGameUIButton( pathTemp.c_str(), menu_selector( CGameLetter::pushBack), i, this,  ccp( pos.x, pos.y - s.height / 9 * i ) );
            m_pGroupNode->addChild( pMenu,100 );
        }
        
	}

	m_iEachSizeDis = s.height / 9;

	pMoveLayer->addChild( m_pGroupNode );

	setIsTouchEnabled( true );
    
    m_pTarget = CCRenderTexture::renderTextureWithWidthAndHeight(s.width, s.height);
	m_pTarget->setPosition(ccp(s.width / 2, s.height / 2));
    this->addChild(m_pTarget, -1);
    
    pNode= (CCNode*)GetXMLNodeFromKey( "money_100" );
   if(g_pPetDataBlock->recordsave.weiboGetMoneyCount > 0)
   {
       pNode->setIsVisible(true);
   }
   else
   {
       pNode->setIsVisible(false);
   }  
}
//--------------------------------------------------------------------------------
//
//
void CGameLetter::pushBack( CCObject * pSender )
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( 0 == CDevice::CheckNet() )
    {
        CCMessageBox("网络貌似有问题！", "提示");
        return;
    }
#endif
    
    int tag = ((CCNode*)pSender)->getTag();
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
    
    g_pPetDataBlock->submaxstaminapoint( FIGHT_SUB_STAMINAPOINT );
    g_pPetDataBlock->subPetMoney( FIGHT_SUB_MONEY );
    
    m_iPushBackPlayerId = m_vMessages[tag].messageType;
    
    CSelectEnemyScene *pSelectEnemyScene = new CSelectEnemyScene();
    
    char uinbuffer[32];
    sprintf( uinbuffer, "%d", m_vMessages[tag].messageType );
    PetDatablock * pDataBlock = pSelectEnemyScene->GetOthers(uinbuffer, m_vMessages[tag].name);
    
    CBaseDataPet* pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( pDataBlock->petid );
    
    pDataBlock->ResId    = pPetBaseData->animationid;
    pDataBlock->rbg[0]   = pPetBaseData->rgb[0];
    pDataBlock->rbg[1]   = pPetBaseData->rgb[1];
    pDataBlock->rbg[2]   = pPetBaseData->rgb[2];
    
    CSelectEnemyScene::s_pPetDataBlock = pDataBlock;
    
    CCDirector::sharedDirector()->purgeCachedData();
    CFightScene::Show();
    
    CC_SAFE_DELETE( pSelectEnemyScene );
}
//----------------------------------------------------------------------------------
//
//
void CGameLetter::weiboCallback( CCObject * pSender )
{
    //m_pTarget->begin();
    //getParent()->visit();
    //m_pTarget->end();
    
    //char str[32];
	//sprintf(str, "image1.png");
	//m_pTarget->saveBuffer(kCCImageFormatPNG, str);
	//CCLOG("Image saved %s", str);
    
    CCNode *pNode= (CCNode*)GetXMLNodeFromKey( "money_100" );
    if(g_pPetDataBlock->recordsave.weiboGetMoneyCount > 0)
    {
        pNode->setIsVisible(true);
        g_pPetDataBlock->recordsave.weiboGetMoneyCount --;
        g_pPetDataBlock->petmoney += 100;
        
    }
    if( g_pPetDataBlock->recordsave.weiboGetMoneyCount <= 0 )
    {
        pNode->setIsVisible(false);
    }  
    
    string path = CCFileUtils::getWriteablePath();
    path = "Icon@2x.png";
    int tag = ((CCNode*)pSender)->getTag();
    string message = m_vMessages[tag].strTime + m_vMessages[tag].strMessage;
    
    string utf81;
	CChineseCode::GB2312ToUTF_8( utf81, (char*)message_gameshow[g_iCurLanguageIndex], 256 );
    message = message + utf81; 
    CNd91Mng::NdShareToThirdPlatform( message, path );
    
    
    
}

//----------------------------------------------------------------------------------
//
//
void CGameLetter::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}
//-----------------------------------------------------------------------
//
//
void CGameLetter::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if( m_vMessages.size() <= 5 )
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
	CCPoint currentPos = m_pGroupNode->getPosition();
	m_pGroupNode->setPosition( ccp( currentPos.x, ccpAdd(currentPos, diff).y ) );

	if( m_pGroupNode->getPosition().y <= 0 )
	{
		m_pGroupNode->setPosition( ccp( m_pGroupNode->getPosition().x, 0 ) );
		//m_pUpScorl->setIsVisible( false );

	}

	if( m_pGroupNode->getPosition().y >= m_iEachSizeDis * ( m_vMessages.size() - 5) )
	{
		m_pGroupNode->setPosition( ccp( m_pGroupNode->getPosition().x, m_iEachSizeDis * ( m_vMessages.size() - 5) ) );
		//m_pDownScorl->setIsVisible( false );

	}
}
//-----------------------------------------------------------------------------------
//
//
void CGameLetter::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}
//-----------------------------------------------------------------------------------
//
//
void CGameLetter::AddMessage( string messgeTime, string message, int MessageType, string name )
{
	SGameMessage MessageData;
	MessageData.strMessage = message;
	MessageData.strTime = messgeTime;
	MessageData.messageType = MessageType;
    MessageData.name = name;
    
	m_vMessages.push_back( MessageData );
}
//----------------------------------------------------------------------------------
//
//
void CGameLetter::onExit()
{
	CCXMLLayer::onExit();	
}
//-----------------------------------------------------------------------------------
//
//
void CGameLetter::menuCallback(CCObject * pSender)
{
//int tag = (( CCNode*)pSender)->getTag();
	( (CGameWorldScene*)getParent() )->EnableAllWorldButton( true );
	  removeFromParentAndCleanup(true);
}
//--------------------------------------------------------------------------------
//
//
void CGameLetter::UpData( ccTime tt  )
{

}
//-------------------------------------------------------------
//
//
CLetterMoveLayer::CLetterMoveLayer()
{
    
}
//------------------------------------------------------------
//
//
CLetterMoveLayer::~CLetterMoveLayer()
{
    
}
//---------------------------------------------------------------
//
//
void CLetterMoveLayer::visit() 
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
void CLetterMoveLayer::preVisitWithClippingRect(CCRect clipRect)
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
void CLetterMoveLayer::postVisit()
{
    glDisable(GL_SCISSOR_TEST);
}