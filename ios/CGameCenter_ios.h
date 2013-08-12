//
//  CGameCenter_ios.h
//  Elftowers
//
//  Created by huyan on 13-1-24.
//
//
#include "GameConstant.h"

class CGameCenter
{
public:
    CGameCenter();
    ~CGameCenter();
    
    static void login();
    static void showLeaderboard();
    static void submitHighScore( int score );
    static void submitAchievement( int ID );
    static void getLeaderBoardPlayer();
    static void FindPlayer();
    static int icloudGetData();
    static void icloudSetData( string str );
};

