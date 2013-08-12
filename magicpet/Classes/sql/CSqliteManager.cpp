//
//  CSqliteManager
//  magicpet
//
//  Created by jia huyan on 11-11-23.
//  Copyright (c) 2011ƒÍ µ¿∑Êª•∂Ø£®±±æ©£©ø∆ºº”–œﬁ‘»Œπ´Àæ. All rights reserved.
//
#include "CSqliteManager.h"
#include "../Sprite/CPet.h"
#include "../Pushui/CEventControl.h"
#include "FileOperation.h"
#include "../md5.h"
#include "../CGameText.h"
//-----------------------------------------------------------------------------
//
//
int sqlcallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
    
	//char name[32];
	//int Id;
    
	for(i=0; i<argc; i++){
		CCLog("%s = %s\n", azColName[i], argv[i] );
	}

	if( NULL == g_pPetDataBlock )
	{
		g_pPetDataBlock = new PetDatablock();
		g_pPetDataBlock->pFeedPetData = new SFeedPetDataSave();
	}
	if( NULL == g_pPlayerEventCtrl )
	{
		g_pPlayerEventCtrl = new CEventControl();

	}

	//string strDeCode = CSqliteManager::GetInstance()->UncrypKey(argv[0],"magicpet");
	g_pPetDataBlock->playershoppingmallmoney = atoi( argv[0] );
	g_pPetDataBlock->petid		= atoi( argv[1] );
	g_pPetDataBlock->petName	= argv[2];
	g_pPetDataBlock->petsex		= atoi( argv[3] );
	g_pPetDataBlock->pettitle	= atoi( argv[4] );
	g_pPetDataBlock->petlevel	= atoi( argv[5] );
	if( 0 == g_pPetDataBlock->petlevel )
	{
		g_pPetDataBlock->petlevel = 1;
	}

	//strDeCode = CSqliteManager::GetInstance()->UncrypKey(argv[6],"magicpet");
	g_pPetDataBlock->petexp		= atoi(argv[6] );
	g_pPetDataBlock->maxstaminapoint = atoi( argv[7] );
	//if( 0 == g_pPetDataBlock->maxstaminapoint )
	//{
	//	g_pPetDataBlock->maxstaminapoint = 100;
	//}
	
	string translate =  argv[8];
	char tempBuf[32];
	CSqliteManager::GetInstance()->getUnit( translate.c_str(),0,",", tempBuf, 32 );
	g_pPetDataBlock->petturning[0] =  atoi( tempBuf );
	CSqliteManager::GetInstance()->getUnit( translate.c_str(),1,",", tempBuf, 32 );
	g_pPetDataBlock->petturning[1] =  atoi( tempBuf );

	g_pPetDataBlock->petskillexppond = atoi( argv[9] );
	g_pPetDataBlock->itemnum = atoi( argv[10] );

	//strDeCode = CSqliteManager::GetInstance()->UncrypKey(argv[11],"magicpet");
	g_pPetDataBlock->petmoney = atoi( argv[11] );

	{
		sqlite3_stmt *stat;
		char SqlBuffer[128];
		sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos] );
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0);
		sqlite3_step(stat);
		const void *pBlob = sqlite3_column_blob(stat, 12);//Get blob
		int blob_size = sqlite3_column_bytes(stat, 12); //Get size
		if(  blob_size <= 1  )
		{
			memset( g_pPlayerEventCtrl->m_TaskData, 0, sizeof( SNMSaveEvent ) * MAX_TASK_EVNET );
		}
		else
		{
			memcpy( g_pPlayerEventCtrl->m_TaskData, pBlob, sizeof( SNMSaveEvent ) * MAX_TASK_EVNET );
		}
	}
	//g_pPetDataBlock->aechieveflag = atoi( argv[13] );
	{
		sqlite3_stmt *stat;
		char SqlBuffer[128];
		sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos] );
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0);
		sqlite3_step(stat);
		const void *pBlob = sqlite3_column_blob(stat, 13);//Get blob
		int blob_size = sqlite3_column_bytes(stat, 13); //Get size
		if( 1 == blob_size )
		{
			memset( g_pPlayerEventCtrl->m_iEventFlag, 0, sizeof( int ) * MAX_EVENT_FLAG );
		}
		else
		{
			memcpy( g_pPlayerEventCtrl->m_iEventFlag, pBlob, sizeof( int ) * MAX_EVENT_FLAG );
		}
	}
	//g_pPetDataBlock->skilllevel = argv[14];
	{
		sqlite3_stmt *stat;
		char SqlBuffer[128];
		sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos] );
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0);
		sqlite3_step(stat);
		const void *pBlob = sqlite3_column_blob(stat, 14);//Get blob
		int blob_size = sqlite3_column_bytes(stat, 14); //Get size
		if( blob_size <= 1  )
		{
			memset( &g_pPetDataBlock->skilllevel, 0, sizeof( SSkillLevel ) );
		}
		else
		{
			memcpy( &g_pPetDataBlock->skilllevel, pBlob, sizeof( SSkillLevel ) );
		}
	}
	//g_pPetDataBlock->skillexp = atoi( argv[15] );
	{
		sqlite3_stmt *stat;
		char SqlBuffer[128];
		sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos] );
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0);
		sqlite3_step(stat);
		const void *pBlob = sqlite3_column_blob(stat, 15);//Get blob
		int blob_size = sqlite3_column_bytes(stat, 15); //Get size
		if( blob_size <= 1  )
		{
			memset( &g_pPetDataBlock->skillexp, 0, sizeof( SSkillExp ) );
		}
		else
		{
			memcpy( &g_pPetDataBlock->skillexp, pBlob, sizeof( SSkillExp ) );
		}
	}
	{
		sqlite3_stmt *stat;
		char SqlBuffer[128];
		sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos] );
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0);
		sqlite3_step(stat);
		const void *pBlob = sqlite3_column_blob(stat, 16);//Get blob
		int blob_size = sqlite3_column_bytes(stat, 16); //Get size
		if( blob_size <= 1  )
		{
			memset( &g_pPetDataBlock->gametime, 0, sizeof( SGameTime ) );
		}
		else
		{
			memcpy( &g_pPetDataBlock->gametime, pBlob, sizeof( SGameTime ) );
		}
	}
	{
		sqlite3_stmt *stat;
		char SqlBuffer[128];
		sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos] );
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0);
		sqlite3_step(stat);
		const void *pBlob = sqlite3_column_blob(stat, 17);//Get blob
		int blob_size = sqlite3_column_bytes(stat, 17); //Get size
		if( blob_size <= 1  )
		{
			memset( g_pPetDataBlock->pFeedPetData, 0, sizeof( SFeedPetDataSave ) );
			g_pPetDataBlock->pFeedPetData->MaxFeedCount = 2;
		}
		else
		{
			memcpy( g_pPetDataBlock->pFeedPetData, pBlob, sizeof( SFeedPetDataSave ) );
		}
	}
	//argv[18];
	{
		sqlite3_stmt *stat;
		char SqlBuffer[128];
		sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos] );
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0);
		sqlite3_step(stat);
		const void *pBlob = sqlite3_column_blob(stat, 18);//Get blob
		int blob_size = sqlite3_column_bytes(stat, 18); //Get size
		if( blob_size <= 1  )
		{
			memset( &g_pPetDataBlock->recordsave, 0, sizeof( SPetDataRecord )  );
		}
		else
		{
			memcpy( &g_pPetDataBlock->recordsave, pBlob, sizeof( SPetDataRecord )  );
		}
	}
	g_pPetDataBlock->gameday = atoi( argv[19] );

	printf("\n");
	return 0;
}


int sqlReSetcallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i; 

	int index;
	memcpy( &index, NotUsed, sizeof( int )  );
    
	for(i=0; i<argc; i++)
	{

		char *zErrMsg = 0;

		//incode 
		char SqlBuffer[128];
		sprintf( SqlBuffer, "update %s set %s=0",  m_iUserSaveName[index], azColName[i]  );
		int rc = sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			return 0;
		}

		CCLog("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

static int sqlSavecallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;

	char *zErrMsg = 0;

	char SqlBuffer[128];

	char inCodeBuffer[128];
	//sprintf( inCodeBuffer, "%d",  g_pPetDataBlock->playershoppingmallmoney);
	//string strInCode = CSqliteManager::GetInstance()->EncrypKey(inCodeBuffer,"magicpet");
	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[0], g_pPetDataBlock->playershoppingmallmoney);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[1], g_pPetDataBlock->petid);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s='%s'",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[2], g_pPetDataBlock->petName.c_str());
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[3], g_pPetDataBlock->petsex);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[4], g_pPetDataBlock->pettitle);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[5], g_pPetDataBlock->petlevel);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	//sprintf( inCodeBuffer, "%d", g_pPetDataBlock->petexp);
	//strInCode = CSqliteManager::GetInstance()->EncrypKey(inCodeBuffer,"magicpet");
	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[6], g_pPetDataBlock->petexp);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[7], g_pPetDataBlock->maxstaminapoint);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	char tempBuffer[32];
	sprintf( tempBuffer, "%d,%d", g_pPetDataBlock->petturning[0], g_pPetDataBlock->petturning[1] );
	sprintf( SqlBuffer, "update %s set %s='%s'",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[8], tempBuffer);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[9], g_pPetDataBlock->petskillexppond);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[10], g_pPetDataBlock->itemnum);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	//sprintf( inCodeBuffer, "%d", g_pPetDataBlock->petmoney);
	//strInCode = CSqliteManager::GetInstance()->EncrypKey(inCodeBuffer,"magicpet");
	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[11], g_pPetDataBlock->petmoney);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	sprintf( SqlBuffer, "update %s set %s=?",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[12]);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);
	if( NULL != g_pPlayerEventCtrl )
	{
		sqlite3_stmt *stat;
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0 ); 
		sqlite3_bind_blob( stat, 1, (char*)g_pPlayerEventCtrl->m_TaskData , sizeof( SNMSaveEvent ) * MAX_TASK_EVNET, NULL );
		sqlite3_step( stat );
		sqlite3_finalize( stat );
	}

	sprintf( SqlBuffer, "update %s set %s=?",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[13]);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);
	if( NULL != g_pPlayerEventCtrl )
	{
		sqlite3_stmt *stat;
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0 ); 
		sqlite3_bind_blob( stat, 1, (char*)g_pPlayerEventCtrl->m_iEventFlag , sizeof( int ) * MAX_EVENT_FLAG, NULL );
		sqlite3_step( stat );
		sqlite3_finalize( stat );
	}

	sprintf( SqlBuffer, "update %s set %s=?",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[14]);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);
	{
		sqlite3_stmt *stat;
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0 ); 
		sqlite3_bind_blob( stat, 1, (char*)&g_pPetDataBlock->skilllevel , sizeof( SSkillLevel ), NULL );
		sqlite3_step( stat );
		sqlite3_finalize( stat );
	}

	sprintf( SqlBuffer, "update %s set %s=?",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[15]);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);
	{
		sqlite3_stmt *stat;
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0 ); 
		sqlite3_bind_blob( stat, 1, (char*)&g_pPetDataBlock->skillexp , sizeof( SSkillExp ), NULL );
		sqlite3_step( stat );
		sqlite3_finalize( stat );
	}

	sprintf( SqlBuffer, "update %s set %s=?",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[16]);
	{
		sqlite3_stmt *stat;
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0 ); 
		sqlite3_bind_blob( stat, 1, (char*)&g_pPetDataBlock->gametime , sizeof( SGameTime ), NULL );
		sqlite3_step( stat );
		sqlite3_finalize( stat );
	}

	sprintf( SqlBuffer, "update %s set %s=?",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[17] );
	if( NULL != g_pPetDataBlock->pFeedPetData )
	{
		sqlite3_stmt *stat;
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0 ); 
		sqlite3_bind_blob( stat, 1, (char*)g_pPetDataBlock->pFeedPetData , sizeof( SFeedPetDataSave ), NULL );
		sqlite3_step( stat );
		sqlite3_finalize( stat );
	}

	sprintf( SqlBuffer, "update %s set %s=?",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[18], g_pPetDataBlock->recordsave);
	{
		sqlite3_stmt *stat;
		sqlite3_prepare(CSqliteManager::GetInstance()->db, SqlBuffer, -1, &stat, 0 ); 
		sqlite3_bind_blob( stat, 1, (char*)&g_pPetDataBlock->recordsave , sizeof( SPetDataRecord ), NULL );
		sqlite3_step( stat );
		sqlite3_finalize( stat );
	}

	sprintf( SqlBuffer, "update %s set %s=%d",  m_iUserSaveName[CSqliteManager::GetInstance()->m_iCurPos], azColName[19], g_pPetDataBlock->gameday);
	sqlite3_exec(CSqliteManager::GetInstance()->db, SqlBuffer, 0, 0, &zErrMsg);

	for(i=0; i<argc; i++)
	{
		CCLog("%s = %s\n", azColName[i], argv[i]);
	}

	return 0;
}

