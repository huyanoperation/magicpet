/*
 *  CltFunction.h
 *  game
 *
 *  Created by huyan on 1/13/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "../loader/CBaseFunRunData.h"
#include "../loader/CBaseFunLimitData.h"

enum
{
	LIMIT_OPERATOR_GREATERTHAN = 1,
	LIMIT_OPERATOR_LESSTHAN,
	LIMIT_OPERATOR_EQUEAL,
	LIMIT_OPERATOR_GREATERTHAN_EQUEAL
};

enum
{
	RUN_OPERATOR_ADD  = 1,
	RUN_OPERATOR_DEC,
	RUN_OPERATOR_DOC,
	RUN_OPERATOR_PEC
};

bool					HandleLimitCheck( int funIndex );

//=====================================================================================
//
// function limite
//
//=====================================================================================
namespace HY_FunLimit
{
	bool	      Limit_Func_Default( CBaseFunLimitData *pLimitData );																	
	bool		  checkpetlevel( CBaseFunLimitData *pLimitData );
	bool		  checkpetAechievePoint( CBaseFunLimitData *pLimitData );
	bool		  checkpetmoney( CBaseFunLimitData *pLimitData );
	bool		  checkpetsex( CBaseFunLimitData *pLimitData  );
	bool		  checkpetid( CBaseFunLimitData *pLimitData );
	bool		  checkitemcount( CBaseFunLimitData *pLimitData );
	bool		  checkfeeditemcount( CBaseFunLimitData *pLimitData );
	bool		  checkplayermallmoney( CBaseFunLimitData *pLimitData );
	bool		  checkskilllevel( CBaseFunLimitData *pLimitData );
	bool		  checkbattlewincount( CBaseFunLimitData *pLimitData );
	bool		  checkbattlelosecount( CBaseFunLimitData *pLimitData );
	bool		  checkbattlecount( CBaseFunLimitData *pLimitData );
	bool		  checkgameruntime( CBaseFunLimitData *pLimitData );
    bool          checkgamehitwood( CBaseFunLimitData *pLimitData );
    bool          checkdotakill( CBaseFunLimitData *pLimitData );
};

//=====================================================================================
//
// function Run
//
//=====================================================================================

void			 HandleExecRunFunc( int index );

namespace HY_FunRun
{
	void	      RUN_Func_Default( CBaseFunRunData *pRunData);																	
	void		  runexp( CBaseFunRunData *pRunData);
	void		  runskillexp( CBaseFunRunData *pLimitData);
	void		  runtitle( CBaseFunRunData *pRunData );
	void		  runpet( CBaseFunRunData *pRunData );
	void		  runeventflag( CBaseFunRunData *pRunData );
	void		  runskill( CBaseFunRunData *pRunData );
    void          runmoney( CBaseFunRunData *pRunData );
    void          runplayermoney( CBaseFunRunData *pRunData );

};
