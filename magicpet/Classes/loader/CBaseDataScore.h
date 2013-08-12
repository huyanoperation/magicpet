/*****************************************************************************/
//File name:	CBaseDataSkill
//Author: 		huyan
//Description:	
/******************************************************************************/
#pragma once

#include "cocos2d.h"


class CBaseDataScore
{

public:
	CBaseDataScore();
	~CBaseDataScore();

	short int m_iID;

	int attack_frame;
	int index[8];
	int score;
    
};