CSqliteManager *CSqliteManager::m_pInstance = NULL;;
//--------------------------------------------------------------------------
//
//
CSqliteManager::CSqliteManager()
{
	m_iCurPos = -1;
	m_bIsPlayerCheat = false;
}
//--------------------------------------------------------------------------
//
//
CSqliteManager::~CSqliteManager()
{
	CC_SAFE_DELETE( g_pPetDataBlock );

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//SaveSql();
#endif
}
//-------------------------------------------------------------------------
//
//
CSqliteManager *CSqliteManager::GetInstance()
{
	if( NULL == m_pInstance )
	{
		m_pInstance = new CSqliteManager();
	}
	return m_pInstance;
}
// Returns, in 'val', the specified component of a string.
//
//
void CSqliteManager::getUnit(const char *string, int index, const char *set, char val[], int len)
{
	int sz;
	while(index--)
	{
		if(!*string)
			return;
		sz = strcspn(string, set);
		if (string[sz] == 0)
			return;
		string += (sz + 1);
	}
	sz = strcspn(string, set);
	if (sz == 0)
		return;

	if( ( sz + 1 ) > len )
		return;

	strncpy(val, string, sz);
	val[sz] = '\0';
}
//----------------------------------------------------------------------
//
//
int CSqliteManager::getUnitCount(const char *string, const char *set)
{
	int count = 0;
	int last = 0;
	while(*string)
	{
		last = *string++;

		for(int i =0; set[i]; i++)
		{
			if(last == set[i])
			{
				count++;
				last = 0;
				break;
			}
		}
	}
	if(last)
	{
		count++;
	}
	return count;
}
//--------------------------------------------------------------------
//
//
string CSqliteManager::getSaveFilePath()
{
    string path;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path = CCFileUtils::getWriteablePath();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// In android, every programe has a director under /data/data.
	// The path is /data/data/ + start activity package name.
	// You can save application specific data here.
	path.append("/data/data/org.cocos2dx.application/tmpfile");
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// You can save file in anywhere if you have the permision.
	//path.append("D:/tmpfile");
    path = CCFileUtils::getWriteablePath();
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE)
	path = cocos2d::CCApplication::sharedApplication().getAppDataPath();
    
