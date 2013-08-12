#pragma once

enum
{
	FIND_TYPEID_LIMIT_NAME,
	FIND_TYPEID_LIMIT_OPERATOR
};

#define MAX_LIMIT_NAME_NUM 32
#define MAX_LIMIT_PARAM_NUM 8

class CBaseFunLimitData
{
	
public:
	CBaseFunLimitData();
	~CBaseFunLimitData();
	
	unsigned short GetFunId(){ return m_iFunId; }
	unsigned short GetOperator(){ return m_iOperator; }
	unsigned short FindIdFormName( int type, const char* ch);
	unsigned short GetID(){ return m_iID; }
	
	unsigned short m_iFunId;
	unsigned short m_iOperator;
	unsigned short m_arrParam[MAX_LIMIT_PARAM_NUM];
	unsigned short m_iID;
	
};