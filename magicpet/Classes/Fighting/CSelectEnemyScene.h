//
//  CSelectEnemyScene.h
//  magicpet
//
//  Created by jia huyan on 12-2-7.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#pragma once
#include "GameConstant.h"
#include "../Sprite/CPet.h"
#include "../GameScene/CGameScene.h"

enum  
{
    CSELECTENEMYSCENE_BUTTON_FIGHT_1,
    CSELECTENEMYSCENE_BUTTON_FIGHT_2,
    CSELECTENEMYSCENE_BUTTON_FIGHT_3,
    CSELECTENEMYSCENE_BUTTON_FIGHT_4,
};

class CSelectEnemyScene : public CGameScene
{
    
public:
    CSelectEnemyScene( bool isUser = false );
    ~CSelectEnemyScene();
    
    static void Show( bool isOtherUser = false );
	virtual void onEnter();
	virtual void onExit();
    
    void menuCallback(CCObject * pSender);
    
    static PetDatablock *s_pPetDataBlock;
    
    PetDatablock *m_pRandomPet1;
    PetDatablock *m_pRandomPet2;
    PetDatablock *m_pRandomPet3;
    PetDatablock *m_pRandomPet4;
    
    PetDatablock* GetOthers( string uin, string name );
    PetDatablock* GetNPC();
    
    bool m_bIsUser;
};