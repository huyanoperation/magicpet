/*****************************************************************************/
//File name:	CClientLoader
//Author: 		huyan
//Description:	”Œœ∑≤ﬂªÆ ˝æ›∂¡»°°¢
/******************************************************************************/
#include "CClientLoader.h"
#include "cocos2d.h"
#include "../CGameText.h"
#include "../md5.h"
using namespace cocos2d;

CClientLoader *g_pClientLoader = NULL;


//////////////////////////////////////////////////////////////////////////

const char* jsonFileBaseDataSkill = "data/loadRes/skillList.json";
const char* jsonFileBaseDataAnimation = "data/loadRes/petanimation.json";
const char* jsonFileBaseDataPet = "data/loadRes/basepet.json";
const char* jsonFileBaseDataItem = "data/loadRes/baseitem.json";
const char* jsonFileBaseDataExp = "data/loadRes/petexp.json";
const char* jsonFileSubEvent = "data/loadRes/subevent.json";
const char* jsonFileEventPoint = "data/loadRes/eventpoint.json";
const char* jsonFileFunctionRun = "data/loadRes/eventrun.json";
const char* jsonFileFunctionLimit = "data/loadRes/eventlimit.json";
const char* jsonFilescore		  = "data/loadRes/minigame3_score.json";

CClientLoader::CClientLoader()
{
    m_bIsCheat = false;
}
//----------------------------------------------------------------------------------
//
//
CClientLoader::~CClientLoader()
{

}
//------------------------------------------------------------------------------------
//
//
void CClientLoader::Init()
{
	LoadBaseDataPet();
	LoadBaseDataSkill();
    LoadBaseDataAnimation();
	LoadBaseDataItem();
    LoadBaseDataExp();
	LoadBaseEvent();
	LoadBaseDataRun();
	LoadBaseDataLimit();
	LoadBaseDataScore();
}
//----------------------------------------------------------------------------------------
//
//
bool CClientLoader::isCheat( string strBuffer, string ReadBuffer )
{
	MD5 md5( strBuffer );
	string strMd5 = md5.toString();
	string strDefineMd5 = ReadBuffer;
	if( strMd5 != strDefineMd5 )
	{
        m_bIsCheat = true;
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------
//
//
void CClientLoader::LoadBaseDataPet()
{
	std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileBaseDataPet)); 
	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer, basepet_json[0] ) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileBaseDataPet  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}

	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}
	Json::Value root;
	if( parseAndSaveValueTree( strBuffer, "", root ) )
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}

	Json::Value Root;

	m_vBaseDataPet.clear();

	for( int i = 0;; i++ )
	{
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["basepet"][buffer];
		if( Root.isNull() )
		{
			break;
		}
		CBaseDataPet BaseData;
		BaseData.m_iID				= Root["id"].asInt();
		BaseData.petname			= Root["petname"].asString();
		BaseData.petsex				= Root["petsex"].asInt();
		BaseData.pettitle			= Root["pettitle"].asInt();
		BaseData.petlevel			= Root["petlevel"].asInt();
		BaseData.petexp				= Root["petexp"].asInt();
		BaseData.initstr			= Root["initstr"].asInt();
		BaseData.upstr				= Root["upstr"].asDouble();
		BaseData.initint			= Root["initint"].asInt();
		BaseData.upint				= Root["upint"].asDouble();
		BaseData.initdex			= Root["initdex"].asInt();
		BaseData.updex				= Root["updex"].asDouble();
		BaseData.initskill			= Root["initskill"].asInt();
		BaseData.upskill			= Root["upskill"].asDouble();
		BaseData.inithp				= Root["inithp"].asInt();
		BaseData.initmp				= Root["initmp"].asInt();
		BaseData.straptitude		= Root["straptitude"].asInt();
		BaseData.intaptitude		= Root["intaptitude"].asInt();
		BaseData.dexaptitude		= Root["dexaptitude"].asInt();
		BaseData.skillaptitude		= Root["skillaptitude"].asInt();
		BaseData.maxstaminapoint	= Root["maxstaminapoint"].asInt();
		BaseData.animationid		= Root["animationid"].asInt();
		BaseData.petshow			= Root["petshow"].asString();
        BaseData.evolutionpist      = Root["evolutionpist"].asString();
		BaseData.evolution_button	= Root["evolution_button"].asString();
        BaseData.attack_sound       = Root["attack_sound"].asString();
        if( Root["animation"].isArray() )
        {
            int count         = Root["animation"].size();
            for( int j = 0; j < count; j++ )
			{
				BaseData.animation[j] = Root["animation"][j].asInt();
			}
        }
		if( Root["skillfield"].isArray() )
		{
			int count = Root["skillfield"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.skillfield[j] = Root["skillfield"][j].asInt();
			}
		}
		if( Root["skillfield_defense"].isArray() )
		{
			int count = Root["skillfield_defense"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.skillfield_defense[j] = Root["skillfield_defense"][j].asInt();
			}
		}
		if( Root["skillfield_3"].isArray() )
		{
			int count = Root["skillfield_3"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.skillfield_3[j] = Root["skillfield_3"][j].asInt();
			}
		}
		
		if( Root["rgb"].isArray() )
		{
			int count = Root["rgb"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.rgb[j] = Root["rgb"][j].asInt();
			}
		}

		m_vBaseDataPet.push_back( BaseData );
	}
}
//----------------------------------------------------------------------------------
//
//
CBaseDataPet *CClientLoader::GetBaseDataPetFromId( int ID )
{
	for( int i = 0; i < m_vBaseDataPet.size(); i++ )
	{
		if( ID == m_vBaseDataPet[i].m_iID )
		{
			return &m_vBaseDataPet[i];
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------------------
//
void CClientLoader::LoadBaseDataSkill()
{
	std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileBaseDataSkill)); 
	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  skillList_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileBaseDataSkill  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}

	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}
	Json::Value root;
	if( parseAndSaveValueTree( strBuffer, "", root ) )
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}

	Json::Value Root;

	m_vBaseDataSkill.clear();

	for( int i = 0;; i++ )
	{
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["skillList"][buffer];
		if( Root.isNull() )
		{
			break;
		}
		CBaseDataSkill BaseData;
		BaseData.m_iID					= Root["id"].asInt();
		BaseData.attack_frame			= Root["attack_frame"].asInt();
		BaseData.damage					= Root["damage"].asInt();
		BaseData.skillmp				= Root["skillmp"].asInt();
		BaseData.improveskilllevelExp	= Root["improveskilllevelExp"].asInt();
		BaseData.animid					= Root["animid"].asInt();
		BaseData.plist					= Root["plist"].asString(); 
		BaseData.skillicon				= Root["skill_icon"].asString();
        BaseData.skill_sound            = Root["skill_sound"].asString();
		m_vBaseDataSkill.push_back( BaseData ); 
	}
}
//---------------------------------------------------------------------------
//
//
CBaseDataSkill *CClientLoader::GetBaseDataSkillFromId( int ID )
{
	for( int i = 0; i < m_vBaseDataSkill.size(); i++ )
	{
		if( ID == m_vBaseDataSkill[i].m_iID )
		{
			return &m_vBaseDataSkill[i];
		}
	}
	return NULL;
}
//------------------------------------------------------------------
//
//
void CClientLoader::LoadBaseDataAnimation()
{
    std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileBaseDataAnimation)); 
	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  petanimation_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileBaseDataAnimation  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}
    
    if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}
	Json::Value root;
	if( parseAndSaveValueTree( strBuffer, "", root ) )
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}
    
    Json::Value Root;
    
	m_vBaseDataAnimation.clear();

	for( int i = 0;; i++ )
	{
		char buffer[64];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["petanimation"][buffer];
		if( Root.isNull() )
		{
			break;
		}
		CBaseDataAnimation BaseData;
		BaseData.m_iID		= Root["id"].asInt();
        BaseData.m_strPlist = Root["plist"].asString();
		BaseData.m_iPlistType = Root["pisttype"].asInt();
        
		if( Root["stand"].isArray() )
		{
			int count = Root["stand"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_stand[j]= Root["stand"][j].asInt();
			}
		}
        if( Root["move"].isArray() )
		{
			int count = Root["move"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_move[j]= Root["move"][j].asInt();
			}
		}
        if( Root["attack"].isArray() )
		{
			int count = Root["attack"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_attack[j]= Root["attack"][j].asInt();
			}
		}
        if( Root["defense"].isArray() )
		{
			int count = Root["defense"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_defense[j]= Root["defense"][j].asInt();
			}
		}
        if( Root["injured"].isArray() )
		{
			int count = Root["injured"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_injured[j]= Root["injured"][j].asInt();
			}
		}
        if( Root["win"].isArray() )
		{
			int count = Root["win"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_win[j]= Root["win"][j].asInt();
			}
		}
        if( Root["lost"].isArray() )
		{
			int count = Root["lost"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_lost[j]= Root["lost"][j].asInt();
			}
		}
        if( Root["skill"].isArray() )
		{
			int count = Root["skill"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_skill[j]= Root["skill"][j].asInt();
			}
		}
        if( Root["specialaction_1"].isArray() )
		{
			int count = Root["specialaction_1"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_specialaction_1[j]= Root["specialaction_1"][j].asInt();
			}
		}
        if( Root["specialaction_2"].isArray() )
		{
			int count = Root["specialaction_2"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_specialaction_2[j]= Root["specialaction_2"][j].asInt();
			}
		}
        if( Root["specialaction_3"].isArray() )
		{
			int count = Root["specialaction_3"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_specialaction_3[j]= Root["specialaction_3"][j].asInt();
			}
		}
        
		m_vBaseDataAnimation.push_back( BaseData );
	}
}
//--------------------------------------------------------------
//
//
CBaseDataAnimation *CClientLoader::GetBaseDataAnimationFromId( int ID )
{
    for( int i = 0; i < m_vBaseDataAnimation.size(); i++ )
	{
		if( ID == m_vBaseDataAnimation[i].m_iID )
		{
			return &m_vBaseDataAnimation[i];
		}
	}
	return NULL;
}
//------------------------------------------------------------------
//
//
void CClientLoader::LoadBaseDataItem()
{
	std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileBaseDataItem)); 
	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  baseitem_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileBaseDataItem  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}

	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}
	Json::Value root;
	if( parseAndSaveValueTree( strBuffer, "", root ) )
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}

	Json::Value Root;

	m_vBaseDataItem.clear();

	for( int i = 0;; i++ )
	{
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["baseitem"][buffer];
		if( Root.isNull() )
		{
			break;
		}
		CBaseDataItem BaseData;
		BaseData.itemid			= Root["itemid"].asInt();
		BaseData.itemindex		= Root["itemindex"].asInt();	
		BaseData.itemkind		= Root["itemkind"].asInt();	
		BaseData.itemcd			= Root["itemcd"].asDouble();		
		BaseData.itemtype		= Root["itemtype"].asInt();	
		BaseData.itemexp		= Root["itemexp"].asInt();	
		BaseData.itemskillexp	= Root["itemskillexp"].asInt();	
		BaseData.itemmoney		= Root["itemmoney"].asInt(); 	
		BaseData.itemlimit		= Root["itemlimit"].asInt();	
		BaseData.itemfunction	= Root["itemfunction"].asInt();
		BaseData.itemname		= Root["itemname"].asString();
		BaseData.itemicon		= Root["itemicon"].asString();	
		BaseData.itemSound		= Root["sound"].asString();
		m_vBaseDataItem.push_back( BaseData );
	}
}
//---------------------------------------------------------------------
//
//
void CClientLoader::LoadBaseDataExp()
{
    std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileBaseDataExp)); 
	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  petexp_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileBaseDataExp  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}

    
	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}
	Json::Value root;
	if( parseAndSaveValueTree( strBuffer, "", root ) )
	{ 
		CCAssert( NULL,"Can not found json file!" );
		return;
	}
    
    Json::Value Root;
    m_vBaseDataExp.clear();
    
    for( int i = 0;; i++ )
	{
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["petexp"][buffer];
		if( Root.isNull() )
		{
			break;
		}
		CBaseDataExp BaseData;
		BaseData.level			= Root["level"].asInt();
		BaseData.exp            = Root["exp"].asInt();	

		m_vBaseDataExp.push_back( BaseData );
	}
    
}
//------------------------------------------------------------
//
//
CBaseDataExp *CClientLoader::GetBaseDataExpFromLevel( int level )
{
    for( int i = 0; i < m_vBaseDataExp.size(); i++ )
	{
		if( level == m_vBaseDataExp[i].level )
		{
			return &m_vBaseDataExp[i];
		}
	}
    return NULL;
}
//----------------------------------------------------------------------
//
//
CBaseDataItem *CClientLoader::GetBaseDataItemFromID( int ID )
{
	for( int i = 0; i < m_vBaseDataItem.size(); i++ )
	{
		if( ID == m_vBaseDataItem[i].itemid )
		{
			return &m_vBaseDataItem[i];
		}
	}
	return NULL;
}
//--------------------------------------------------------
//
//
CBaseDataItem *CClientLoader::GetBaseDataItemIndex( int index )
{
	for( int i = 0; i < m_vBaseDataItem.size(); i++ )
	{
		if( index == m_vBaseDataItem[i].itemindex )
		{
			return &m_vBaseDataItem[i];
		}
	}
	return NULL;
}
//---------------------------------------------------------------
//
//
void CClientLoader::LoadBaseEvent()
{
	std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileEventPoint));
	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  eventpoint_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileEventPoint  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}


	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL, "Can not found json file!" );
		return;
	}

	Json::Value root;
	if( parseAndSaveValueTree( strBuffer,"", root ) )
	{ 
		CCAssert( NULL, "read jsonfile is not success!" );
		return;
	}


	vector< SEventPoint > vEventPoint;

	Json::Value Root; 

	for( int i = 0;;i++ )
	{
		SEventPoint EventPoint;
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["eventpoint"][buffer];
		if( Root.isNull() )
		{
			break;
		}
		EventPoint.m_wPointId = Root["id"].asInt();
        //EventPoint.m_w91Id    = Root["id_91"].asInt();

		if( Root["subevent"].isArray() )
		{
			Json::UInt index = 0;
			EventPoint.m_wSubEventCount = Root["subevent"].size();
			for( int j = 0; j < EventPoint.m_wSubEventCount; j++ )
			{
				EventPoint.m_iSubEventId[j] = Root["subevent"][index++].asInt();
			}
		}

		if( Root["pointlimit"].isArray() )
		{
			Json::UInt index = 0;
			EventPoint.m_iPointLimitCount = Root["pointlimit"].size();
			for( int j = 0; j < EventPoint.m_iPointLimitCount; j++ )
			{
				EventPoint.m_iPointLimitArray[j] = Root["pointlimit"][index++].asInt();
			}
		}

		if( Root["pointrun"].isArray() )  
		{
			Json::UInt index = 0;
			EventPoint.m_iPointRunCount = Root["pointrun"].size();
			for( int j = 0; j < EventPoint.m_iPointRunCount; j++ )
			{
				EventPoint.m_iPointRunArray[j] = Root["pointrun"][index++].asInt();
			}
		}

		vEventPoint.push_back( EventPoint );
	}


	std::string fullpath2(CCFileUtils::fullPathFromRelativePath(jsonFileSubEvent));
	strBuffer = readInputTestFile( fullpath2.c_str() );

	if( isCheat( strBuffer,  subevent_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileSubEvent  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}


	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL, "Can not found json file!" );
		return;
	}

	Json::Value root2;
	if( parseAndSaveValueTree( strBuffer,"", root2 ) )
	{ 
		CCAssert( NULL, "read jsonfile is not success!" );
		return;
	}


	m_vBaseDataEvent.clear();

	Json::Value Root2; 
	for( int i = 0;;i++ )
	{
		SSubEvent SubEvent;
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root2 = root2["subevent"][buffer];
		if( Root2.isNull() )
		{
			break;
		}
		SubEvent.m_wId = Root2["id"].asInt();

		SubEvent.m_wLimitCount = Root2["eventlimit"].size();
		Json::UInt index = 0;
		for( int j = 0; j < SubEvent.m_wLimitCount; j++ )
		{
			SubEvent.m_iLimitId[j] = Root2["eventlimit"][index++].asInt();
		}

		SubEvent.m_wEventRunCount = Root2["eventrun"].size();
		SubEvent.m_wSubEventLogID = Root2["eventlog"].asInt();
		SubEvent.m_wSubEventNameID = Root2["eventname"].asInt();
		SubEvent.eventicon = Root2["eventicon"].asString();

		index = 0;
		for( int j = 0; j < SubEvent.m_wEventRunCount; j++ )
		{
			SubEvent.m_iFunctionRun[j] = Root2["eventrun"][index++].asInt();
		}

		CBaseDataEvent BaseDataEvent;

		BaseDataEvent.m_SubEvent =  SubEvent;
		for( int j = 0; j < vEventPoint.size(); j++ )
		{
			if( vEventPoint[j].m_wPointId == SubEvent.m_wId )
			{
				BaseDataEvent.m_EventPointInfo = vEventPoint[j];
			}
		}
		m_vBaseDataEvent.push_back( BaseDataEvent );

	}

}
//--------------------------------------------------------------
//
//
CBaseDataEvent* CClientLoader::GetEventDataFromId( int ID )
{
	for( int i = 0; i < m_vBaseDataEvent.size(); i++ )
	{
		if( ID == m_vBaseDataEvent[i].m_SubEvent.m_wId )
		{
			return &m_vBaseDataEvent[i];
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
//
//
void CClientLoader::LoadBaseDataRun()
{
	std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileFunctionRun));
	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  eventrun_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileFunctionRun  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}

	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL, "Can not found json file!" );
		return;
	}

	Json::Value root;
	if( parseAndSaveValueTree( strBuffer,"", root ) )
	{ 
		CCAssert( NULL, "read jsonfile is not success!" );
		return;
	}

	Json::Value Root;
	m_vBaseFunRunData.clear();

	for( int i = 0;; i++ )
	{
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["eventrun"][buffer];
		if( Root.isNull() )
		{
			break;
		} 
		CBaseFunRunData BaseData; 
		BaseData.m_iID			= Root["id"].asInt();
		BaseData.m_iFunId		= BaseData.FindIdFormName( FIND_TYPEID_FUN_NAME,Root["functionname"].asString().c_str() );
		BaseData.m_iOperator	= BaseData.FindIdFormName( FIND_TYPEID_FUN_OPERATOR,Root["operator"].asString().c_str() );
		BaseData.m_strIcon		= BaseData.m_strIcon = Root["runicon"].asString();

		if( Root["param"].isArray() )
		{
			int count = Root["param"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_arrParam[j]	=  Root["param"][j].asInt();
			}
		}

		m_vBaseFunRunData.push_back( BaseData );
	} 
}
//-----------------------------------------------------------------------------------
//
//
CBaseFunRunData* CClientLoader::GetFunRunDataFromId( int ID )
{
	for( int i = 0; i < m_vBaseFunRunData.size(); i++ )
	{
		if( ID == m_vBaseFunRunData[i].m_iID )
		{
			return &m_vBaseFunRunData[i];
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------------------
//
//
void CClientLoader::LoadBaseDataLimit()
{
	std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFileFunctionLimit));

	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  eventlimit_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFileFunctionLimit  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}

	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL, "Can not found json file!" );
		return;
	}

	Json::Value root;
	if( parseAndSaveValueTree( strBuffer,"", root ) )
	{ 
		CCAssert( NULL, "read jsonfile is not success!" );
		return;
	}

	Json::Value Root;
	m_vBaseFunLimitData.clear();

	for( int i = 0;; i++ ) 
	{
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["eventlimit"][buffer]; 
		if( Root.isNull() )
		{
			break; 
		} 
		CBaseFunLimitData BaseData;

		BaseData.m_iID		= Root["id"].asInt();
		BaseData.m_iFunId	= BaseData.FindIdFormName( FIND_TYPEID_LIMIT_NAME, Root["functionname"].asString().c_str() );
		BaseData.m_iOperator= BaseData.FindIdFormName( FIND_TYPEID_LIMIT_OPERATOR, Root["operator"].asString().c_str() );

		if( Root["param"].isArray() )
		{
			int count = Root["param"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.m_arrParam[j]	=  Root["param"][j].asInt();
			}
		}
		m_vBaseFunLimitData.push_back( BaseData );
	} 
}
//-------------------------------------------------------------------------------------
//
//
CBaseFunLimitData* CClientLoader::GetFunLimitDataFromId( int ID )
{
	for( int i = 0; i < m_vBaseFunLimitData.size(); i++ )
	{
		if( ID == m_vBaseFunLimitData[i].m_iID )
		{
			return &m_vBaseFunLimitData[i];
		}
	}
	return NULL;
}
//--------------------------------------------------------------
//
//
void CClientLoader::LoadBaseDataScore()
{
	std::string fullpath(CCFileUtils::fullPathFromRelativePath(jsonFilescore));

	std::string strBuffer = readInputTestFile( fullpath.c_str() );

	if( isCheat( strBuffer,  minigame3_score_json[0]) )
	{
		char buffer[512];
		sprintf( buffer, the_message_cheat_json[g_iCurLanguageIndex], jsonFilescore  );
		CCMessageBox( buffer, the_message_alert[g_iCurLanguageIndex] );
	}

	if( strBuffer.empty() ) 
	{ 
		CCAssert( NULL, "Can not found json file!" );
		return;
	}

	Json::Value root;
	if( parseAndSaveValueTree( strBuffer,"", root ) )
	{ 
		CCAssert( NULL, "read jsonfile is not success!" );
		return;
	}

	Json::Value Root;
	m_vBaseDataScore.clear();

	for( int i = 0;; i++ ) 
	{
		char buffer[32];
		sprintf( buffer, "key%d", i + 1 );
		Root = root["minigame3_score"][buffer]; 
		if( Root.isNull() )
		{
			break; 
		} 
		CBaseDataScore BaseData;
		BaseData.m_iID			= Root["id"].asInt();
		if( Root["param"].isArray() )
		{
			int count = Root["index"].size();
			for( int j = 0; j < count; j++ )
			{
				BaseData.index[j]=  Root["index"][j].asInt();
			}
		}

		BaseData.score = Root["score"].asInt();

		m_vBaseDataScore.push_back( BaseData );
	} 
}

CBaseDataScore* CClientLoader::GetBaseDataScore( int ID )
{
	for( int i = 0; i < m_vBaseDataScore.size(); i++ )
	{
		if( ID == m_vBaseDataScore[i].m_iID )
		{
			return &m_vBaseDataScore[i];
		}
	}
	return NULL;
}