//
//  CBaseDataAnimation.h
//  magicpet
//
//  Created by jia huyan on 12-1-9.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#pragma once

#include "cocos2d.h"


class CBaseDataAnimation
{
    
public:
	CBaseDataAnimation();
	~CBaseDataAnimation();
    
	 int m_iID;
	 int m_stand[3];
     int m_move[3];
	 int m_attack[3];
	 int m_defense[3];
    
     int m_injured[3];
     int m_win[3];
     int m_lost[3];
     int m_skill[3];
     int m_specialaction_1[3];
     int m_specialaction_2[3];
     int m_specialaction_3[3];
	 int m_iPlistType;
    
	string m_strPlist;
};

