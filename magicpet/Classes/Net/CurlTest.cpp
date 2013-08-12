#include "CurlTest.h"

#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "curl/curl.h"
#include "../loader/CClientLoader.h"
#include "../platform/CDevice.h"

using namespace std;

static char* s_CurlTestUrl = "http://113.31.30.107/ServerTaskManager2.php";

//----------------------------------------------
static char errorBuffer[CURL_ERROR_SIZE];
static int writer(char *, size_t, size_t, string *);
static bool ainit(CURL *&, char *,string *);
static std::string& trim(std::string &);//函数声明放在ainit()声明后面

CurlTest *CurlTest::m_pInstance = NULL;
//100添加101更新102查找
CurlTest::CurlTest()
{
    setIsTouchEnabled(true);
}

CurlTest* CurlTest::ShareInstance()
{
    if( NULL == m_pInstance )
    {
        m_pInstance = new CurlTest();
    }
    return m_pInstance;
}

void CurlTest::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{

	//test2();
}
//--------------------------------------------------------------------
//
//
void  CurlTest::PostUpData( string Postbuffer )
{
    return;
    if( 0 == g_iGameMode )
    {
        return;
    }
    std::string TaskInfo = "TaskType=101&TaskData=" + Postbuffer;//更新
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( 0 == CDevice::CheckNet() )
    {
        CCMessageBox("网络貌似有问题！", "提示");
        return;
    }
#endif
	CURL *conn = NULL;
	CURLcode code;
	string buffer;
    
	curl_global_init(CURL_GLOBAL_DEFAULT);
	char* url = s_CurlTestUrl;
    
	if (!ainit(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
		return;
	}
    
	code = curl_easy_setopt(conn, CURLOPT_POST, 1);   
	code = curl_easy_setopt(conn,CURLOPT_POSTFIELDS,TaskInfo.c_str());
	//-----------------
	code = curl_easy_perform(conn);
    
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}
	curl_easy_cleanup(conn);
    
    string buf = trim(buffer);
    
    CCLOG( "PostUpData return = %s", buf.c_str() );
}
//--------------------------------------------------------------------
//
//
string CurlTest::GetData( string bufferData )
{
    return "";
    if( 0 == g_iGameMode )
    {
        return "{0}";
    }
    char TempBuf[1024];
    sprintf(TempBuf, "TaskType=102&TaskData={\"playerid\":\"%s\"}", bufferData.c_str());
    std::string TaskInfo = TempBuf;//查找
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( 0 == CDevice::CheckNet() )
    {
        CCMessageBox("网络貌似有问题！", "提示");
        return "";
    }
#endif
	CURL *conn = NULL;
	CURLcode code;
	string buffer;
    
	curl_global_init(CURL_GLOBAL_DEFAULT);
	char* url = s_CurlTestUrl;
    
	if (!ainit(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}
    
	code = curl_easy_setopt(conn, CURLOPT_POST, 1);   
	code = curl_easy_setopt(conn,CURLOPT_POSTFIELDS,TaskInfo.c_str());
	//-----------------
	code = curl_easy_perform(conn);
    
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		return "";
	}
	curl_easy_cleanup(conn);
    
	//重要一步，删除php返回信息的非json格式字符
	string buf = trim(buffer);
    return buf;
}
//--------------------------------------------------------------------
//
//
void  CurlTest::AddSelfData( string bufferData )
{
    return;
    if( 0 == g_iGameMode )
    {
        return;
    }
    std::string TaskInfo = "TaskType=100&TaskData=" + bufferData;//添加
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( 0 == CDevice::CheckNet() )
    {
        CCMessageBox("网络貌似有问题！", "提示");
        return;
    }
#endif
	CURL *conn = NULL;
	CURLcode code;
	string buffer;
    
	curl_global_init(CURL_GLOBAL_DEFAULT);
	char* url = s_CurlTestUrl;
    
	if (!ainit(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
        return;
	}
    

	code = curl_easy_setopt(conn, CURLOPT_POST, 1);   
	code = curl_easy_setopt(conn,CURLOPT_POSTFIELDS,TaskInfo.c_str());
	//-----------------
	code = curl_easy_perform(conn);
    
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}
	curl_easy_cleanup(conn);
    
    string buf = trim(buffer);
    CCLOG( "addSelf return = %s", buf.c_str() );
}

