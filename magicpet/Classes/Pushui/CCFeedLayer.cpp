#include "CCFeedLayer.h"
#include "../GameScene/CGameWorldScene.h"
#include "../Sprite/CPet.h"
#include "../Function/CExpressions.h"
#include "../loader/CClientLoader.h"
#include "CFeedScorllLayer.h"
#include "../loader/CClientLoader.h"

#include "../CGameMessage.h"
#include "CMessageBoxYes.h"
#include "CMessageBoxYesNo.h"

const static char* levelFileFeedPlist = "peteat.plist"; 

#define MAX_ITEMGROUP1 10
#define MAX_ITEMGROUP2 30
#define MAX_ITEMGROUP3 50
#define MAX_ITEMGROUP4 100

CCFeedLayer::CCFeedLayer()
{
	m_iCurItemId = 0;
}
//-------------------------------------------------
//
//
CCFeedLayer::~CCFeedLayer()
{

}
//-------------------------------------------------
//
//
void CCFeedLayer::onEnter()
{
	CCXMLLayer::onEnter();

	ccColor4B color ={ 0,0,0,150 };
	CCLayerColor* pLayerColor = CCLayerColor::layerWithColor(color);
	addChild( pLayerColor );

	LoadPlist( levelFileFeedPlist );

	CCNode* pNode = GetXMLNodeFromKey( "t2dSceneObject_peteatslideframe" );

	CFeedScorllLayer *pScrollLayer = new CFeedScorllLayer();
	pScrollLayer->init(); 
	pScrollLayer->autorelease();
	pScrollLayer->setContentSize( pNode->getContentSize() );
	pScrollLayer->setPosition( pNode->getPosition() );
	pScrollLayer->setIsRelativeAnchorPoint( true );

	m_pScrollLayer = pScrollLayer;

	m_pItemLogGroup = CCNode::node();
	addChild( m_pItemLogGroup, 100 );

	const char *MenuButton[] =
	{
		"button_left",
		"button_right",
		"button_back",
        "button_addfeed"
	};

	for( int i = 0; i < 4; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CCFeedLayer::menuCallback), i, this, pSprite->getPosition() );
        addChild( pMenu,pSprite->getZOrder());
		RemoveNode( pSprite );	
		m_pFeedControlPoint[i] = pMenu;
	}
	pScrollLayer->m_pRightPoint = m_pFeedControlPoint[1];
	pScrollLayer->m_pLeftPoint = m_pFeedControlPoint[0];

	pScrollLayer->m_pRightPoint->setIsVisible( false );
	pScrollLayer->m_pLeftPoint->setIsVisible( false );

	addChild( pScrollLayer, 99 );

	const char* FeedButton[] = { "button_surefeed_1","button_surefeed_2","button_surefeed_3","button_surefeed_4" };

	for( int i = 0; i < 4; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( FeedButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CCFeedLayer::feedMenuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu,100 );
		RemoveNode( pSprite );	

		m_pFeedMenu[i] = pMenu;
		m_pFeedMenu[i]->setIsVisible( false );
	}

    pNode = GetXMLNodeFromKey( "t2dSceneObject_feed_position" );
    
    int count = 0;
    for( int i = 0; i < m_pScrollLayer->m_vScorllFeedItem.size(); i++)
    {
        if(m_pScrollLayer->m_vScorllFeedItem[i].isActive)
        {
            count++;
        }
    }
    char buffer[32];
	sprintf( buffer, "%d/%d", count , g_pPetDataBlock->pFeedPetData->MaxFeedCount );
	CCLabelTTF *pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_MED].fontName, kFontSystem[FONT_MED].fontSize );
	pLabel->setPosition( pNode->getPosition() );
	addChild( pLabel,100 );
    
    m_pLabelFeedCount = pLabel;
    schedule( schedule_selector( CCFeedLayer::UpData ), 0.1f );
    
    CCSprite *pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4_1");
    pSprite->setIsVisible(false);
    pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4_2");
    pSprite->setIsVisible(false);
    if( !g_pPetDataBlock->recordsave.isNotBeginner[3] && g_pPetDataBlock->recordsave.isNotBeginner[2] )
    {
        CCSprite *pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4_1");
        pSprite->setIsVisible(true);
        reorderChild(pSprite, 1000);
    }
}
//------------------------------------------------------
//
//
void CCFeedLayer::UpData(ccTime tt)
{
    int count = 0;
    for( int i = 0; i < m_pScrollLayer->m_vScorllFeedItem.size(); i++)
    {
        if(m_pScrollLayer->m_vScorllFeedItem[i].isActive)
        {
            count++;
        }
    }
    char buffer[32];
	sprintf( buffer, "%d/%d", count , g_pPetDataBlock->pFeedPetData->MaxFeedCount ); 
    m_pLabelFeedCount->setString(buffer);
}
//----------------------------------------------------------------------
//
//
void CCFeedLayer::feedMenuCallback(CCObject * pSender)
{
    int count = 0;
    for( int i = 0; i < m_pScrollLayer->m_vScorllFeedItem.size(); i++)
    {
        if(m_pScrollLayer->m_vScorllFeedItem[i].isActive)
        {
            count++;
        }
    }
    if( count >= g_pPetDataBlock->pFeedPetData->MaxFeedCount  )
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#else
        CCMessageBox("您超过了最大喂养数！", "提示");
#endif
        return;
    }
    
    
	int tag = (( CCNode*)pSender)->getTag();
	
	SScorllFeedItem *pScorllFeedItem = NULL;

	for( int i = 0; i < m_pScrollLayer->m_vScorllFeedItem.size(); i++ )
	{
		if( m_pScrollLayer->m_vScorllFeedItem[i].ItemId == m_iCurItemId )
		{
			pScorllFeedItem = &m_pScrollLayer->m_vScorllFeedItem[i];
			break;
		}
	}
	if( pScorllFeedItem == NULL )
	{
		return;
	}

	pScorllFeedItem->isActive = true;
	pScorllFeedItem->leaveTime = GetCurrentSeconds();

    for( int i = 0; i < 4; i++ )
    {
        m_pFeedMenu[i]->setIsVisible( false );
    }
    SFeedDataItem* pSaveData = g_pPetDataBlock->GetSaveItemFromId( m_iCurItemId );

	switch( tag )
	{
	case 0:
		{
			pScorllFeedItem->ItemCount -= MAX_ITEMGROUP1;
            pScorllFeedItem->activeCount = MAX_ITEMGROUP1;

		}
		break;
	case 1:
		{
			pScorllFeedItem->ItemCount -= MAX_ITEMGROUP2;
            pScorllFeedItem->activeCount = MAX_ITEMGROUP2;
            
		}
		break;
	case 2:
		{
			pScorllFeedItem->ItemCount -= MAX_ITEMGROUP3;
            pScorllFeedItem->activeCount = MAX_ITEMGROUP3;
            
		}
		break;
	case 3:
		{
			pScorllFeedItem->ItemCount -= MAX_ITEMGROUP4;
            pScorllFeedItem->activeCount = MAX_ITEMGROUP4;
            
		}
		break;
	}
    char buffer[32];
    sprintf( buffer, "%d", pScorllFeedItem->ItemCount );
    pScorllFeedItem->pLabelItemCount->setString( buffer );
	pSaveData->isActive = pScorllFeedItem->isActive;
	pSaveData->itemCount= pScorllFeedItem->ItemCount;
	pSaveData->leaveTime= pScorllFeedItem->leaveTime;
    pSaveData->activeCount = pScorllFeedItem->activeCount;
    
    m_pScrollLayer->OnActiveItem( m_iCurItemId );
    
    if( !g_pPetDataBlock->recordsave.isNotBeginner[3] )
    {
        g_pPetDataBlock->recordsave.isNotBeginner[3] = true;
        CCSprite *pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4_2");
        pSprite->setIsVisible(false);
    }
}
//----------------------------------------------------------------------
//
//
void CCFeedLayer::menuCallback(CCObject * pSender)
{
	int tag = (( CCNode*)pSender)->getTag();
	switch( tag )
	{
	case CCFEEDLAYER_BUTTON_LEFT:
		{
			m_pScrollLayer->onButtonMove(0);
		}
		break;
	case CCFEEDLAYER_BUTTON_RIGHT:
		{
			m_pScrollLayer->onButtonMove(1);
		}
		break;
	case CCFEEDLAYER_BUTTON_BACK:
		{
            close();
		}
		break;
    case CCFEEDLAYER_BUTTON_ADDFEED:
        {
            string utf81;
            char buffer[256];
            sprintf( buffer, message_moneypoint_3[g_iCurLanguageIndex], 10 );
            CChineseCode::GB2312ToUTF_8( utf81, buffer, 256 );
            CMessageBoxYesNo *pMessage= new CMessageBoxYesNo( (char*)utf81.c_str(), callfuncN_selector( CCFeedLayer::AddFeedYes ), NULL, getParent(), 0, true );
            pMessage->autorelease();
            getParent()->addChild( pMessage, 100 );
            
        }
        break;
	}
}
//-----------------------------------------------------------
void CCFeedLayer::ItemCompleteYes( CCNode* pNode )
{
    if( !g_pPetDataBlock->subPlayershoppingmallmoney( 1 ) )
    {
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoneypoint[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
        close();
        return;
    }
    
    int itemId = pNode->getTag();
    
    SFeedDataItem* pSaveData = g_pPetDataBlock->GetSaveItemFromId( itemId );
    CBaseDataItem *pDataItem = g_pClientLoader->GetBaseDataItemFromID( itemId );
    pSaveData->leaveTime -= pDataItem->itemcd * ( pSaveData->activeCount * 2) ;
    close();
}
//------------------------------------------------------------------------------
//
//
void CCFeedLayer::AddFeedYes( CCNode* pNode )
{
    if( !g_pPetDataBlock->subPlayershoppingmallmoney( 10 ) )
    {
        string utf81,utf82;
        CChineseCode::GB2312ToUTF_8( utf81, (char*)message_nomoneypoint[g_iCurLanguageIndex], 256 );
        CChineseCode::GB2312ToUTF_8( utf82, (char*)message_tip[g_iCurLanguageIndex], 256 );
        CMessageBoxYes *pMessage= new CMessageBoxYes( (char*)utf81.c_str(), (char*)utf82.c_str(), NULL, NULL );
        pMessage->autorelease();
        CCDirector::sharedDirector()->getRunningScene() ->addChild( pMessage, 100 );
        return;
    }
    
    g_pPetDataBlock->pFeedPetData->MaxFeedCount++;
}
//----------------------------------------------------------------------------------------
//
//
void CCFeedLayer::ItemCompleteNo( CCNode* pNode )
{
    close();
}
//--------------------------------------------------
//
//
void CCFeedLayer::close()
{
    CGameWorldScene* pWorldLayer =  ( CGameWorldScene* )getParent();
    pWorldLayer->EnableAllWorldButton( true );
    getParent()->removeChild(this,true);
}
//---------------------------------------------------
//
//
void CCFeedLayer::OnItemSelect( int ItemId )
{
	m_pItemLogGroup->removeAllChildrenWithCleanup( true );

	CBaseDataItem *pBaseDataItem = g_pClientLoader->GetBaseDataItemFromID( ItemId );

	CCNode* pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_feednum_1" );

	char buffer[64];
	sprintf( buffer, "%s %d", pBaseDataItem->itemname.c_str(), MAX_ITEMGROUP1 );
	CCLabelTTF *pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
	
    CCRenderTexture* pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_foodexp_1" );

	sprintf( buffer, "%d", pBaseDataItem->itemexp * MAX_ITEMGROUP1 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_skillexp_1" );
	sprintf( buffer, "%d", pBaseDataItem->itemskillexp * MAX_ITEMGROUP1 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );
	//
	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_feednum_2" );

	sprintf( buffer, "%s %d", pBaseDataItem->itemname.c_str(), MAX_ITEMGROUP2 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_foodexp_2" );

	sprintf( buffer, "%d", pBaseDataItem->itemexp * MAX_ITEMGROUP2 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_skillexp_2" );
	sprintf( buffer, "%d", pBaseDataItem->itemskillexp * MAX_ITEMGROUP2 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );
	
	//

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_feednum_3" );

	sprintf( buffer, "%s %d", pBaseDataItem->itemname.c_str(), MAX_ITEMGROUP3 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_foodexp_3" );

	sprintf( buffer, "%d", pBaseDataItem->itemexp * MAX_ITEMGROUP3 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_skillexp_3" );
	sprintf( buffer, "%d", pBaseDataItem->itemskillexp * MAX_ITEMGROUP3 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	//
	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_feednum_4" );

	sprintf( buffer, "%s %d", pBaseDataItem->itemname.c_str(), MAX_ITEMGROUP4 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_foodexp_4" );

	sprintf( buffer, "%d", pBaseDataItem->itemexp * MAX_ITEMGROUP4 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	pNode = GetXMLNodeFromKey( "t2dSceneObject_peteat_skillexp_4" );
	sprintf( buffer, "%d", pBaseDataItem->itemskillexp * MAX_ITEMGROUP4 );
	pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabel->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabel, 1, kFontSystem[FONT_THIN].strokeColor);
    m_pItemLogGroup->addChild( pstroke );
	m_pItemLogGroup->addChild( pLabel );

	m_iCurItemId = ItemId;

	ShowCanFeedButton();
    
    if( !g_pPetDataBlock->recordsave.isNotBeginner[3] && g_pPetDataBlock->recordsave.isNotBeginner[2] )
    {
        CCSprite *pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4_2");
        pSprite->setIsVisible(true);
        reorderChild(pSprite, 1000);
        
        pSprite = ( CCSprite*)GetXMLNodeFromKey("t2dAnimatedSprite_teacher_4_1");
        pSprite->setIsVisible(false);
    }

}
//--------------------------------------------------
//
//
void CCFeedLayer::ShowCanFeedButton()
{
	SFeedDataItem* pSaveData = g_pPetDataBlock->GetSaveItemFromId( m_iCurItemId );

	for( int i = 0; i < 4; i++ )
	{
		m_pFeedMenu[i]->setIsVisible( false );
	}

	if( pSaveData->itemCount >= MAX_ITEMGROUP1 && !pSaveData->isActive )
	{
		m_pFeedMenu[0]->setIsVisible( true );
	}
    if( pSaveData->itemCount >= MAX_ITEMGROUP2 && !pSaveData->isActive  )
	{
		m_pFeedMenu[1]->setIsVisible( true );
	}
	if( pSaveData->itemCount >= MAX_ITEMGROUP3 && !pSaveData->isActive )
	{
		m_pFeedMenu[2]->setIsVisible( true );
	}
	if( pSaveData->itemCount >= MAX_ITEMGROUP4 && !pSaveData->isActive )
	{
		m_pFeedMenu[3]->setIsVisible( true );
	}
}
//-------------------------------------------------
//
//
void CCFeedLayer::onExit()
{
	CCXMLLayer::onExit();
}
//---------------------------------------------
//
//
void CCFeedLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{

}
//---------------------------------------------
//
//
void CCFeedLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{

}
//---------------------------------------------
//
//
void CCFeedLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

}

//----------------------------------------------------
//
//
