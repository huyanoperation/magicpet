//
//  CGHelp_ios.m
//  Elftowers
//
//  Created by huyan on 13-3-29.
//
//
#import "CGHelp_ios.h"
#import "CPet.h"
#import "CSelectEnemyScene.h"
#import "CClientLoader.h"
#import "CFightScene.h"

@implementation CGHelp_ios

static CGHelp_ios *pshareCGHelp_ios = nil;
+ (CGHelp_ios *) sharedInstance {
    if (!pshareCGHelp_ios) {
        pshareCGHelp_ios = [[CGHelp_ios alloc] init];
    }
    return pshareCGHelp_ios;
}

-(id) init
{
	// always call "super" init
	// Apple recommends to re-assign "self" with the "super" return value
	if( (self=[super init])) {
        ourRandom = arc4random();
        [self setGameState:kGameStateWaitingForMatch];
        
	}
	return self;
}

- (void)sendData:(NSData *)data {
    NSError *error;
    BOOL success = [[GCHelper sharedInstance].match sendDataToAllPlayers:data withDataMode:GKMatchSendDataReliable error:&error];
    if (!success) {
        //CCLOG(@"Error sending init packet");
        [self matchEnded];
    }
}

- (void)sendRandomNumber {
    
    MessageRandomNumber message;
    message.message.messageType = kMessageTypeRandomNumber;
    message.randomNumber = ourRandom;
    NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageRandomNumber)];
    [self sendData:data];
}

- (void)sendGameBegin {
    
    MessageGameBegin message;
    message.message.messageType = kMessageTypeGameBegin;
    NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageGameBegin)];
    [self sendData:data];
    
}

- (void)tryStartGame {
    
    if (isPlayer1 && gameState == kGameStateWaitingForStart) {
        [self setGameState:kGameStateActive];
        [self sendGameBegin];
    }
    
}

- (void)inviteReceived
{
    //CGameCenter::GameCenterFindPlayer();
}

- (void)setGameState:(GameState)state {
    
    gameState = state;
    if (gameState == kGameStateWaitingForMatch) {
        //[debugLabel setString:@"Waiting for match"];
        NSLog(@"Waiting for match");
    } else if (gameState == kGameStateWaitingForRandomNumber) {
        //[debugLabel setString:@"Waiting for rand #"];
        NSLog(@"Waiting for rand #");
    } else if (gameState == kGameStateWaitingForStart) {
        //[debugLabel setString:@"Waiting for start"];
        NSLog(@"Waiting for start");
    } else if (gameState == kGameStateActive) {
        //[debugLabel setString:@"Active"];
        NSLog(@"Active");
    } else if (gameState == kGameStateDone) {
        //[debugLabel setString:@"Done"];
        NSLog(@"Done");
    } 
    
}

-(void)findPlayer:(NSString*) playerNickName
{
    //CPlayerDataBlock::GetInstance()->m_NetOtherPlayerName = [playerNickName UTF8String];
}

- (void)matchStarted {
    NSLog(@"Match started");
    if (receivedRandom) {
        [self setGameState:kGameStateWaitingForStart];
    } else {
        [self setGameState:kGameStateWaitingForRandomNumber];
    }
    [self sendRandomNumber];
    [self tryStartGame];
}

- (void)matchEnded {
    NSLog(@"matchEnded");
   // CGameCenter::OnPlayerPKExit();
}

- (void)SendMoveAction
{
//    MessageMove message;
//    message.message.messageType = kMessageTypeMove;
//    for( int i = 0; i < 5; i++ )
//    {
//        message.Action[i] = CPlayerDataBlock::GetInstance()->m_OherPlayerAttackData.Action[i];
//        message.skill[i] = CPlayerDataBlock::GetInstance()->m_OherPlayerAttackData.skill[i];
//    }
//    NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageMove)];
//    [self sendData:data];
}