void CurlTest::SendMail(string SendId, string recId, string str )
{
    return;
    if( 0 == g_iGameMode )
    {
        return;
    }
    char szBuffer[256];
    sprintf( szBuffer, "{\"sid\":\"%s\",\"rid\":\"%s\",\"mail\":\"%s\"}", SendId.c_str(), recId.c_str(), str.c_str()  );
    std::string TaskInfo = "TaskType=104&TaskData=";//添加
    TaskInfo = TaskInfo + szBuffer;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( 0 == CDevice::CheckNet() )
    {
        CCMessageBox("网络貌似有问题！", "提示");
        return;
    }
#endif
	CURL *conn = NULL;
	CURLcode code;
	string buffer;
    
	curl_global_init(CURL_GLOBAL_DEFAULT);
	char* url = s_CurlTestUrl;
    
	if (!ainit(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
        return;
	}
    
    
	code = curl_easy_setopt(conn, CURLOPT_POST, 1);   
	code = curl_easy_setopt(conn,CURLOPT_POSTFIELDS,TaskInfo.c_str());
	//-----------------
	code = curl_easy_perform(conn);
    
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}
	curl_easy_cleanup(conn);
}

string CurlTest::GetMailData( string BufferData )
{
    return "";
    if( 0 == g_iGameMode )
    {
        return "{0}";
    }
    char TempBuf[1024];
    sprintf(TempBuf, "TaskType=103&TaskData={\"playerid\":\"%s\"}", BufferData.c_str());
    std::string TaskInfo = TempBuf;//查找
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( 0 == CDevice::CheckNet() )
    {
        CCMessageBox("网络貌似有问题！", "提示");
        return "";
    }
#endif
	CURL *conn = NULL;
	CURLcode code;
	string buffer;
    
	curl_global_init(CURL_GLOBAL_DEFAULT);
	char* url = s_CurlTestUrl;
    
	if (!ainit(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}
    
	code = curl_easy_setopt(conn, CURLOPT_POST, 1);   
	code = curl_easy_setopt(conn,CURLOPT_POSTFIELDS,TaskInfo.c_str());
	//-----------------
	code = curl_easy_perform(conn);
    
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		return "";
	}
	curl_easy_cleanup(conn);
    
	//重要一步，删除php返回信息的非json格式字符
	string buf = trim(buffer);
    return buf;
}

void CurlTest::test2()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( 0 == CDevice::CheckNet() )
    {
        CCMessageBox("网络貌似有问题！", "提示");
        return;
    }
#endif
	CURL *conn = NULL;
	CURLcode code;
	string buffer;
    
	curl_global_init(CURL_GLOBAL_DEFAULT);
	//char* url="http://113.31.30.107/db.php";
	//char* url = "http://192.168.1.101:88/ServerTaskManager2.php";
	char* url = "http://113.31.30.107/ServerTaskManager2.php";
    
	if (!ainit(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}
	//-----------------
	//post
	//std::string postdata = "{\"num\":\"2\"}";
	//std::string name = "name=魏征";
	
	//std::string TaskType = "TaskType=100&TaskData={\"first\":\"魏征\"}";
	//std::string TaskData = "TaskData={\"userid\":1,\"first\":\"\u9B4F\u5F81\",\"second\":\"two\",\"third\":\"three\",\"fourth\":\"four\"}";
    
	std::string aaa = "{\"playerid\":\"1008\",\"playershoppingmallmoney\":\"1001\",\"petid\":\"2\",\"petname\":\"魏征22aaaa加油\",\"petsex\":\"2\",\"pettitle\":\"9999\",\"petlevel\":\"8888\",\"petexp\":\"7777\",\"maxstaminapoint\":\"6666\",\"petturning\":\"5555\",\"petskillexppond\":\"4444\",\"itemnum\":\"3333\",\"petmoney\":\"2222\",\"aechievepoint\":\"1111\",\"aechieveflag\":\"0000\",\"skilllevel\":\"999\",\"skillexp\":\"888\",\"gametime\":\"777\",\"feed\":\"666\",\"feedsave\":\"555\",\"gameday\":\"444\"}";
	std::string TaskInfo = "TaskType=102&TaskData={\"playerid\":\"1008\"}";//查找
	//std::string TaskInfo = "TaskType=100&TaskData=" + aaa;//添加
	//std::string TaskInfo = "TaskType=101&TaskData=" + aaa;//更新
    
	char pp[2048];
	//sprintf(pp,name.c_str());
	sprintf(pp,TaskInfo.c_str());
	std::string o;
	//CChineseCode *d = new CChineseCode;
	//d->GB2312ToUTF_8(o,pp,name.size());
	//d->GB2312ToUTF_8(o,pp,TaskInfo.size());
	code = curl_easy_setopt(conn, CURLOPT_POST, 1);   
	//code = curl_easy_setopt(conn, CURLOPT_POSTFIELDS, o.c_str() );
	code = curl_easy_setopt(conn,CURLOPT_POSTFIELDS,pp);
	//code = curl_easy_setopt(conn,CURLOPT_POSTFIELDS,TaskData.c_str());
	//-----------------
	code = curl_easy_perform(conn);
    
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}
	curl_easy_cleanup(conn);
    
	//重要一步，删除php返回信息的非json格式字符
	string buf = trim(buffer);
    
	//m_pLabel->setString( buffer.c_str() );
    
	Json::Reader reader;
	Json::Value val;
    
	if(!reader.parse(buf,val))
	{
		int a = 0;
	}
	//数组
	//const Json::Value arrayObj = val["array"];  //如果有数组键值
	const Json::Value arrayObj = val; 
    
	int ii = arrayObj.size();
    
	for (int i=0; i<arrayObj.size(); i++)  
	{  
        
		/*std::string prop_id   = arrayObj[i]["prop_id"].asString();
         std::string prop_name = arrayObj[i]["prop_name"].asString();  
         m_pLabel->setString( prop_id.c_str() );
         m_pLabel->setString( prop_name.c_str() );*/
        
		//std::string petname = arrayObj[i]["petname"].asString();
		//m_pLabel->setString( petname.c_str() );
        
	}  
    
}
std::string& trim(std::string &s) 
{ 
    if (s.empty()) 
    { 
        return s; 
    }
    
	s.erase(0,s.find_first_of("{[")); 
	s.erase(s.find_last_of("]}") + 1); 
    return s; 
	
}

