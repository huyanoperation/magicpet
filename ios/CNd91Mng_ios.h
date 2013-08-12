//
//  CNd91Mng.h
//  magicpet
//
//  Created by jia huyan on 12-3-22.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include <string>

using namespace std;

class CNd91Mng
{
public: 
    CNd91Mng();
    ~CNd91Mng();
    
    static bool islogin();
    
    static void checkNewVersion();
    
    static void buyVirtualGoodsWithId( string productId, int count );
    static void checkAbnormalOrders();
    
    static void login91Platform();
    static void enter91Platform();	//如果未登录，会先引导登录
    static void enterUserFeedBack();//如果未登录，会先引导登录
    static void enter91Recharge();	//如果未登录，会先引导登录
    static void upLoadLeaderBoard( int score, int Id );
    static void NdUnLockAchievement(int achiveId);
    static void BuyResult(string str);
    
    static string NdGetMyInfo();
    static bool  NdGetUserLeaderBoardInfoList( string uin, int Id );
    static void onGetPlayerInfoCallBack( string uin, string name, string seluin );
    static void onGetPlayerFinish( );
    static void NdShareToThirdPlatform(string str, string path);
    static void OpenUrl( const char* url );
    
    static void InitadBanner();
    static void RemoveBanner();
};