#ifdef _TRANZDA_VM_
	// If runs on WoPhone simulator, you should insert "D:/Work7" at the
	// begin. We will fix the bug in no far future.
	path = "D:/Work7" + path;
	path.append("tmpfile");
#endif
    
#endif
    
	return path;
}
//-----------------------------------------------------------------
//
//
int CSqliteManager::copyfile(const char* source, const char* dest)
{
    
	FILE *f,*y;
	char ch;
	f = fopen(source,"rb");
	y = fopen(dest,"wb");
	if(f==NULL) return 1;
	if(y==NULL) return 2;
	while((ch=fgetc(f))!=EOF)
	{
		if(fputc(ch,y)==EOF) return 3;
	}
	fputc(EOF,y);
	fclose(f);
	fclose(y);
	return 0;
}
//-------------------------------------------------
//
//
bool CSqliteManager::isDBFileExist( string FilePath )
{
	FILE *fp = fopen(FilePath.c_str(), "r");
	bool bRet = false;
    
	if (fp)
	{
		bRet = true;
		fclose(fp);
	}
    
	return bRet;
}
//-------------------------------------------------------------------
//
//
bool CSqliteManager::CreateBaseDataDB()
{
	char *zErrMsg = 0;
	int rc;
    
	string strPath = getSaveFilePath();
	strPath = strPath + "magicpet.sl";
    
	bool isFile = false;
	isFile = isDBFileExist( strPath );
	if( !isFile )
	{
		string path;
		path = CCFileUtils::fullPathFromRelativePath( "data/magicpet.sl" );
		copyfile( path.c_str(), strPath.c_str() );
        
		rc = sqlite3_open(strPath.c_str(), &db);
        
		if(rc)
		{
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			sqlite3_close(db);
			CCAssert( NULL,"" );
			return false;
		}
		ReSetSql(0);
		
		CCUserDefault::sharedUserDefault()->setBoolForKey("music", true );
		CCUserDefault::sharedUserDefault()->setBoolForKey("sound", true );
	}
    
	rc = sqlite3_open(strPath.c_str(), &db);
	if(rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		CCAssert( NULL,"" );
		return false;
	}

    
    return true;
}
//---------------------------------------------------------
//
//
void CSqliteManager::CheckMD5()
{
	string md5Read = CCUserDefault::sharedUserDefault()->getStringForKey( "incode" );
	//string strDeCode = CSqliteManager::GetInstance()->UncrypKey(md5Read.c_str() ,"magicpet");

	string strPath = getSaveFilePath();
	strPath = strPath + "magicpet.sl";
	ifstream infile(strPath.c_str(), ios::binary );
	MD5 md5(infile);

	string strMd5 = md5.toString();
	if( strMd5 != md5Read )
	{
		m_bIsPlayerCheat = true;
		CCMessageBox(the_message_cheat[g_iCurLanguageIndex],the_message_alert[g_iCurLanguageIndex]);
	}
}
//-------------------------------------------------------
//
//
void CSqliteManager::WriteMD5()
{
	string strPath = getSaveFilePath();
	strPath = strPath + "magicpet.sl";
	ifstream infile(strPath.c_str(), ios::binary );
	MD5 md5(infile);
    	
	//string strInCode = CSqliteManager::GetInstance()->EncrypKey(md5.toString().c_str(),"magicpet");
	CCUserDefault::sharedUserDefault()->setStringForKey("incode", md5.toString() );
}
//---------------------------------------------------------
//
//
void CSqliteManager::ReSetSql(  int index )
{
	//update film set starring

	char SqlBuffer[64];
	sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[index] );

	char *zErrMsg = 0;

	int pIndex = index;

	int rc = sqlite3_exec(db, SqlBuffer, sqlReSetcallback, &pIndex, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
        return;
	}

	WriteMD5();
}
//----------------------------------------------------
//
//
void CSqliteManager::ReadSql()
{
	CheckMD5();

	char SqlBuffer[64];
	sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[m_iCurPos] );

	char *zErrMsg = 0;

	int rc = sqlite3_exec(db, SqlBuffer, sqlcallback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
        return;
	}

}
//--------------------------------------------------------------------
//
//
bool CSqliteManager::CheckIsEmpty()
{
	ReadSql();

	if( 0 == g_pPetDataBlock->petid && 0 == g_pPetDataBlock->petexp )
	{
		return true;
	}
	return false;
}
//--------------------------------------------------------------------
//
//
void CSqliteManager::SaveSql()
{
    if( -1 == CSqliteManager::GetInstance()->m_iCurPos )
    {
        return;
    }
	char SqlBuffer[64];
	sprintf( SqlBuffer, "select * from %s", m_iUserSaveName[m_iCurPos] );

	char *zErrMsg = 0;

	int rc = sqlite3_exec(db, SqlBuffer, sqlSavecallback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
        return;
	}

	WriteMD5();
}