void CurlTest::test()
{
	CURL *conn = NULL;
	CURLcode code;
	string buffer;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	char* url="http://113.31.30.107/phptest.php";
	if (!ainit(conn,url,&buffer ))
	{
		fprintf(stderr, "Connection initializion failed\n");
		exit(EXIT_FAILURE);
	}
	code = curl_easy_perform(conn);
	   
	if (code != CURLE_OK)
	{
		fprintf(stderr, "Failed to get '%s' [%s]\n", url, errorBuffer);
		exit(EXIT_FAILURE);
	}
	curl_easy_cleanup(conn);

	printf("%s\n",buffer.c_str());

	Json::Reader reader;

	Json::Value val;

	if(!reader.parse(buffer,val))
	{
		int a = 0;
	}
    
    string textstr = val["Name"].asString();

    //char cstringbuffer[32];
	//memcpy( cstringbuffer,  "\u8fd9\u91cc\u662f\u4e2d\u6587\u6807\u9898", 32 );
    
    //int c = 0;
    //char pText[32];
    
   // char *testbuffer;
    //CChineseCode::UnicodeToUTF_8(pText, cstringbuffer);
    
    m_pLabel->setString( textstr.c_str() );
    
   // sprintf(testbuffer, "str = %s", str.c_str() );
   // CCLOG( testbuffer );
    
   // printf( testbuffer );


}
CurlTest::~CurlTest()
{
	m_pLabel->release();
}


static bool ainit(CURL *&conn, char *url,string *p_buffer)
{
    CURLcode code;
    conn = curl_easy_init();
    if (conn == NULL)
    {
        fprintf(stderr, "Failed to create CURL connection\n");
        exit(EXIT_FAILURE);
    }
    code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "Failed to set error buffer [%d]\n", code);
        return false;
    }
    code = curl_easy_setopt(conn, CURLOPT_URL, url);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
        return false;
    }
    code = curl_easy_setopt(conn, CURLOPT_FOLLOWLOCATION, 1);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "Failed to set redirect option [%s]\n", errorBuffer);
        return false;
    }
    code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
        return false;
    }
    code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, p_buffer);
    if (code != CURLE_OK)
    {
        fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
        return false;
    }
    return true;
}

static int writer(char *data, size_t size, size_t nmemb, string *writerData)
{
    unsigned long sizes = size * nmemb;
    if (writerData == NULL) return 0;
    writerData->append(data, sizes);
    return sizes;
}
 


