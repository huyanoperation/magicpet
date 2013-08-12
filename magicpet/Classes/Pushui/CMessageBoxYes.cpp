//
//  CMessageBoxYes.cpp
//  magicpet
//
//  Created by jia huyan on 12-3-8.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include <iostream>

#include "CMessageBoxYes.h"

//---------------------------------------------------------
//
//
CMessageBoxYes::CMessageBoxYes( char *Message, char *title, SEL_CallFunc pSelector, SelectorProtocol *pTarget )
{
    strcpy( m_szBuffer, Message );
    strcpy( m_TitleBuffer, title );
    //Callback funciton pointer.
    m_pfnSelectorDelegate = pSelector;
    m_pTarget = pTarget;
}
//---------------------------------------------------------
//
//
CMessageBoxYes::~CMessageBoxYes()
{
    
}
//---------------------------------------------------------
//
//
void CMessageBoxYes::onEnter()
{
    CCXMLLayer::onEnter();
    
    LoadPlist( "tip_2.plist" );
    
    const char *MenuButton[] = { "button_yes" };
    for( int i = 0; i < 1; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CMessageBoxYes::menuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
        removeChild( pSprite, true );
        
       
        CCLabelTTF* pLabelTTF = CCLabelTTF::labelWithString( "ok", kFontSystem[FONT_GRAND].fontName, kFontSystem[FONT_GRAND].fontSize );
        pLabelTTF->setPosition( pMenu->getPosition() );
        CCRenderTexture* pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_GRAND].strokeColor);
        addChild( pstroke,100 );
        addChild( pLabelTTF,100 );
	}
    
    CCNode *pNode =  GetXMLNodeFromKey( "t2dSceneObject_Message" );
	CCLabelTTF* pLabelTTF = CCLabelTTF::labelWithString( m_szBuffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    CCRenderTexture* pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );
    
    pNode =  GetXMLNodeFromKey( "t2dSceneObject_tipname" );
	pLabelTTF = CCLabelTTF::labelWithString( m_TitleBuffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	pLabelTTF->setPosition( pNode->getPosition() );
    pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    addChild( pstroke,100 );
	addChild( pLabelTTF,100 );
}
//---------------------------------------------------------
//
//
void CMessageBoxYes::onExit()
{
    CCXMLLayer::onExit();
}
//------------------------------------------------------
//
//
void CMessageBoxYes::menuCallback(CCObject * pSender)
{
    if(m_pTarget != 0 && m_pfnSelectorDelegate != 0)
    {
        (m_pTarget->*m_pfnSelectorDelegate)();
    }
    removeFromParentAndCleanup(true);
}
//------------------------------------------------------
//
//
void CMessageBoxYes::UpData( ccTime tt )
{
    
}