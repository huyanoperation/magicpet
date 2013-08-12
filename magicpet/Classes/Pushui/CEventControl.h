//-------------------------------------------------------------------------------------
//
//	Author:huyan
//	玩家任务事件控制 
//
//
//-------------------------------------------------------------------------------------
#pragma once
#include "cocos2d.h"
#include "../Loader/CCLientLoader.h"
//#include "CEventLog.h"

//=====================================================================================
//
// 任务控制
//
//=====================================================================================
#define MAX_EVENT_COUNTER 1
#define MAX_TASK_EVNET 256
#define MAX_EVENT_FLAG 256

#define SPECIAL_COUNTER_BATTLE_WIN		10
#define SPECIAL_COUNTER_BATTLE_LOSE		11
#define SPECIAL_COUNTER_BATTLE			12

#define SUB_EVENT_FLAG_MAX 128 


struct SEventCounter
{
	int CurCount;
	int MaxCount;
	int CounterId;
};

struct SNMSaveEvent
{
	int EventId;
	SEventCounter GameCounter[MAX_EVENT_COUNTER];
};

enum
{
	EVENT_STATE_NONE,
	EVENT_STATE_NORMAL,
	EVENT_STATE_NEW
};

#define TASK_EVNET_DIALOG_TAG		 1024
#define TASK_EVNET_DIALOG_NORMAL_TAG 1025

class  CEventControl :public  CCNode
{
public:
	CEventControl();
	~CEventControl();

	// 任务相关处理
	SEventPoint		*GetCurrSubEventPoint()				  { return &m_TempEventPoint; } 
	void			 SetEventFlag( int eventId, int value );
	int				 GetEventFlag( int eventId );
	void			 SetEventPointFlag( int eventId, int value  );
	int				 GetEventPointFlag( int eventId );
	void			 InitEvent();
	void			 ClearEventFlag();
	void			 NextEventPoint();
	int				 GetCurTaskCount();
	int				 GetCurUnFinishTaskCount();
	void			 AddTaskData( CBaseDataEvent* pBaseDataEvent);
	void			 RemoveTaskData( int EventId );

	bool			 CheckAllSubEventIsAchieve();

	bool			 CheckCurrentEventPointLimit();
	bool			 CheckSubEvent( int EventId );
	void			 upData( ccTime dt );

	int				 GetEventState(){ return m_iEventState; }
	void			 SetEventState( int state ){ m_iEventState = state; }
	bool			 GetIsInitEvent(){ return m_iIsInit; }
	void			 PushDialogEvent();
	void			 FreshCounter( int Id, int num, int max );
	void			 AddCounterNum(  int num, int counterType, int Id );
	int				 GetCurTaskCounterNum( int limitId );
	int				 GetCurTaskCounterNumFromEventId( int EventId );

   SNMSaveEvent		m_TaskData[MAX_TASK_EVNET];
   int				m_iEventFlag[MAX_EVENT_FLAG];

   int				m_iTempSoulCount;
   int				m_iCheatCode;
   int				m_iTempFightMonsterDead;
   bool				m_bTempFightWin;

protected:

   SEventPoint 	m_TempEventPoint;
   int			m_iEventState;
   bool			m_iIsInit;

	
};

extern CEventControl *g_pPlayerEventCtrl;
