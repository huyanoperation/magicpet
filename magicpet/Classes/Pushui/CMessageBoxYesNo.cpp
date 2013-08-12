//
//  CMessageBoxYes.cpp
//  magicpet
//
//  Created by jia huyan on 12-3-8.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include <iostream>

#include "CMessageBoxYesNo.h"


bool CMessageBoxYesNo::m_bIsSelfDelete = false;
//---------------------------------------------------------
//
//
CMessageBoxYesNo::CMessageBoxYesNo( char *Message, SEL_CallFuncN pSelectorYes, SEL_CallFuncN pSelectorNo,SelectorProtocol *pTarget, int tag, bool isSelfDelete  )
{
    strcpy( m_szBuffer, Message );
    //Callback funciton pointer.
    m_pfnSelectorDelegateYes = pSelectorYes;
    m_pfnSelectorDelegateNo = pSelectorNo;
    m_pTarget = pTarget;
    setTag( tag );
    
    m_bIsSelfDelete = isSelfDelete;
}
//---------------------------------------------------------
//
//
CMessageBoxYesNo::CMessageBoxYesNo()
{
    
}
//---------------------------------------------------------
//
//
void CMessageBoxYesNo::onEnter()
{
    CCXMLLayer::onEnter();
    
    LoadPlist( "tip_1.plist" );
    
    const char *MenuButton[] = { "button_yes", "button_no" };
    
    const char *YesNo[] = { "YES", "NO" };
    for( int i = 0; i < 2; i++ )
	{
		CCSprite *pSprite= (CCSprite*)GetXMLNodeFromKey( MenuButton[i] );
		CCMenu *pMenu = CreatGameUIWithSprite( pSprite, menu_selector(CMessageBoxYesNo::menuCallback), i, this, pSprite->getPosition() );
		addChild( pMenu, pSprite->getZOrder() );
        removeChild( pSprite, true );
        
        
        CCLabelTTF* pLabelTTF = CCLabelTTF::labelWithString( YesNo[i], kFontSystem[FONT_GRAND].fontName, kFontSystem[FONT_GRAND].fontSize );
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
    
    //pNode =  GetXMLNodeFromKey( "t2dSceneObject_tipname" );
	//pLabelTTF = CCLabelTTF::labelWithString( m_TitleBuffer, kFontSystem[FONT_THIN].fontName, kFontSystem[FONT_THIN].fontSize );
	//pLabelTTF->setPosition( pNode->getPosition() );
    //pstroke = createStroke(pLabelTTF, 1, kFontSystem[FONT_THIN].strokeColor);
    //addChild( pstroke,100 );
	//addChild( pLabelTTF,100 );
}
//---------------------------------------------------------
//
//
void CMessageBoxYesNo::onExit()
{
    CCXMLLayer::onExit();
}
//------------------------------------------------------
//
//
void CMessageBoxYesNo::menuCallback(CCObject * pSender)
{
    int tag = ((CCNode*)pSender)->getTag();
    switch( tag )
    {
        case 0:
            if(m_pTarget != 0 && m_pfnSelectorDelegateYes != 0)
            {
                (m_pTarget->*m_pfnSelectorDelegateYes)((CCNode*)this);
            }
            break;
        case 1:
            if(m_pTarget != 0 && m_pfnSelectorDelegateNo != 0)
            {
                (m_pTarget->*m_pfnSelectorDelegateNo)((CCNode*)this);
            }
            break;
    }
    if( m_bIsSelfDelete )
    {
        removeFromParentAndCleanup(true);
    }
}
//------------------------------------------------------
//
//
void CMessageBoxYesNo::UpData( ccTime tt )
{
    
}