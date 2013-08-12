//
//  CNd91Mng.cpp
//  magicpet
//
//  Created by jia huyan on 12-3-22.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//
//
#include "CNd91Mng_ios.h"
//#include "Nd91Mng.h"
//#include "CShopLayer.h"
#include "CGameWorldScene.h"
#include "CGameCenter_ios.h"
#include "RootViewController.h"

CNd91Mng::CNd91Mng()
{
    
}

CNd91Mng::~CNd91Mng()
{
    
}

bool CNd91Mng::islogin()
{
//    if ([[Nd91Mng sharedInstance] isLogin]) 
//    {
//        return true;
//    }
    
    
    return true;
}

void CNd91Mng::checkNewVersion()
{
    CGameCenter::login();
}

void CNd91Mng::buyVirtualGoodsWithId( string productId, int count )
{
    // NSString *str = [NSString stringWithUTF8String:productId.c_str()];
    // [[Nd91Mng sharedInstance] buyVirtualGoodsWithId:str count:count];
}

void CNd91Mng::checkAbnormalOrders()
{
    //[[Nd91Mng sharedInstance] checkAbnormalOrders];
}

void CNd91Mng::login91Platform()
{
    //[[Nd91Mng sharedInstance] login91Platform];
}

//如果未登录，会先引导登录
void CNd91Mng::enter91Platform()
{
    //[[Nd91Mng sharedInstance] enter91Platform];
    CGameCenter::showLeaderboard();
}
//如果未登录，会先引导登录
void CNd91Mng::enterUserFeedBack()
{
    //[[Nd91Mng sharedInstance] enterUserFeedBack];
}
//如果未登录，会先引导登录
void CNd91Mng::enter91Recharge()
{
    //[[Nd91Mng sharedInstance] enter91Recharge];
}

void CNd91Mng::upLoadLeaderBoard( int score, int Id )
{
//    if( !CNd91Mng::islogin() )
//    {
//        return;
//    }
//    [[Nd91Mng sharedInstance] upLoadLeaderBoard: score : Id];
    
    CGameCenter::submitHighScore(score);
}

void CNd91Mng::NdUnLockAchievement( int achiveId )
{
//    if( !CNd91Mng::islogin() )
//    {
//        return;
//    }
//    [[Nd91Mng sharedInstance] NdUnLockAchievement:achiveId];
}

void CNd91Mng::BuyResult(string str)
{
    //CShopLayer::BuyResult( str );
}

string CNd91Mng::NdGetMyInfo()
{
//    if( !CNd91Mng::islogin() )
//    {
//        return "";
//    }
//    return [[[Nd91Mng sharedInstance] NdGetMyInfo] UTF8String];
    return "";
}

bool CNd91Mng::NdGetUserLeaderBoardInfoList( string uin, int Id )
{
//    if( !CNd91Mng::islogin() )
//    {
//        login91Platform();
//        return false;
//    }
//    
//     NSString *Uin = [NSString stringWithUTF8String: uin.c_str() ];
//    
//    [[Nd91Mng sharedInstance] NdGetUserLeaderBoardInfoList:Uin : (int)Id  ];
    CGameCenter::getLeaderBoardPlayer();
    return true;
}

void CNd91Mng::onGetPlayerInfoCallBack( string uin, string name, string sefuin )
{
    if( sefuin != uin )
    {
        CGameWorldScene::OnUserCallBack(uin, name);
    }
}

void CNd91Mng::onGetPlayerFinish()
{
    CGameWorldScene::OnUserFinishCallBack();
}

void CNd91Mng::NdShareToThirdPlatform(string str, string path)
{
//    if( !CNd91Mng::islogin() )
//    {
//        login91Platform();
//        return;
//    }
//    string newStr = str + "http://mobile.91.com/Soft/iPhone/com.doformat.91magicpet.html";
//    
//    NSString *nsstr = [NSString stringWithUTF8String: newStr.c_str() ];
//    NSString *nspath = [NSString stringWithUTF8String: path.c_str() ];
//    [[Nd91Mng sharedInstance] NdShareToThirdPlatform:nsstr : nspath];
}
void CNd91Mng::OpenUrl( const char* url )
{
    if (!url) return;
    NSURL *nsurl = [NSURL URLWithString:[NSString stringWithCString:url encoding:NSUTF8StringEncoding]];
    [[UIApplication sharedApplication] openURL:nsurl];
}

void CNd91Mng::InitadBanner()
{
    [g_RootViewController InitMobiSageAdBanner];
}


void CNd91Mng::RemoveBanner()
{
    [g_RootViewController RemoveMobiSageAdBanner];
}

