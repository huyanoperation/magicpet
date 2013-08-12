/*****************************************************************************/
//File name:	filecpp
//Author: 		huyan
//Description:	CShopLayer¡¢
/******************************************************************************/
#pragma once

#include "cocos2d.h"
using namespace cocos2d;

#include "../loader/CClientLoader.h"
#include "../CCXMLLayer.h"

class CShopLayer : public CCXMLLayer
{

public:
	CShopLayer();
	~CShopLayer();

	void onEnter();
	void onExit();
    
    static void BuyResult( string str );

	void menuCallback(CCObject * pSender);
};