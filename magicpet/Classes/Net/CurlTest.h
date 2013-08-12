#ifndef _CURL_TEST_H_
#define _CURL_TEST_H_

#include "cocos2d.h"
#include "CChineseCode.h"

using namespace cocos2d;

class CurlTest :public CCLayer
{
public:
	CurlTest();
	~CurlTest();
    
    static CurlTest* m_pInstance;
    static CurlTest* ShareInstance();

	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void test();
    void test2();
    void SendMail(string SendId, string recId, string str);
    string GetMailData( string BufferData );
    
    void  PostUpData( string buffer );
    string GetData( string buffer );
    void  AddSelfData( string buffer );


private:
	cocos2d::CCLabelTTF*         m_pLabel;
};

#endif // _CURL_TEST_H_
