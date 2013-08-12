#include "CFightFinish.h"
#include "../Sprite/CPet.h"

CFightFinish::CFightFinish( bool isWin )
{
	m_bIsWin = isWin;
}
//--------------------------------------------------------------
//
//
CFightFinish::~CFightFinish()
{

}
//--------------------------------------------------------------------
//
//
void CFightFinish::onEnter()
{
	CCXMLLayer::onEnter();

	CCXMLLayer::onEnter();
	LoadPlist( "fightbonus.plist" );


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


	if( m_bIsWin )
	{
		CCNode *pNode =  GetXMLNodeFromKey( "t2dSceneObject_money" );
		
		CCLabelTTF *pLabel  = CCLabelTTF::labelWithString( "+100", kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
		pLabel->setColor( ccBlack );
		pLabel->setPosition( pNode->getPosition() );
		addChild( pLabel,100 );

		char buffer[32];
		sprintf( buffer, "+%d",  ( int )( MaxExp * 0.05 ) );
		pNode =  GetXMLNodeFromKey( "t2dSceneObject_exp" );
		pLabel  = CCLabelTTF::labelWithString( buffer, kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
		pLabel->setPosition( pNode->getPosition() );
		pLabel->setColor( ccBlack );
		addChild( pLabel,100 );

		g_pPetDataBlock->petexp += ( int )( MaxExp * 0.05 );
		g_pPetDataBlock->petmoney += 100;
	}
	else
	{
		CCNode *pNode =  GetXMLNodeFromKey( "t2dSceneObject_money" );

		CCLabelTTF *pLabel  = CCLabelTTF::labelWithString( "+0", kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
		pLabel->setPosition( pNode->getPosition() );
		pLabel->setColor( ccBlack );
		addChild( pLabel,100 );

		pNode =  GetXMLNodeFromKey( "t2dSceneObject_exp" );
		pLabel  = CCLabelTTF::labelWithString( "+0", kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
		pLabel->setPosition( pNode->getPosition() );
		pLabel->setColor( ccBlack );
		addChild( pLabel,100 );

	}
}
//--------------------------------------------------------------------
//
//
void CFightFinish::onExit()
{
	CCXMLLayer::onExit();
}
//-----------------------------------------------------------------------
//
//
void CFightFinish::FadeOut()
{

}