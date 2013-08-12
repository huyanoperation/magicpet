 /*****************************************************************************/
//File name:	filecpp
//Author: 		huyan
//Description:	任务完成弹出界面、
/******************************************************************************/
#include "CLoginDay.h"
#include "../GameConstant.h"
#include "../CGameMessage.h"
#include "../Sprite/CPet.h"

CLoginDay::CLoginDay(   )
{
}
//------------------------------------------------------------------
//
//
CLoginDay::~CLoginDay()
{
	
}
//---------------------------------------------------------------------
//
//
void CLoginDay::onEnter()
{
	CCXMLLayer::onEnter();
	LoadPlist( "Landaward.plist" );

	CCNode *pNode =  GetXMLNodeFromKey( "t2dSceneObject_money" );

	CCLabelTTF *pLabel  = CCLabelTTF::labelWithString( "+1000", kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
	pLabel->setPosition( pNode->getPosition() );
	pLabel->setColor( ccBlack );
	addChild( pLabel,100 );

	pNode =  GetXMLNodeFromKey( "t2dSceneObject_stamina" );
	pLabel  = CCLabelTTF::labelWithString( "+100", kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
	pLabel->setPosition( pNode->getPosition() );
	pLabel->setColor( ccBlack );
	addChild( pLabel,100 );

	g_pPetDataBlock->maxstaminapoint = 100;
	g_pPetDataBlock->petmoney        += 99999;


}
void CLoginDay::FadeOut()
{
	for( int i = 0; i < getChildren()->count(); i ++ )
	{
		((CCNode*)(getChildren()->objectAtIndex(i)))->runAction( CCSequence::actions( CCDelayTime::actionWithDuration(3.0f), CCFadeOut::actionWithDuration( 5.0f ),NULL ) );
	}
}
//-----------------------------------------------------------------------
//
//
void CLoginDay::onExit()
{
	CCXMLLayer::onExit();
}