- (void)match:(GKMatch *)match didReceiveData:(NSData *)data fromPlayer:(NSString *)playerID {
    
    // Store away other player ID for later
    if (otherPlayerID == nil) {
        otherPlayerID = [playerID retain];
    }
    
    Message *message = (Message *) [data bytes];
    if (message->messageType == kMessageTypeRandomNumber) {
        
        MessageRandomNumber * messageInit = (MessageRandomNumber *) [data bytes];
        NSLog(@"Received random number: %ud, ours %ud", messageInit->randomNumber, ourRandom);
        bool tie =false;
        
        if (messageInit->randomNumber == ourRandom) {
            NSLog(@"TIE!");
            tie =true;
            ourRandom = arc4random();
            [self sendRandomNumber];
        } else if (ourRandom > messageInit->randomNumber) {
            NSLog(@"We are player 1");
            isPlayer1 = YES;
        } else {
            NSLog(@"We are player 2");
            isPlayer1 = NO;
        }
        if (!tie) {
            receivedRandom = YES;
            if (gameState == kGameStateWaitingForRandomNumber)
            {
                [self setGameState:kGameStateWaitingForStart];
            }
            [self tryStartGame];
            if( isPlayer1 )
            {
                [self setGameState:kGameStateActive];
                NSLog(@" player 1 start game");
                MessageGetOherPlayerData message;
                message.message.messageType = kMessageActiveGetOtherPlayerData;
                strcpy(message.PetName, g_pPetDataBlock->petName.c_str() );
                message.petId       = g_pPetDataBlock->petid;
                if( g_pPetDataBlock->petid <= 10 )
                {
                    return;
                }
                message.petlevel    = g_pPetDataBlock->petlevel;
                
                message.skill[0]    = g_pPetDataBlock->skilllevel.skillsLevel[0];
                message.skill[1]    = g_pPetDataBlock->skilllevel.skillsLevel[1];
                
        
                NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageGetOherPlayerData)];
                [self sendData:data];
                

            }
        }
        
    }
    else if (message->messageType == kMessageTypeGameBegin)
    {
        [self setGameState:kGameStateActive];
        NSLog(@" player 2 start game");
        if( !isPlayer1 )
        {
            MessageGetOherPlayerData message;
            message.message.messageType = kMessageActiveGetOtherPlayerData;
            strcpy(message.PetName, g_pPetDataBlock->petName.c_str() );
            message.petId       = g_pPetDataBlock->petid;
            if( g_pPetDataBlock->petid <= 10 )
            {
                return;
            }
            message.petlevel    = g_pPetDataBlock->petlevel;
            
            message.skill[0]    = g_pPetDataBlock->skilllevel.skillsLevel[0];
            message.skill[1]    = g_pPetDataBlock->skilllevel.skillsLevel[1];
            NSData *data = [NSData dataWithBytes:&message length:sizeof(MessageGetOherPlayerData)];
            [self sendData:data];
        }
        
    }
    else if (message->messageType == kMessageActiveGetOtherPlayerData)
    {
        MessageGetOherPlayerData * messageData = (MessageGetOherPlayerData *) [data bytes];
        CSelectEnemyScene *pSelectEnemyScene = new CSelectEnemyScene();
        char uinbuffer[32];
 
       PetDatablock * pDataBlock = pSelectEnemyScene->GetOthers(uinbuffer, messageData->PetName);
        pDataBlock->petName     =  messageData->PetName;
        pDataBlock->petid       = messageData->petId;
        if( pDataBlock->petid <= 10 )
        {
            return;
        }
        pDataBlock->petlevel    = messageData->petlevel;
        pDataBlock->skilllevel.skillsLevel[0] = messageData->skill[0];
        pDataBlock->skilllevel.skillsLevel[1] = messageData->skill[1];
        //
        CBaseDataPet* pPetBaseData = g_pClientLoader->GetBaseDataPetFromId( pDataBlock->petid );
        //
        pDataBlock->ResId    = pPetBaseData->animationid;
        pDataBlock->rbg[0]   = pPetBaseData->rgb[0];
        pDataBlock->rbg[1]   = pPetBaseData->rgb[1];
        pDataBlock->rbg[2]   = pPetBaseData->rgb[2];
        //
        CSelectEnemyScene::s_pPetDataBlock = pDataBlock;
        //
        CCDirector::sharedDirector()->purgeCachedData();
        CFightScene::Show();
        //                
        CC_SAFE_DELETE( pSelectEnemyScene );
        
    }
    else if (message->messageType == kMessageTypeMove)
    {
//        MessageMove * messageData = (MessageMove *) [data bytes];
//        memset( &CPlayerDataBlock::GetInstance()->m_OherPlayerAttackData,0,sizeof(NetOtherPlayerAttack));
//        for( int i = 0; i < 5; i++ )
//        {
//            CPlayerDataBlock::GetInstance()->m_OherPlayerAttackData.Action[i] =  messageData->Action[i];
//            CPlayerDataBlock::GetInstance()->m_OherPlayerAttackData.skill[i]  = messageData->skill[i];
//        }
//        NSLog(@"Received move");
//        g_pFightLayer->NetOtherPlayerAttack();
        
//        if (isPlayer1) {
//            [player2 moveForward];
//        } else {
//            [player1 moveForward];
//        }
        
    }
    else if (message->messageType == kMessageTypeGameOver)
    {
//        
//        MessageGameOver * messageGameOver = (MessageGameOver *) [data bytes];
//        NSLog(@"Received game over with player 1 won: %d", messageGameOver->player1Won);
        
//        if (messageGameOver->player1Won) {
//            [self endScene:kEndReasonLose]; 
//        } else {
//            [self endScene:kEndReasonWin]; 
//        }
        
    } 
}



@end
