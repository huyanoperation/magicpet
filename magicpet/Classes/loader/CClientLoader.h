/*****************************************************************************/
//File name:	CClientLoader
//Author: 		huyan
//Description:	游戏策划数据读取、
/******************************************************************************/
#pragma once
#include "jsoncpp/json.h"
#include "CBaseDataPet.h"
#include "CBaseDataSkill.h"
#include "CBaseDataAnimation.h"
#include "CBaseDataItem.h"
#include "CBaseDataExp.h"
#include "CBaseDataEvent.h"
//#include "CBaseDataEvent.h"
#include "GameConstant.h"
#include "CBaseFunRunData.h"
#include "CBaseFunLimitData.h"
#include "CBaseDataScore.h"

class CClientLoader
{
public:
	CClientLoader();
	~CClientLoader();
private:
	vector<CBaseDataPet> m_vBaseDataPet;
	vector<CBaseDataSkill> m_vBaseDataSkill;
    vector<CBaseDataAnimation> m_vBaseDataAnimation;
	vector<CBaseDataItem> m_vBaseDataItem;
    vector<CBaseDataExp> m_vBaseDataExp;
	vector<CBaseDataEvent> m_vBaseDataEvent;
	vector<CBaseFunRunData> m_vBaseFunRunData;
	vector<CBaseFunLimitData> m_vBaseFunLimitData;
	vector<CBaseDataScore> m_vBaseDataScore;
public:
	void Init();
	void LoadBaseDataPet();
	CBaseDataPet *GetBaseDataPetFromId( int ID );
	void LoadBaseDataSkill();
	CBaseDataSkill *GetBaseDataSkillFromId( int ID );
    void LoadBaseDataAnimation();
    CBaseDataAnimation* GetBaseDataAnimationFromId( int ID );
	void LoadBaseDataItem();
	CBaseDataItem *GetBaseDataItemFromID( int ID );
	CBaseDataItem *GetBaseDataItemIndex( int index );
    void LoadBaseDataExp();
    CBaseDataExp *GetBaseDataExpFromLevel( int level );
	void LoadBaseEvent();
	CBaseDataEvent* GetEventDataFromId(int ID);
	void LoadBaseDataRun();
	CBaseFunRunData* GetFunRunDataFromId( int ID );
	void LoadBaseDataLimit();
	CBaseFunLimitData* GetFunLimitDataFromId( int ID );
	void LoadBaseDataScore();
	CBaseDataScore* GetBaseDataScore( int ID );

	bool isCheat( string strBuffer, string ReadBuffer );
    bool m_bIsCheat;
};

extern CClientLoader *g_pClientLoader;