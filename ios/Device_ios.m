//
//  CCUtility_ios.m
//
//  Created by huyan on 11-5-16.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "Device_ios.h"
#import "GameKit/GameKit.h"
#import "Reachability.h"
#import <sys/utsname.h>


#define HOST_ADDRESS @""
#define LOCAL_HOST_ADDRESS @""

static id s_instanceUtil = NULL;

@implementation DeviceDetection

-(NSString*) GetUDID
{
    UIDevice* device = [UIDevice currentDevice];
    NSString *uid = [device uniqueIdentifier];
    
//    NSLog(@"uid: %@", uid);
    
    return uid;
//    return [[UIDevice currentDevice] uniqueIdentifier];
}



//-(void) CheckNetWorkStatus:(BOOL)localhost
//{
// 
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(NetworkStatusNotice:) name:kReachabilityChangedNotification object:nil];
//    
//    internetReach = [[Reachability reachabilityForInternetConnection] retain];
//    [internetReach startNotifier];
//    if (localhost) 
//    {
//            hostReach = [[Reachability reachabilityWithHostName:LOCAL_HOST_ADDRESS]retain];
//    }
//    else
//    {
//            hostReach = [[Reachability reachabilityWithHostName:HOST_ADDRESS]retain];
//    }
//
//    [hostReach startNotifier];
//    
//}
//
//-(void) NetworkStatusNotice:(NSNotification *)notice
//{
//    // called after network status changes
//    NetworkStatus internetStatus = [internetReach currentReachabilityStatus];
//    
//    switch (internetStatus) {
//        case NotReachable:
//        {
//            NSLog(@"The Internet is down");
//            internetActive = NO;
//        }
//            break;
//        case ReachableViaWiFi:
//        {
//            NSLog(@"The internet is working via WIFI");
//            internetActive = YES;
//        }
//            break;
//        case ReachableViaWWAN:
//        {
//            NSLog(@"The internet is working via WWAN");
//            internetActive = YES;
//        }
//            break;
//    }
// 
//    
//    NetworkStatus hostStatus = [hostReach currentReachabilityStatus];
//    
//    switch (hostStatus) {
//        case NotReachable:
//        {
//            NSLog(@"A gateway to the host is down");
//            hostActive = NO;
//        }
//            break;
//        case ReachableViaWiFi:
//        {
//            NSLog(@"A gateway to the host is working via WIFI");
//            hostActive = YES;
//        }
//            break;
//        case ReachableViaWWAN:
//        {
//            NSLog(@"A gateway to the host is working via WWAN");
//            hostActive = YES;
//        }
//            break;
//    }
//    
//}
//
//
//-(BOOL) GetNetworkStatus
//{
//    return internetActive;
//}
//
//-(BOOL) GetHostStatus
//{
//    return hostActive;
//}



- (uint) detectDevice 
{
    NSString *model= [[UIDevice currentDevice] model];
    struct utsname u;
	uname(&u);
    
    NSString *iPad = @"iPad";
    
    NSString *iPadSimulator = @"iPad Simulator";
    NSString *iPhoneSimulator = @"iPhone Simulator";

    if( [model compare:iPhoneSimulator] == NSOrderedSame )
    {
        return MODEL_IPHONE_SIMULATOR;
    }
    else if([model compare:iPadSimulator] == NSOrderedSame)
    {
        return MODEL_IPAD_SIMULATOR;
    }
    else if( [model compare:iPad] == NSOrderedSame  )
    {
        return MODEL_IPAD;
    }
   
    
    if (!strcmp(u.machine, "iPhone1,1")) {
		return MODEL_IPHONE;
	} else if (!strcmp(u.machine, "iPhone1,2")){
		return MODEL_IPHONE_3G;
	} else if (!strcmp(u.machine, "iPhone2,1")){
		return MODEL_IPHONE_3GS;
	} else if (!strcmp(u.machine, "iPhone3,1")){
		return MODEL_IPHONE_4;
	} else if (!strcmp(u.machine, "iPod1,1")){
		return MODEL_IPOD_TOUCH_GEN1;
	} else if (!strcmp(u.machine, "iPod2,1")){
		return MODEL_IPOD_TOUCH_GEN2;
	} else if (!strcmp(u.machine, "iPod3,1")){
		return MODEL_IPOD_TOUCH_GEN3;
    } else if (!strcmp(u.machine, "iPod4,1")){
		return MODEL_IPOD_TOUCH_GEN4;
	} else if (!strcmp(u.machine, "iPad1,1")){
		return MODEL_IPAD;
    }else if ( !strcmp(u.machine, "iPad1,2")){
        return MODEL_IPAD;
	} else if (!strcmp(u.machine, "i386")){
		//NSString *iPhoneSimulator = @"iPhone Simulator";
		NSString *iPadSimulator = @"iPad Simulator";
		if([model compare:iPadSimulator] == NSOrderedSame)
			return MODEL_IPAD_SIMULATOR;
		else
			return MODEL_IPHONE_SIMULATOR;
	}
	else {
		return MODEL_UNKNOWN;
	}
}

- (NSString *) returnDeviceName {
    NSString *returnValue = @"Unknown";
    
    switch ([[DeviceDetection sharedUtil] detectDevice])
	{
        case MODEL_IPHONE_SIMULATOR:
			returnValue = @"iPhone Simulator";
			break;
        case MODEL_IPAD_SIMULATOR:
            returnValue = @"iPad";
            break;
		case MODEL_IPOD_TOUCH_GEN1:
			returnValue = @"iPod Touch";
			break;
		case MODEL_IPOD_TOUCH_GEN2:
			returnValue = @"iPod Touch";
			break;
		case MODEL_IPOD_TOUCH_GEN3:
			returnValue = @"iPod Touch";
            break;
        case MODEL_IPOD_TOUCH_GEN4:
            returnValue = @"iPod Touch";
			break;
		case MODEL_IPHONE:
			returnValue = @"iPhone";
			break;
		case MODEL_IPHONE_3G:
			returnValue = @"iPhone 3G";
			break;
		case MODEL_IPHONE_3GS:
			returnValue = @"iPhone 3GS";
			break;
		case MODEL_IPHONE_4:
			returnValue = @"iPhone 4";
			break;
		case MODEL_IPAD:
			returnValue = @"iPad";
			break;
		default:
			break;
	}
	return returnValue;
}


////////////////////////////////////////////////////////////
+(id) sharedUtil
{
    if (s_instanceUtil == nil)
    {
        s_instanceUtil = [ DeviceDetection new];
    }
    
    return s_instanceUtil;
}

+(void) destory
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    
    [s_instanceUtil release];
}


@end
