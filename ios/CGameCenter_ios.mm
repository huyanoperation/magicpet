//
//  CGameCenter_ios.cpp
//  Elftowers
//
//  Created by huyan on 13-1-24.
//
//

#include "CGameCenter_ios.h"
#include "TapperController.h"
#include "RootViewController.h"
#include "GCHelper.h"
#include "CGHelp_ios.h"

#define GAMECENTER_TURN_ON

CGameCenter::CGameCenter()
{
    
}

CGameCenter::~CGameCenter()
{
    
}
//----------------------------------------------
//
//
void CGameCenter::showLeaderboard()
{    
#ifdef GAMECENTER_TURN_ON
    [[TapperController sharedInstance] showLeaderboard];
#endif
}
//------------------------------------
//
//
void CGameCenter::submitHighScore( int score )
{

#ifdef GAMECENTER_TURN_ON
    [[TapperController sharedInstance] submitHighScore:score];
#endif
}
//-----------------------------------
//
//
void CGameCenter::submitAchievement( int index )
{
    
#ifdef GAMECENTER_TURN_ON
//    char buffer[64];
//    sprintf(buffer, "com.doformat.Elftowers.level%d", index);
//    NSString *str = [NSString stringWithUTF8String: buffer ];
//    [[TapperController sharedInstance] submitAchievement : str];
#endif
}
//-----------------------------------
//
//
void CGameCenter::getLeaderBoardPlayer()
{
    [[TapperController sharedInstance] retrieveTopTenScores];
}
//-----------------------------------
//
//
void CGameCenter::FindPlayer()
{
    CGHelp_ios *pHelp = [CGHelp_ios sharedInstance];
    [[GCHelper sharedInstance] findMatchWithMinPlayers:2 maxPlayers:2 viewController:g_RootViewController delegate:pHelp];
}
//-----------------------------------
//
//
void CGameCenter::login()
{
    CGHelp_ios *pHelp = [CGHelp_ios sharedInstance];
    [[GCHelper sharedInstance] setDelegate:pHelp];
#ifdef GAMECENTER_TURN_ON
    [[TapperController sharedInstance] login];
#endif
}
//-----------------------------------
//
//
int CGameCenter::icloudGetData()
{
#ifdef GAMECENTER_TURN_ON
//    NSString *str = [[TapperController sharedInstance] GetSaveKey];
//    if( str != nil )
//    {
//        string UTF8 = [ str UTF8String ];
//        int data = atoi(UTF8.c_str());
//        return data;
//    }
#endif
    return 0;
}
//----------------------------------------
//
//
void CGameCenter::icloudSetData( string str )
{
#ifdef GAMECENTER_TURN_ON
//    NSString *nstr = [NSString stringWithUTF8String:str.c_str() ];
//    [[TapperController sharedInstance] saveKey : nstr];
#endif
}