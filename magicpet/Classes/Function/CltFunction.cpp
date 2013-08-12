/*
 *  CltFunction.cpp
 *  game
 *
 *  Created by huyan on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CltFunction.h"
#include "../loader/CClientLoader.h"
#include "../Sprite/CPet.h"
#include "../Pushui/CEventControl.h"
#include "../sql/CSqliteManager.h"
#include "../Fighting/CFightGroundLayer.h"


/**********************************************************************************************************/
//
//LIMITE FUNCTION
//
/**********************************************************************************************************/

typedef bool(*LimitFuntion)( CBaseFunLimitData *pLimitData );


LimitFuntion   m_pLimitFunctionArray[] = 
{
	&HY_FunLimit::Limit_Func_Default,
	&HY_FunLimit::checkpetlevel,
	&HY_FunLimit::checkpetAechievePoint,
	&HY_FunLimit::checkpetmoney,
	&HY_FunLimit::checkpetsex,
	&HY_FunLimit::checkpetid,
	&HY_FunLimit::checkitemcount,
	&HY_FunLimit::checkfeeditemcount,
	&HY_FunLimit::checkplayermallmoney,
	&HY_FunLimit::checkskilllevel,
	&HY_FunLimit::checkbattlewincount,
	&HY_FunLimit::checkbattlelosecount,
	&HY_FunLimit::checkbattlecount,
	&HY_FunLimit::checkgameruntime,
    &HY_FunLimit::checkgamehitwood,
    &HY_FunLimit::checkdotakill
};
//------------------------------------------
//
//
bool HandleLimitCheck( int Index  )
{ 
	CBaseFunLimitData *pData = g_pClientLoader->GetFunLimitDataFromId( Index );
	if( NULL == pData )
	{
		return (m_pLimitFunctionArray[0])( NULL );
	}
	int Funid = pData->GetFunId();
	return (m_pLimitFunctionArray[Funid])( pData );
}
//-----------------------------------------
//
//
bool HY_FunLimit::Limit_Func_Default( CBaseFunLimitData *pLimitData )
{
	return false;
}
//------------------------------------------------------
//城堡等级判断
//
bool HY_FunLimit::checkpetlevel( CBaseFunLimitData *pLimitData )
{
	if( NULL != pLimitData  )
	{
		int curLevel = g_pPetDataBlock->petlevel;

		switch( pLimitData->GetOperator() )
		{
			case LIMIT_OPERATOR_GREATERTHAN:
				if( curLevel >  pLimitData->m_arrParam[0] )
				{
					return true;
				}
				break;
			case LIMIT_OPERATOR_LESSTHAN:
				if( curLevel < pLimitData->m_arrParam[0] )
				{
					return true;
				}
				break;
			case LIMIT_OPERATOR_EQUEAL:
				if( curLevel == pLimitData->m_arrParam[0] )
				{
					return true;
				}
				break;
			case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
				if( curLevel >= pLimitData->m_arrParam[0] )
				{
					return true;
				}
				break;
			default:
				break;
		}
	}
	return false;
}
//------------------------------------------------------
//雇员等级判断
//
bool HY_FunLimit::checkpetAechievePoint( CBaseFunLimitData *pLimitData )
{
	int data;

	data = g_pPlayerEventCtrl->m_iEventFlag[pLimitData->m_arrParam[0]];

	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//-------------------------------------------------------
//horror值判断
//
bool HY_FunLimit::checkpetmoney( CBaseFunLimitData *pLimitData )
{
	int data;
	data = g_pPetDataBlock->petmoney;

	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//---------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkpetsex( CBaseFunLimitData *pLimitData )
{
	int data;
	data = g_pPetDataBlock->petsex;

	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkpetid( CBaseFunLimitData *pLimitData  )
{
	if( g_pPetDataBlock->petid >= pLimitData->m_arrParam[0] && g_pPetDataBlock->petid <= pLimitData->m_arrParam[1] )
	{
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkitemcount( CBaseFunLimitData *pLimitData  )
{
	SFeedDataItem* pFeedDataItem =  g_pPetDataBlock->GetSaveItemFromId( pLimitData->m_arrParam[1] );
	if( NULL == pFeedDataItem )
	{
		return false;
	}

	int data = pFeedDataItem->itemCount;

	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//------------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkfeeditemcount( CBaseFunLimitData *pLimitData )
{
	int data = 0;
	for( int i = 0; i < MAX_ITEM_HAVE_DFINE; i++ )
	{
		if( g_pPetDataBlock->recordsave.Item[i].itemId ==  pLimitData->m_arrParam[1] )
		{
			data = g_pPetDataBlock->recordsave.Item[i].itemCount;
		}
	}

	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//----------------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkplayermallmoney( CBaseFunLimitData *pLimitData )
{
	int data = g_pPetDataBlock->playershoppingmallmoney;
	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkskilllevel(  CBaseFunLimitData *pLimitData  )
{
	int data = g_pPetDataBlock->skilllevel.skillsLevel[pLimitData->m_arrParam[1] - 1];
	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkbattlewincount( CBaseFunLimitData *pLimitData )
{
	//int data = g_pPlayerEventCtrl->GetCurTaskCounterNum( pLimitData->GetID() );
	int data = g_pPetDataBlock->recordsave.winBattleCount;
	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkbattlelosecount( CBaseFunLimitData *pLimitData )
{
	//int data = g_pPlayerEventCtrl->GetCurTaskCounterNum( pLimitData->GetID() );
	int data = g_pPetDataBlock->recordsave.loseBattleCount;
	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkbattlecount(  CBaseFunLimitData *pLimitData  )
{
	//int data = g_pPlayerEventCtrl->GetCurTaskCounterNum( pLimitData->GetID() );
	int data = g_pPetDataBlock->recordsave.winBattleCount + g_pPetDataBlock->recordsave.loseBattleCount;
	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkgameruntime( CBaseFunLimitData *pLimitData )
{
	int data = g_pPetDataBlock->gametime.gameRunTime;
	switch( pLimitData->GetOperator() )
	{
	case LIMIT_OPERATOR_GREATERTHAN:
		if( data >  pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_LESSTHAN:
		if( data < pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_EQUEAL:
		if( data == pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
		if( data >= pLimitData->m_arrParam[0] )
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}
//------------------------------------------------------------------------------------
//
//
bool HY_FunLimit::checkgamehitwood( CBaseFunLimitData *pLimitData )
{
    int data = g_pPetDataBlock->recordsave.HitWoodCount;
	switch( pLimitData->GetOperator() )
	{
        case LIMIT_OPERATOR_GREATERTHAN:
            if( data >  pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        case LIMIT_OPERATOR_LESSTHAN:
            if( data < pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        case LIMIT_OPERATOR_EQUEAL:
            if( data == pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
            if( data >= pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        default:
            break;
	}
	return false;
}

bool HY_FunLimit::checkdotakill( CBaseFunLimitData *pLimitData )
{
    int data = CFightGroundLayer::s_iWincount;
    
	switch( pLimitData->GetOperator() )
	{
        case LIMIT_OPERATOR_GREATERTHAN:
            if( data >  pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        case LIMIT_OPERATOR_LESSTHAN:
            if( data < pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        case LIMIT_OPERATOR_EQUEAL:
            if( data == pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        case LIMIT_OPERATOR_GREATERTHAN_EQUEAL:
            if( data >= pLimitData->m_arrParam[0] )
            {
                return true;
            }
            break;
        default:
            break;
	}
	return false;
}
/**********************************************************************************************************/
//
//RUN FUNCTION
//
/**********************************************************************************************************/
typedef void(*RunFuntion)( CBaseFunRunData *pRunData );

RunFuntion   m_pRunFunctionArray[] = 
{
	&HY_FunRun::RUN_Func_Default,
	&HY_FunRun::runexp,
	&HY_FunRun::runskillexp,
	&HY_FunRun::runtitle,
	&HY_FunRun::runpet,
	&HY_FunRun::runeventflag,
	&HY_FunRun::runskill,
    &HY_FunRun::runmoney,
    &HY_FunRun::runplayermoney
    
};
//-------------------------------------------------------------
//
//
void HandleExecRunFunc( int Index )
{
	CBaseFunRunData *pData = g_pClientLoader->GetFunRunDataFromId( Index );
	if( NULL == pData )
	{
		(m_pRunFunctionArray[0])( NULL );
		return;
	}
	(m_pRunFunctionArray[pData->m_iFunId])( pData );
}
//-------------------------------------------------------------
//
// 
void HY_FunRun::RUN_Func_Default( CBaseFunRunData *pRunData )
{
    printf("run nothing!");
}

//--------------------------------------------------------------
//
//
void HY_FunRun::runexp( CBaseFunRunData *pRunData)
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();

	switch( pRunData->GetOperator() )
	{
	case RUN_OPERATOR_ADD:
		g_pPetDataBlock->addPetExp( pRunData->m_arrParam[0], ccp( s.width / 2, s.height / 2) );
		break;
	case RUN_OPERATOR_DEC:
		g_pPetDataBlock->petexp -= pRunData->m_arrParam[0];
		break;
	}
}
//-------------------------------------------------------------------------------------
//            
//
void HY_FunRun::runskillexp( CBaseFunRunData *pRunData )
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	switch( pRunData->GetOperator() )
	{
	case RUN_OPERATOR_ADD:
		g_pPetDataBlock->addSkillExp( pRunData->m_arrParam[0], ccp( s.width / 2, s.height / 2) );
		break;
	case RUN_OPERATOR_DEC:
		g_pPetDataBlock->petskillexppond -= pRunData->m_arrParam[0];
		break;
	}
}
//-------------------------------------------------------------------------------------
//            
//
void HY_FunRun::runtitle( CBaseFunRunData *pRunData )
{
	
}
//-------------------------------------------------------------------------------------
//            
//
void HY_FunRun::runpet( CBaseFunRunData *pRunData )
{
	char buffer[32];
	sprintf(buffer,"%d,%d", pRunData->m_arrParam[0], pRunData->m_arrParam[1] );
	g_pPetDataBlock->petturning[0] = pRunData->m_arrParam[0];
	g_pPetDataBlock->petturning[1] = pRunData->m_arrParam[1];
}
//---------------------------------------------------------------------------------
//
//
void HY_FunRun::runeventflag( CBaseFunRunData *pRunData )
{
	g_pPlayerEventCtrl->SetEventFlag( pRunData->m_arrParam[0],pRunData->m_arrParam[1] );
}
//------------------------------------------------------------------------------------
//
//
void HY_FunRun::runskill( CBaseFunRunData *pRunData )
{
	g_pPetDataBlock->skilllevel.skillsLevel[pRunData->m_arrParam[0] - 1] = 1;
}
//-----------------------------------------------------------------
//
//
void HY_FunRun::runmoney( CBaseFunRunData *pRunData )
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
    switch( pRunData->GetOperator() )
	{
        case RUN_OPERATOR_ADD:
            //g_pPetDataBlock->petmoney += pRunData->m_arrParam[0];
			g_pPetDataBlock->addPetMoney( pRunData->m_arrParam[0], ccp( s.width / 2, s.height / 2) );
            break;
        case RUN_OPERATOR_DEC:
            g_pPetDataBlock->petmoney -= pRunData->m_arrParam[0];
            break;
	}
    
}
//-----------------------------------------------------------------
//
//
void HY_FunRun::runplayermoney( CBaseFunRunData *pRunData )
{
	CCSize s = CCDirector::sharedDirector()->getWinSize();
    switch( pRunData->GetOperator() )
	{
        case RUN_OPERATOR_ADD:
            //g_pPetDataBlock->playershoppingmallmoney += pRunData->m_arrParam[0];
			g_pPetDataBlock->addPlayerMoney( pRunData->m_arrParam[0], ccp( s.width / 2, s.height / 2) );
            break;
        case RUN_OPERATOR_DEC:
            g_pPetDataBlock->playershoppingmallmoney -= pRunData->m_arrParam[0];
            break;
	}
}
