//
//  Nd91Mng.h
//
//  采用虚拟商店＋同支付模式，包含了漏单处理(漏单信息需要处理缓存机制的安全性！)
//
//  Created by xujianye on 11-11-8.
//  Copyright 2011 NetDragon WebSoft Inc. All rights reserved.
//
//技术支持咨询QQ：33583768、2449103724
//上架包提交QQ：2449103724
//技术支持QQ群： 160430278、130913993

#import <Foundation/Foundation.h>


@interface Nd91Mng : NSObject {
	NSString*	strSel_nextStep;
	NSString*	strProductId;
	int			nProductCount;

	NSString*	strFileForOrderInfo;
	NSString*	strLoginUin;
	NSString*	strLoginNickName;
	NSMutableDictionary*	dicAbnormalOrderInfo;
}

+ (id)sharedInstance;

- (void)checkNewVersion;

- (void)buyVirtualGoodsWithId:(NSString*)productId  count:(int)count;
- (void)checkAbnormalOrders;

- (void)login91Platform;
- (void)enter91Platform;	//如果未登录，会先引导登录
- (void)enterUserFeedBack;	//如果未登录，会先引导登录
- (void)enter91Recharge;	//如果未登录，会先引导登录
- (bool)isLogin;
- (void)upLoadLeaderBoard:(int)count : (int)Id;
- (void)NdUnLockAchievement : (int)achiveId;
- (NSString*)NdGetMyInfo;
-(void)NdGetUserLeaderBoardInfoList:(NSString*)uin : (int)LeadId;
-(void)getPortraitDidFinish;
//-(void)getLeaderBoardWithBoardIdDidFinish;
-(void)NdShareToThirdPlatform:(NSString*) nsStr : (NSString*) nsPath;
@end
