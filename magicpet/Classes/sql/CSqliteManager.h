//
//  CSqliteManager
//  magicpet
//
//  Created by jia huyan on 11-11-23.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
#include "../GameConstant.h"
#include "sqlite3.h"

static const char* m_iUserSaveName[] = 
{
	"userdata1",
	"userdata2",
	"userdata3",
};

static int sqlcallback(void *NotUsed, int argc, char **argv, char **azColName);

static int sqlReSetcallback(void *NotUsed, int argc, char **argv, char **azColName);

static int sqlSavecallback(void *NotUsed, int argc, char **argv, char **azColName);

class CSqliteManager
{
public:
	CSqliteManager();
	~CSqliteManager();

	static CSqliteManager *GetInstance();

	static CSqliteManager *m_pInstance;

	string getSaveFilePath();

	int copyfile(const char* source, const char* dest);

	bool isDBFileExist( string FilePath );

	void CheckMD5();
	void WriteMD5();

	bool CreateBaseDataDB();

	void ReadSql();
	bool CheckIsEmpty();

	void ReSetSql( int index );

	void SaveSql();

	void Release();

	void getUnit(const char *string, int index, const char *set, char val[], int len);

	string EncrypKey(const char* src,  const char* key);

	char* substr(const char* _str, int pos, int len);

	string UncrypKey(const char* src, const char* key);

	int m_iCurPos;

	int getUnitCount(const char *string, const char *set);

	sqlite3 *db;

	bool m_bIsPlayerCheat;

};

