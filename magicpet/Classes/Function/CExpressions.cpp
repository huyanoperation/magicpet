/*****************************************************************************/
//File name:	CExpressions
//Author: 		huyan
//Description:	fight¡¢
/******************************************************************************/
//---------------------------------------------------------------

#include "CExpressions.h"


CExpressions* CExpressions::m_pInstance = NULL;

CExpressions::CExpressions()
{

}
//-----------------------------------------------------------------
//
//
CExpressions::~CExpressions()
{

}
//-----------------------------------------------------------------
//
//
CExpressions * CExpressions::GetInstance()
{
	if( NULL == m_pInstance )
	{
		m_pInstance = new CExpressions();
	}
	return m_pInstance;
}
//------------------------------------------------------------------
//ex:Str=[( StrAptitude X UpStr) + InitStr + Level] / 2 
//
int CExpressions::GetAttribute( int Aptitude, float Up, int Init, int level )
{
	return ( ( Aptitude * Up) + Init + level);
}

//------------------------------------------------------------------
//¼´£ºDamage=(1+Dex) X Level
//
int CExpressions::GetDamage( int Agility )
{
	return ( 1 + Agility ) + 10;
}
//---------------------------------------------------------------------
//AddHp=[£¨UpLevelStr ¨C LevelStr£©]X10
//
//int CExpressions::GetAddHP( int curLevelData )
//{
//	return ( curLevelData) * 10;
//}
////-----------------------------------------------------------------------
////
////
//int CExpressions::GetAddMP( int curLevelData )
//{
//	return ( curLevelData) * 15;
//}
//---------------------------------------------------------------------
//Skillfrequency =30%+[(Skill X Level X 7%)]0.1%
//
float CExpressions::GetSkillFrquency( int skill, int level )
{
	return 0.3 + ( (skill* level * 0.07f ) ) * 0.001f;
}