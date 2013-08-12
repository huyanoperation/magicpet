/*****************************************************************************/
//File name:	CBaseDataPet
//Author: 		huyan
//Description:	
/******************************************************************************/
#pragma once

#include "cocos2d.h"

//"id":1,
//"petname":"dahou",
//"petsex":0,
//"pettitle":1,
//"petlevel":1,
//"petexp":100,
//"initstr":100,
//"upstr":100,
//"initint":100,
//"upint":100,
//"initdex":100,
//"updex":100,
//"initskill":100,
//"upskill":100,
//"inithp":100,
//"initmp":100,
//"straptitude":100,
//"intaptitude":100,
//"dexaptitude":100,
//"skillaptitude":100,
//"maxstaminapoint":100,
//"skillfield":[1,2,3] 

class CBaseDataPet
{

public:
	CBaseDataPet();
	~CBaseDataPet();

	short int m_iID;
	string petname;
	int petsex;
	int pettitle;
	int petlevel;
	int petexp;
	int initstr;
	float upstr;
	int initint;
	float upint;
	int initdex;
	float updex;
	int initskill;
	float upskill;
	int inithp;
	int initmp;
	int straptitude;
	int intaptitude;
	int dexaptitude;
	int skillaptitude;
	int maxstaminapoint;
	int skillfield[3];
	int skillfield_defense[3];
	int skillfield_3[3];
	int animationid;
	int rgb[3];
	string petshow;
    string evolutionpist;
    int    animation[3];
	string evolution_button;
    string attack_sound;

};