//
//  CGameScene.cpp
//  magicpet
//
//  Created by jia huyan on 12-2-24.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include <iostream>
#include "CGameScene.h"

CGameScene::CGameScene()
{
    
}
//------------------------------------
//
//
CGameScene::~CGameScene()
{
    
}
    
    
void CGameScene::onEnter()
{
    CCScene::onEnter();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( CCDirector::sharedDirector()->getWinSize().width >= 1024  )
    {
        setScaleX( 1.06f );
        setScaleY( 1.2f );
    }

#endif

}
    
    

