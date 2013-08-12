/*****************************************************************************/
//File name:	CExpressions
//Author: 		huyan
//Description:	fight¡¢
/******************************************************************************/
#pragma once
#include "../GameConstant.h"

//typedef struct _PetData
//{
//	int ID;
//	int Strength;
//	int Intelligence;
//	int Agility;
//	int Technique;
//	
//	int hp;
//	int mp;
//	int Damage;
//	int SkillFrequency;
//	string petName;
//
//}PetData;

class CExpressions
{
public:
	CExpressions();
	~CExpressions();

	static CExpressions* GetInstance();

	static CExpressions* m_pInstance;

	int GetAttribute( int Aptitude, float Up, int Init, int level );
	int GetDamage( int Agility );

	
	//int GetAddHP( int curLevelData);
	//int GetAddMP( int curLevelData );
	float GetSkillFrquency( int skill, int level );

};