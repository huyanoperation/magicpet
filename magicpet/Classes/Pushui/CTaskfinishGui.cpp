 /*****************************************************************************/
//File name:	filecpp
//Author: 		huyan
//Description:	任务完成弹出界面、
/******************************************************************************/
#include "CTaskFinishGui.h"
#include "../GameConstant.h"
#include "../CGameMessage.h"

CTaskFinishGui::CTaskFinishGui( int eventId  )
{
	m_iEventId = eventId;
	setIsTouchEnabled( true );
}
//------------------------------------------------------------------
//
//
CTaskFinishGui::~CTaskFinishGui()
{
	
}
//---------------------------------------------------------------------
//
//
void CTaskFinishGui::onEnter()
{
	CCXMLLayer::onEnter();
	LoadPlist( "achievement_complete.plist" );

	CBaseDataEvent* pBaseDataEvent =  g_pClientLoader->GetEventDataFromId(  m_iEventId );

	CCNode *pNode =  GetXMLNodeFromKey( "t2dSceneObject_event_icon" );

	string path = GetGameImagesPath();
	path = path + pBaseDataEvent->m_SubEvent.eventicon;
	CCSprite *pSprite = CCSprite::spriteWithFile( path.c_str() );
	pSprite->setPosition( pNode->getPosition() );
	addChild( pSprite );

	pNode =  GetXMLNodeFromKey( "t2dSceneObject_event_name" );
	
	string utf8;
	CChineseCode::GB2312ToUTF_8( utf8, ( char* )CEventName[pBaseDataEvent->m_SubEvent.m_wSubEventNameID][g_iCurLanguageIndex], 256 );
	CCLabelTTF *pLabel  = CCLabelTTF::labelWithString( utf8.c_str(), kFontSystem[FONT_LARGE].fontName, kFontSystem[FONT_LARGE].fontSize );
	pLabel->setPosition( pNode->getPosition() );
	addChild( pLabel );

}
void CTaskFinishGui::FadeOut()
{
	for( int i = 0; i < getChildren()->count(); i ++ )
	{
		((CCNode*)(getChildren()->objectAtIndex(i)))->runAction( CCFadeOut::actionWithDuration( 5.0f ) );
	}
}
//-----------------------------------------------------------------------
//
//
void CTaskFinishGui::onExit()
{
	CCXMLLayer::onExit();
}
