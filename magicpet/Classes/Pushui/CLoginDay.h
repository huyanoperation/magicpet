//

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

class CLoginDay : public CCXMLLayer
{

public:
	CLoginDay( );
	~CLoginDay();

	void onEnter();
	void onExit();

	void FadeOut();

};