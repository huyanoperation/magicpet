/*****************************************************************************/
//File name:	CPet
//Author: 		huyan
//Description:	
/******************************************************************************/
#pragma once

#include "GameConstant.h"
#include "AdvanceSprite.h"

// event

#define MAX_SKILL_EXP 5000
//--------------------------------------Item------------------------------------------
struct SFeedDataItem
{
	int  itemId;
	int  leaveTime;
	int  itemCount;
    int  activeCount;
	bool isActive;
};

#define MAX_ITEM_HAVE_DFINE 64

struct SFeedPetDataSave
{
	SFeedDataItem feedItem[MAX_ITEM_HAVE_DFINE];
    int MaxFeedCount;
};
//---------------------------------------------------------------------------------
//-------------------------message-------------------------------------------
#define MAX_PET_DATA_SAVE_MESSAGE 20
struct SPetDataSaveMessage
{
	int type;
	char gameDay[512];
	char strMessage[512];
	bool isHas;
    char name[32];
};

//-----------------------------------record------------------------------------------
struct SPetDataRecord
{
	SFeedDataItem Item[MAX_ITEM_HAVE_DFINE];
	int winBattleCount;
	int loseBattleCount;
	SPetDataSaveMessage saveMessage[MAX_PET_DATA_SAVE_MESSAGE];
    int HitWoodCount;
	bool isHaveNewMessage;
    int isNotBeginner[9];
    int weiboGetMoneyCount;
};
//----------------------------------------------------------------------------------
struct SGameTime
{
	unsigned int gameRunTime;
    unsigned int lastGameTime;
};
//--------------------------------------------------------------------------------------------
#define MAX_SKILL_EXP_COUNT 3
struct SSkillExp
{
	int skillsExp[MAX_SKILL_EXP_COUNT];
};
//-------------------------------------------------------------------------------------------
struct SSkillLevel
{
	int skillsLevel[MAX_SKILL_EXP_COUNT];
};
//--------------------------------------------------------------------------------------------
#define OTHERUSERS_NUM 10240
struct SOtherUser
{
    char uin[256];
    char name[256];
};

struct SOtherUsers  
{
    int count;
    SOtherUser  otheruser[OTHERUSERS_NUM];
};

class PetDatablock
{
public:
	PetDatablock();
	~PetDatablock();

	int petid;
	int petsex;
	int SkillFrequency;
	string petName;
	int petlevel;
	int petexp;
	SSkillLevel skilllevel;
	SSkillExp skillexp;
	int pettitle;
	int playershoppingmallmoney;
	int maxstaminapoint;
	int petturning[2];
	int petskillexppond;
	int itemnum;
	int petmoney;
	int ResId;
	int MaxHp;
	int hp;
	int MaxMp;
	int mp;
	int rbg[3];
	SGameTime gametime;
	int gameday;
    
    string userid;
    string playername;

	SPetDataRecord recordsave;
	SFeedPetDataSave *pFeedPetData;
    
    SOtherUsers otherUsers;
    

	void addItem( int Id, int count = 1 );

	SFeedDataItem *GetSaveItemFromId( int ID );
	void addSkillExp( int skillExp, CCPoint pos );
	void addPetExp( int data, CCPoint pos );
	void addPetMoney( int data, CCPoint pos );
	void addPlayerMoney( int data, CCPoint pos );
	void addGameMessage( string gameday, string message, int type = 0, string name = "" );
    bool subPlayershoppingmallmoney( int num );
    bool subPetMoney( int num );
    bool submaxstaminapoint( int num );

};
extern PetDatablock *g_pPetDataBlock;

struct SAttackType
{
	int Type;
};

#define FIRE_BALL_TAG 10

enum
{
	CPET_STATE_NONE,
	CPET_STATE_DISTANCE_ATTACK,
};

class CPet : public AdvanceSprite
{
public:
	CPet();
	~CPet();

	void onEnter();
	void onExit();

	void Attack();
	void ApplyHurt( int damage, bool isHardAttack = false );

	void upMainData( ccTime dt  );

	void setTurnEndCallBack( SelectorProtocol* pSelectorTarget, SEL_CallFuncND selector, void* d );
	
	void AttackBeginCallBack( CCNode *pNode, void *d );
	void AttackEndCallBack();
	void HurtEndCallBack();
	void AttackOverCallBack( CCNode *pNode );
	void setTargetPosition( CCPoint pos );
	void setPetDataBlock( void *pData );
	int  getID(){ return m_PetData.petid; }
	void setLockTarget( CPet* pet );
	void setHp( int hp ){ m_PetData.hp = hp; }
	int  getHp(){ return m_PetData.hp; }
	void setMaxHp( int hp ){ m_PetData.MaxHp = hp; }
	int  getMaxHp(){ return m_PetData.MaxHp; }
	string getName(){ return m_PetData.petName; }

	void setMp( int mp ){ m_PetData.mp = mp; }
	int  getMp(){ return m_PetData.mp; }
	void setMaxMp( int mp ){ m_PetData.MaxMp = mp; }
	int  getMaxMp(){ return m_PetData.MaxMp; }

	PetDatablock *GetPetData(){ return &m_PetData; }
    
     bool m_bIsWin;
protected:
	SEL_CallFuncND m_pCallFuncND;
	SelectorProtocol* m_pSelectorTarget;
	CCPoint m_TargetPositon;

	CCPoint m_StandPositon;

	CC_SYNTHESIZE( int, m_iSitId, SitId );

	CPet *m_pLockTarget;

	int m_iState;

	PetDatablock m_PetData;
   
	//“‘∫Û“™±‰
	//CC_SYNTHESIZE( int, m_iID, ID );
};