void CSqliteManager::Release()
{
	CC_SAFE_DELETE( m_pInstance );
}


char* CSqliteManager::substr(const char* _str, int pos, int len)
{
    static char ptr[10];

    memcpy(ptr, _str+pos-1, len);
    ptr[len] = '\0';

    return ptr;
}

//加密函数
string CSqliteManager::EncrypKey(const char* src,  const char* key)
{
    int KeyPos = -1;
    int SrcPos = 0;   
    int SrcAsc = 0;
    time_t t;
    
    int KeyLen = strlen(key);
    if(KeyLen == 0)
          return "";

    srand((unsigned) time(&t));
    int offset = rand() % 255;

    char buff[3];
    sprintf(buff, "%1.2x", offset);
    string dest = buff;

    for(int i = 0; i < strlen(src); i++)
    {
        SrcAsc = (src[i] + offset) % 255;

        if (KeyPos < KeyLen-1)
            KeyPos++;
        else
            KeyPos = 0;

        SrcAsc = SrcAsc ^ key[KeyPos];

        memset(buff, 0, sizeof(buff));
        sprintf(buff, "%1.2x", SrcAsc);   
        dest = dest + (string)buff;

        offset = SrcAsc;   
    }
    return dest;   
}


//解密函数
string CSqliteManager::UncrypKey(const char* src, const char* key)
{   
    int KeyLen = strlen(key);
    if(KeyLen == 0)
        return "";

    int KeyPos = -1;
    int offset = 0;   
    string dest = "";
    int SrcPos = 0;   
    int SrcAsc = 0;
    int TmpSrcAsc = 0;  

    char buff[5];
    sprintf(buff, "0x%s", substr(src, 1, 2));
    sscanf(buff, "%x", &offset);
    SrcPos = 3;
    while(SrcPos <= strlen(src))
    {
        sprintf(buff, "0x%s", substr(src, SrcPos, 2));
        sscanf(buff, "%x", &SrcAsc);
        if (KeyPos < KeyLen-1)
            KeyPos++;
        else
            KeyPos = 0;

        TmpSrcAsc = SrcAsc ^ key[KeyPos];
              
        if (TmpSrcAsc <= offset)
            TmpSrcAsc = 255 + TmpSrcAsc - offset;
        else   
            TmpSrcAsc = TmpSrcAsc - offset;

        dest += char(TmpSrcAsc);
        offset = SrcAsc;
        SrcPos = SrcPos + 2;
    }

    return dest;
}
