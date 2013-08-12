//
//  CGHelp_ios.h
//  Elftowers
//
//  Created by huyan on 13-3-29.
//
//

#import <Foundation/Foundation.h>
#import "GCHelper.h"

typedef enum {
    kGameStateWaitingForMatch =0,
    kGameStateWaitingForRandomNumber,
    kGameStateWaitingForStart,
    kGameStateActive,
    kGameStateDone
} GameState;



//　　你也需要添加一个游戏结束的新的原因---断开连接。因此，修改EndReason枚举类型，如下所示：

typedef enum {
    kEndReasonWin,
    kEndReasonLose,
    kEndReasonDisconnect
} EndReason;


//　　接下来，你需要为每个消息定义相应的结构体类型。因此，在HelloWorldLayer.h里面添加下面的代码：


typedef enum {
    kMessageTypeRandomNumber =0,
    kMessageTypeGameBegin,
    kMessageActiveGetOtherPlayerData,
    kMessageTypeMove,
    kMessageTypeGameOver
} MessageType;

typedef struct {
    MessageType messageType;
} Message;
//-------------------------------------------NewWork MessageData--------------------------------------/
typedef struct {
    Message message;
    uint32_t randomNumber;
} MessageRandomNumber;

typedef struct {
    Message message;
} MessageGameBegin;

typedef struct {
    Message message;
    int Action[5];
    bool skill[5];
    int data[32];
} MessageMove;

typedef struct {
    Message message;
    BOOL player1Won;
} MessageGameOver;

typedef struct {
    Message message;
    char PetName[32];
    int petId;
    int petlevel;
    int skill[2];
} MessageGetOherPlayerData;

@interface CGHelp_ios : NSObject<GCHelperDelegate>
{

    BOOL isPlayer1;
    GameState gameState;
    
    uint32_t ourRandom;
    BOOL receivedRandom;
    NSString *otherPlayerID;
}
- (void)setGameState:(GameState)state;
- (void)findPlayer:(NSString*) playerNickName;
- (void)matchStarted;
- (void)matchEnded;
- (void)match:(GKMatch *)match didReceiveData:(NSData *)data fromPlayer:(NSString *)playerID;
- (void)SendMoveAction;
+ (CGHelp_ios *)sharedInstance;
@end
