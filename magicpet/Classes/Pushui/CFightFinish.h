/*****************************************************************************/
//File name:	filecpp
//Author: 		huyan
//Description:	
/******************************************************************************/
#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "../loader/CClientLoader.h"
#include "../CCXMLLayer.h"

class CFightFinish : public CCXMLLayer
{

public:
	CFightFinish( bool isWin );
	~CFightFinish();

	void onEnter();
	void onExit();

	void FadeOut();

	bool m_bIsWin;

};