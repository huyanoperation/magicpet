/*****************************************************************************/
//File name:	CBaseDataSkill
//Author: 		huyan
//Description:	
/******************************************************************************/
#pragma once

#include "cocos2d.h"

//"id":1,
//"attack_frame":44,
//"damage":100,
//"skillmp":35,
//"improveskilllevelExp":100,
//"animid":2,
//"plist":"fireballEffect.plist" 

class CBaseDataSkill
{

public:
	CBaseDataSkill();
	~CBaseDataSkill();

	short int m_iID;

	int attack_frame;
	int damage;
	int skillmp;
	int improveskilllevelExp;
	int animid;
	string plist; 
	string skillicon;
    string skill_sound;
    
};