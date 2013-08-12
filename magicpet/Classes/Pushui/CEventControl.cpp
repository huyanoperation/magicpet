//-------------------------------------------------------------------------------------
//
//	Author:huyan
//	ÕÊº“»ŒŒÒ ¬º˛øÿ÷∆ 
//
//
//
//-------------------------------------------------------------------------------------
#include "CEventControl.h"
#include "../Loader/CClientLoader.h"
#include "../Function/CltFunction.h"
#include "CTaskFinishGui.h"
#include "CTaskEventSubGui.h"
#include "../Platform/CCNd91Mng.h"
#include "../GameConstant.h"

CEventControl *g_pPlayerEventCtrl = NULL;

CEventControl::CEventControl()
{ 
	 memset( m_iEventFlag, 0, sizeof( int ) * MAX_EVENT_FLAG );
	 memset( m_TaskData, 0, sizeof( SNMSaveEvent ) * MAX_TASK_EVNET );

	 m_iEventState = EVENT_STATE_NONE;

	 m_iTempSoulCount = 0;

	 m_iIsInit = false;

	 m_iCheatCode = 0;
	 m_iTempFightMonsterDead = 0;
	 m_bTempFightWin = false;
}
//--------------------------------------------------------------------------------------
//
//
CEventControl::~CEventControl()
{

}
//------------------------------------------------------------------------------
//
//
void CEventControl::InitEvent()
{

}
//------------------------------------------------------------------------------
//
//
void CEventControl::NextEventPoint()
{

}
//--------------------------------------------------------------------------------
//
//
int CEventControl::GetCurUnFinishTaskCount()
{
	int count = 0;
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		if( m_TaskData[i].EventId != 0 && 0 == GetEventFlag( m_TaskData[i].EventId ) )
		{
			count++;
		}
	}
	return count;
}
//------------------------------------------------------------------------------
//
//
int CEventControl::GetCurTaskCount()
{
	int count = 0;
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		if( m_TaskData[i].EventId != 0 )
		{
			count++;
		}
	}
	return count;
}
//-----------------------------------------------------------------------------
//
//
void CEventControl::AddTaskData( CBaseDataEvent* pBaseDataEvent )
{
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		if(  m_TaskData[i].EventId == pBaseDataEvent->m_SubEvent.m_wId )
		{
			return;
		}
	}

	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		if( m_TaskData[i].EventId == 0 )
		{
			m_TaskData[i].EventId = pBaseDataEvent->m_SubEvent.m_wId;
			m_TaskData[i].GameCounter[0].CurCount = 0;

			CBaseFunLimitData *pData = g_pClientLoader->GetFunLimitDataFromId( pBaseDataEvent->m_SubEvent.m_iLimitId[0] ) ;
			if( NULL != pData )
			{
				m_TaskData[i].GameCounter[0].MaxCount = pData->m_arrParam[0];
				m_TaskData[i].GameCounter[0].CounterId = pData->m_arrParam[1];
			}

			break;
		}
	}

}
//-----------------------------------------------------------------------------
//
//
void CEventControl::RemoveTaskData( int EventId )
{
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		if(  m_TaskData[i].EventId == EventId)
		{
			memset( &m_TaskData[i], 0, sizeof( SNMSaveEvent ) );
			break;
		}
	}

}
//-----------------------------------------------------------------------------
//
//
void CEventControl::upData( ccTime dt )
{
	for( int i = 0; ; i++ )
	{
		if( 0 == GetEventPointFlag( i + 1 ) )
		{
			CBaseDataEvent* pBaseDataEvent =  g_pClientLoader->GetEventDataFromId( i + 1 );		
			if( NULL == pBaseDataEvent )
			{
				break;
			}
			m_TempEventPoint = pBaseDataEvent->m_EventPointInfo;
			//task point unlock
			if( CheckCurrentEventPointLimit()  )
			{	
				//…Ë÷√∆Ï±ÍŒ™1(Ω‚À¯◊¥Ã¨)
				m_iEventState = EVENT_STATE_NEW;
				AddTaskData( pBaseDataEvent );
				SetEventPointFlag( i + 1, 1 );

			}
			
		}
	}

	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		CBaseDataEvent* pBaseDataEvent =  g_pClientLoader->GetEventDataFromId(  m_TaskData[i].EventId );

		if( NULL != pBaseDataEvent )
		{
			if(  0 == GetEventFlag( m_TaskData[i].EventId ) && CheckSubEvent(  m_TaskData[i].EventId  ))
			{
				SetEventFlag( m_TaskData[i].EventId , 1 );
                
                for( int iRunCount = 0; iRunCount < pBaseDataEvent->m_SubEvent.m_wEventRunCount; iRunCount++ )
				{
					HandleExecRunFunc( pBaseDataEvent->m_SubEvent.m_iFunctionRun[iRunCount] );
				}

				for( int iRunCount = 0; iRunCount < pBaseDataEvent->m_EventPointInfo.m_iPointRunCount; iRunCount++ )
				{
					HandleExecRunFunc( pBaseDataEvent->m_EventPointInfo.m_iPointRunArray[iRunCount] );
				}
                
                //CNd91Mng::NdUnLockAchievement( pBaseDataEvent->m_EventPointInfo.m_w91Id );

				CTaskFinishGui *pGui = new CTaskFinishGui( m_TaskData[i].EventId );
				pGui->init();
				pGui->autorelease();
				CCSize s = CCDirector::sharedDirector()->getWinSize();
				pGui->setContentSize( s );
				CCDirector::sharedDirector()->getRunningScene()->addChild( pGui,TASK_EVNET_DIALOG_TAG, TASK_EVNET_DIALOG_TAG );

				pGui->FadeOut();

			}
		}
	}

	if( 0 == GetCurTaskCount() )
	{
		m_iEventState = EVENT_STATE_NONE;
	}

}
//--------------------------------------------------------------------------------------
//
//
void CEventControl::FreshCounter( int Id, int num, int max )
{
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		CBaseDataEvent* pBaseDataEvent =  g_pClientLoader->GetEventDataFromId(  m_TaskData[i].EventId );
		
		if( NULL != pBaseDataEvent )
		{
			if( 0 == GetEventFlag( m_TaskData[i].EventId ) )
			{
				if( pBaseDataEvent->m_SubEvent.m_iLimitId[0] == Id )
				{
					m_TaskData[i].GameCounter[0].CurCount = num;
					m_TaskData[i].GameCounter[0].MaxCount = max;

					if( m_TaskData[i].GameCounter[0].CurCount >= m_TaskData[i].GameCounter[0].MaxCount )
					{
						m_TaskData[i].GameCounter[0].CurCount = m_TaskData[i].GameCounter[0].MaxCount;
					}

				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------
//
//
void CEventControl::AddCounterNum( int num, int counterType, int Id )
{
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		CBaseDataEvent* pBaseDataEvent =  g_pClientLoader->GetEventDataFromId(  m_TaskData[i].EventId );

		if( NULL != pBaseDataEvent )
		{
			if( 0 == GetEventFlag( m_TaskData[i].EventId ) )
			{
				CBaseFunLimitData *pFunData = g_pClientLoader->GetFunLimitDataFromId( pBaseDataEvent->m_SubEvent.m_iLimitId[0] );

				if( pFunData->GetFunId() == counterType && m_TaskData[i].GameCounter[0].CounterId == Id )
				{
					m_TaskData[i].GameCounter[0].CurCount += num;

					if( m_TaskData[i].GameCounter[0].CurCount >= m_TaskData[i].GameCounter[0].MaxCount )
					{
						m_TaskData[i].GameCounter[0].CurCount = m_TaskData[i].GameCounter[0].MaxCount;
					}

				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------
//
//
int CEventControl::GetCurTaskCounterNumFromEventId( int EventId )
{
	int num = 0;
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		if( g_pPlayerEventCtrl->m_TaskData[i].EventId == EventId )
		{
			num = g_pPlayerEventCtrl->m_TaskData[i].GameCounter[0].CurCount;
			break;
		}
	}
	return num;
}
//--------------------------------------------------------------------------------------
//
//
int CEventControl::GetCurTaskCounterNum( int LimitId )
{
	int num = 0;
	for( int i = 0; i < MAX_TASK_EVNET; i++ )
	{
		CBaseDataEvent* pBaseDataEvent =  g_pClientLoader->GetEventDataFromId( g_pPlayerEventCtrl->m_TaskData[i].EventId );
		if( 0 == g_pPlayerEventCtrl->GetEventFlag( g_pPlayerEventCtrl->m_TaskData[i].EventId ) )
		{
			if( NULL != pBaseDataEvent )
			{
				if( pBaseDataEvent->m_SubEvent.m_iLimitId[0] == LimitId )
				{
					num = g_pPlayerEventCtrl->m_TaskData[i].GameCounter[0].CurCount;
					break;
				}
			}
		}
	}

	return num;
}
//--------------------------------------------------------------------------------------
//
//
void CEventControl::PushDialogEvent()
{
	//upData(0);

	//CCDirector::sharedDirector()->getRunningScene()->removeChildByTag( TASK_EVNET_DIALOG_NORMAL_TAG,true );
	//CTaskEventSubGui *pGui = new CTaskEventSubGui();
	//CCDirector::sharedDirector()->getRunningScene()->addChild( pGui,TASK_EVNET_DIALOG_NORMAL_TAG, TASK_EVNET_DIALOG_NORMAL_TAG );
	//pGui->release();

}
//-------------------------------------------------------------------------------------
//
//
bool CEventControl::CheckAllSubEventIsAchieve()
{
	for( int i = 0; i < m_TempEventPoint.m_wSubEventCount; i++ )
	{
		if( 0 == GetEventFlag( m_TempEventPoint.m_iSubEventId[i] ) )
		{
			return false;
		}
	}
	return true;
}
//--------------------------------------------------------------------------------------
//
//
bool CEventControl::CheckSubEvent( int EventId )
{	
	CBaseDataEvent* pBaseDataEvent = g_pClientLoader->GetEventDataFromId( EventId );
	if( NULL != pBaseDataEvent )
	{
		for( int i = 0; i <  pBaseDataEvent->m_SubEvent.m_wLimitCount; i ++ )
		{
			if( !HandleLimitCheck( pBaseDataEvent->m_SubEvent.m_iLimitId[i] ) )
			{
				return false;
			}
		}
	}
	return true; 
}
//--------------------------------------------------------------------------
//
//
bool CEventControl::CheckCurrentEventPointLimit()
{
	for( int i = 0; i < m_TempEventPoint.m_iPointLimitCount; i++ )
	{
		if( !HandleLimitCheck( m_TempEventPoint.m_iPointLimitArray[i] ) )  
		{
			return false;
		}

	}
	return true;
}
//-----------------------------------------------------------------------------------------
//…Ë÷√∆Ï±Í◊¥Ã¨
//
void CEventControl::SetEventFlag( int eventId, int value )
{
	if(  0 <= eventId && eventId < MAX_EVENT_FLAG )
	{
		m_iEventFlag[eventId] = value;
	}
}
//-----------------------------------------------------------------------------------------
//ªÒµ√∆Ï±Í◊¥Ã¨
//
int CEventControl::GetEventFlag( int eventId )
{
	if(  0 <= eventId && eventId < MAX_EVENT_FLAG )
	{
		return m_iEventFlag[eventId];
	}
	return 0;
}

//--------------------------------------------------------------------------------------------
//set Task Point Flag
//
void CEventControl::SetEventPointFlag( int eventId, int value  )
{
	if(  0 <= eventId + SUB_EVENT_FLAG_MAX && eventId + SUB_EVENT_FLAG_MAX < MAX_EVENT_FLAG )
	{
		m_iEventFlag[eventId + SUB_EVENT_FLAG_MAX ] = value;
	}
}
//------------------------------------------------------------------------------------------
//set Task Point Flag
//
int CEventControl::GetEventPointFlag( int eventId )
{
	if(  0 <= eventId + SUB_EVENT_FLAG_MAX && eventId + SUB_EVENT_FLAG_MAX < MAX_EVENT_FLAG )
	{
		return m_iEventFlag[eventId + SUB_EVENT_FLAG_MAX];
	}
	return 0;
}
//
void CEventControl::ClearEventFlag()
{
	memset( m_iEventFlag, 0, sizeof( int ) * MAX_EVENT_FLAG );
}