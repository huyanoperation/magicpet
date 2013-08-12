//
//  CCUtility_ios.h
//  
//
//  Created by huyan on 11-5-16.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
enum {
    MODEL_UNKNOWN=0,/**< unknown model */
    MODEL_IPHONE_SIMULATOR,/**< iphone simulator */
    MODEL_IPAD_SIMULATOR,/**< ipad simulator */
    MODEL_IPOD_TOUCH_GEN1,/**< ipod touch 1st Gen */
    MODEL_IPOD_TOUCH_GEN2,/**< ipod touch 2nd Gen */
    MODEL_IPOD_TOUCH_GEN3,/**< ipod touch 3th Gen */
    MODEL_IPOD_TOUCH_GEN4,/**< ipod touch 4th Gen */
    MODEL_IPHONE,/**< iphone  */
    MODEL_IPHONE_3G,/**< iphone 3G */
    MODEL_IPHONE_3GS,/**< iphone 3GS */
    MODEL_IPHONE_4,	/**< iphone 4 */
	MODEL_IPAD/** ipad  */
};

//@"i386"      on the simulator
//@"iPod1,1"   on iPod Touch
//@"iPod2,1"   on iPod Touch Second Generation
//@"iPod3,1"   on iPod Touch Third Generation
//@"iPod4,1"   on iPod Touch Fourth Generation
//@"iPhone1,1" on iPhone
//@"iPhone1,2" on iPhone 3G
//@"iPhone2,1" on iPhone 3GS
//@"iPad1,1"   on iPad
//@"iPad2,1"   on iPad 2
//@"iPhone3,1" on iPhone 4
//@"iPhone4,1" on iPhone 4S


@class Reachability;
@interface DeviceDetection : NSObject
{
    Reachability* internetReach;
    Reachability* hostReach;
    
    BOOL    internetActive;
    BOOL    hostActive;
}

// member methord
-(NSString*) GetUDID;

//-(void) CheckNetWorkStatus:(BOOL)localhost;
//-(void) NetworkStatusNotice:(NSNotification *)notice;
//-(BOOL) GetNetworkStatus;
//-(BOOL) GetHostStatus;

- (uint) detectDevice;
- (NSString *) returnDeviceName;

// class methord
+(id) sharedUtil;
+(void) destory;

@end
