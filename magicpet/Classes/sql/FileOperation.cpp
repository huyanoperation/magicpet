// to enable CCLOG()
#define COCOS2D_DEBUG 1

#include "FileOperation.h"
#include <stdio.h>

using namespace std;

char g_DecodeBuffer[128];

void FileOperation::saveFile( const char *buf )
{
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "wb");

	if (! fp)
	{
		CCLOG("can not create file %s", path.c_str());
		return;
	}

	memcpy( g_DecodeBuffer, buf, 128 );

	fwrite( g_DecodeBuffer, sizeof( unsigned char ), 128, fp  );
	fclose(fp);
}

string FileOperation::readFile()
{
	string path = getFilePath();
	FILE *fp = fopen(path.c_str(), "rb");

	if (! fp)
	{
		CCLOG("can not open file %s", path.c_str());
		return "0";
	}

	fread( g_DecodeBuffer, sizeof( unsigned char ), 128, fp );
	CCLOG("read content %s", g_DecodeBuffer);

	fclose(fp);

	string decodeStr( g_DecodeBuffer );

	return decodeStr;
}

string FileOperation::getFilePath()
{
	string path("");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	path = CCFileUtils::getWriteablePath();
	path = path + "decode.m";
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// In android, every programe has a director under /data/data.
	// The path is /data/data/ + start activity package name.
	// You can save application specific data here.
	path.append("/data/data/org.cocos2dx.application/tmpfile");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// You can save file in anywhere if you have the permision.
	path = CCFileUtils::getWriteablePath();
	path = path + "incode.m";
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