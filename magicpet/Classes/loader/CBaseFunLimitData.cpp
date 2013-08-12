#include "CBaseFunLimitData.h"
#include "CCString.h"


static const char* s_LimitFunIndex[] =
{
	"default",
	"checkpetlevel",
	"checkpetAechievePoint",
	"checkpetmoney",
	"checkpetsex",
	"checkpetid",
	"checkitemcount",
	"checkfeeditemcount",
	"checkplayermallmoney",
	"checkskilllevel",
	"checkbattlewincount",
	"checkbattlelosecount",
	"checkbattlecount",
	"checkgameruntime",
    "checkgamehitwood",
    "checkdotakill"

};

static const char* s_LimitFunOperator[] =
{
	"default",
	">",
	"<",
	"=",
	">="
};

CBaseFunLimitData::CBaseFunLimitData()
{

}
//-----------------------------------------------------
//
//
CBaseFunLimitData::~CBaseFunLimitData()
{

}

//----------------------------------------------------------------
//
//	
unsigned short CBaseFunLimitData::FindIdFormName( int type, const char* ch)
{
	if( FIND_TYPEID_LIMIT_NAME == type )
	{
		for( int i = 0; i < MAX_LIMIT_NAME_NUM; i++ )
		{
 			 if( 0 == strcmp( ch, s_LimitFunIndex[i] ) )
			 {
				return i;
			 }
		}
	}
	else if( FIND_TYPEID_LIMIT_OPERATOR == type )
	{
		for( int i = 0; i < MAX_LIMIT_NAME_NUM; i++ )
		{	
			 if( 0 == strcmp( ch, s_LimitFunOperator[i] ) )
			 {
				return i;
			 }
		}
	}
	return 0;
}