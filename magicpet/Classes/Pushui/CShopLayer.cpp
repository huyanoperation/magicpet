/*****************************************************************************/
//File name:	filecpp
//Author: 		huyan
//Description:	CShopLayer¡¢
/******************************************************************************/
#include "CShopLayer.h"
#include "../GameScene/CGameWorldScene.h"
#include "../Platform/CCNd91Mng.h"
#include "../CGameText.h"

CShopLayer::CShopLayer()
{

}

CShopLayer::~CShopLayer()
{

}
//------------------------------------
//
//
void CShopLayer::onEnter()
{
	CCXMLLayer::onEnter();

	LoadPlist("gameshop.plist");

	const char *MenuButton[] =
	{
		"button_back",
        "button_1",
        "button_2",
        "button_3",
        "button_4",
        "button_91"
	};

	for( int i = 0; i < 6; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CShopLayer::menuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
		removeChild( pSprite, true );
	}
    
    CCNode *pNode = GetXMLNodeFromKey( "t2dSceneObject_money_1" );
    
    char buffer[32];
    sprintf( buffer, shop_1[g_iCurLanguageIndex], 1 );
    string utf8;
	CChineseCode::GB2312ToUTF_8( utf8, buffer, 256 );
    
	CCLabelTTF *plabelName = CCLabelTTF::labelWithString(buffer,  kFontSystem[FONT_MED].fontName,kFontSystem[FONT_MED].fontSize);
	addChild(plabelName,99);
	plabelName->setPosition( pNode->getPosition() );
    
    
    pNode = GetXMLNodeFromKey( "t2dSceneObject_money_2" );
    
    sprintf( buffer, shop_1[g_iCurLanguageIndex], 4 );
	CChineseCode::GB2312ToUTF_8( utf8, buffer, 256 );
    
	plabelName = CCLabelTTF::labelWithString(buffer,  kFontSystem[FONT_MED].fontName,kFontSystem[FONT_MED].fontSize);
	addChild(plabelName,99);
	plabelName->setPosition( pNode->getPosition() );
}
//-------------------------------------------
//
//
void CShopLayer::onExit()
{
	CCXMLLayer::onExit();
}
//-----------------------------------------
//
//
void CShopLayer::BuyResult( string str )
{
    if( str == "108355-20120323-145155338-37" )
    {
        g_pPetDataBlock->playershoppingmallmoney += 1;
    }
    else if( str == "108355-20120323-151357042-69" )
    {
        g_pPetDataBlock->playershoppingmallmoney += 5;
    }
    
}
//---------------------------------------------
//
//
void CShopLayer::menuCallback( CCObject * pSender )
{
    int tag = ((CCNode*)pSender)->getTag();
    
    switch ( tag ) 
    {
        case 0:
        {
            CGameWorldScene* pWorldLayer =  ( CGameWorldScene* )getParent();
            pWorldLayer->EnableAllWorldButton( true );
            removeFromParentAndCleanup( true );
        }
        break;
        case 1:
        {
            CNd91Mng::buyVirtualGoodsWithId( "108355-20120323-145155338-37", 1);
        }
        break;
        case 2:
        {
            CNd91Mng::buyVirtualGoodsWithId( "108355-20120323-151357042-69", 1);
        }
        break;
        case 3:
        {
            if( g_pPetDataBlock->playershoppingmallmoney > 0 )
            {
                g_pPetDataBlock->playershoppingmallmoney--;
                g_pPetDataBlock->petmoney += 5000;
            }
        }
        break;
        case 4:
        {
            if( g_pPetDataBlock->petmoney >= 6500 )
            {
                g_pPetDataBlock->playershoppingmallmoney++;
                g_pPetDataBlock->petmoney -= 6500;
            }
        }
            break;
        case 5:
        {
            CNd91Mng::enter91Platform();
        }
        break;
            
    }

}