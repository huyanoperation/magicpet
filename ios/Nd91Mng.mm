//
//  Nd91Mng.m
//  Nd_SNS_Demo
//
//  Created by xujianye on 11-11-8.
//  Copyright 2011 NetDragon WebSoft Inc. All rights reserved.
//

#import "Nd91Mng.h"
#import <NdComPlatform/NdComPlatform.h>
#import <NdComPlatform/NdCPNotifications.h>
#import <NdComPlatform/NdComPlatformAPIResponse.h>
#import <NdComPlatform/NdComPlatform+VirtualGoods.h>
#import <NdComPlatform/NdComPlatformAPIResponse+VirtualGoods.h>
#import <NdComPlatform/NdComPlatformAPIResponse.h>
#import "CNd91Mng_ios.h"

#define	KEY_91_NICKNAME			@"KEY_91_NICKNAME"
#define	KEY_91_ORDERS_LIST		@"KEY_91_ORDERS_LIST"
#define	KEY_91_PRODUCT_ID		@"KEY_91_PRODUCT_ID"
#define	KEY_91_PRODUCT_COUNT	@"KEY_91_PRODUCT_COUNT"
/*
 dicAbnormalOrderInfo结构
 key(uin)  value(dic):
					key(KEY_91_NICKNAME)    value(nickName)
					key(KEY_91_ORDERS_LIST) value(dic):
													key(KEY_91_PRODUCT_ID)		value(productId)
													key(KEY_91_PRODUCT_COUNT)	value(productCount)
 
 */

#define FILE_NAME_FOR_ORDER		@"abnormalOrderInfo_nd"
#define KEY_91_FILE_FOR_ORDER	@"KEY_91_FILE_FOR_ORDER"


@interface Nd91Mng()

@property (nonatomic, retain) 	NSString*	strSel_nextStep;
@property (nonatomic, retain) 	NSString*	strProductId;
@property (nonatomic, assign) 	int			nProductCount;
@property (nonatomic, retain) 	NSString*	strLoginUin;
@property (nonatomic, retain) 	NSString*	strLoginNickName;
@property (nonatomic, retain) 	NSMutableDictionary*	dicAbnormalOrderInfo;
@property (nonatomic, retain) 	NSString*	strFileForOrderInfo;

- (void)doNextOperationIfNeed;
- (void)buyVirtualGoods;
- (void)startWaitingView;
- (void)stopWaitingView;
- (void)isNotBeginner:(NSString*)errorTip;

- (void)willPayWithOrderInfo:(NSString*)orderSerial  productId:(NSString*)productId  productCount:(int)productCount;
- (void)didPayWithOrderInfo:(NSString*)orderSerial  success:(BOOL)bSuccess;

- (NSString*)fileForAbnormalOrderInfo;
- (void)saveOrderInfo;
- (BOOL)isFileValidForAbnormalOrderInfo;
- (void)loadAbnormalOrderInfo;
- (void)showTipForAbnormalOrderInfo;

- (void)appendOrderInfo:(NSString*)orderSerial  productId:(NSString*)productId  productCount:(int)count;
- (void)removeOrderInfo:(NSString*)orderSerail;

- (void)userDidPaySuccessWithProduct:(NSString*)productId  productCount:(int)productCount;

@end


@implementation Nd91Mng
@synthesize strSel_nextStep;
@synthesize strProductId;
@synthesize nProductCount;
@synthesize strLoginUin, strLoginNickName;
@synthesize dicAbnormalOrderInfo;
@synthesize strFileForOrderInfo;

+ (id)sharedInstance
{
	static id s_91Mng = nil;
	if (nil == s_91Mng) {
		s_91Mng = [[self alloc] init];
	}
	return s_91Mng;
}

- (id)init
{
	self = [super init];
	if (self) {

		//TODO: 使用你的 appId & appKey
		[[NdComPlatform defaultPlatform] setAppId:108355];
		[[NdComPlatform defaultPlatform] setAppKey:@"b116082f5b2fc501fc5b3c3a8cfbcd95a3b7e932d92c6ce7"];

		//设定平台为横屏或者竖屏
		[[NdComPlatform defaultPlatform] NdSetScreenOrientation:UIInterfaceOrientationLandscapeRight];	
		
		//设置是否自动旋转
		[[NdComPlatform defaultPlatform] NdSetAutoRotation:YES];
		//设置升级，支付为测试模式，release版本请注释掉这句调用
#if COCOS2D_DEBUG
		[[NdComPlatform defaultPlatform] NdSetDebugMode:0];
#endif
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SNSLoginResult:) name:(NSString *)kNdCPLoginNotification object:nil];
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(SNSBuyResult:) name:kNdCPBuyResultNotification object:nil];	
	}
	return self;
}

- (void)dealloc
{
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	self.strSel_nextStep = nil;
	self.strProductId = nil;
	self.strFileForOrderInfo = nil;
	self.strLoginUin = nil;
	self.strLoginNickName = nil;
	self.dicAbnormalOrderInfo = nil;
	[super dealloc];
}

- (void)startWaitingView
{
	//TODO: 加入你的卡屏界面，开始等待
//#error 开始界面等待
}

- (void)stopWaitingView
{
	//TODO: 移除你的卡屏界面，结束等待
//#error 停止界面等待
}

- (void)isNotBeginner:(NSString*)stringTip
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:stringTip
													message:nil
												   delegate:nil
										  cancelButtonTitle:nil
										  otherButtonTitles:@"确定", nil];
	[alert show];
	[alert release];
}

- (void)doNextOperationIfNeed
{
	if ([strSel_nextStep length] > 0) {
		[self performSelector:NSSelectorFromString(strSel_nextStep)];
		self.strSel_nextStep = nil;
	}
}

- (void)checkNewVersion
{
	int nRes = [[NdComPlatform defaultPlatform] NdAppVersionUpdate:0 delegate:self];
	if (nRes < 0) {
		NSLog(@"check version error = %d", nRes);
	}
}

/**
 @brief !!!:新版本检查结果
 */
- (void)appVersionUpdateDidFinish:(ND_APP_UPDATE_RESULT)updateResult
{
	NSString *title = nil;
	switch (updateResult) {
		case ND_APP_UPDATE_NO_NEW_VERSION:
			title = @"无可用更新";
			break;
		case ND_APP_UPDATE_FORCE_UPDATE_CANCEL_BY_USER:
			title = @"下载强制更新被取消";
			break;
		case ND_APP_UPDATE_NORMAL_UPDATE_CANCEL_BY_USER:
			title = @"下载普通更新被取消";
			break;
		case ND_APP_UPDATE_NEW_VERSION_DOWNLOAD_FAIL:
			title = @"下载新版本失败";
			break;
		case ND_APP_UPDATE_CHECK_NEW_VERSION_FAIL:
			title = @"检测新版本信息失败";
			break;
		default:
			break;
	}
	
	NSLog(@"check version result = %@", title);
	
	if (ND_APP_UPDATE_FORCE_UPDATE_CANCEL_BY_USER == updateResult) {
		//TODO: 用户取消了强制更新，默认退出应用。
		exit(333);
	}
	[self loadAbnormalOrderInfo];
	[self showTipForAbnormalOrderInfo];
}

- (void)enter91Platform
{
	if ([[NdComPlatform defaultPlatform] isLogined]) {
		[[NdComPlatform defaultPlatform] NdEnterPlatform:0];
	}
	else {
		[[NdComPlatform defaultPlatform] NdLogin:0];
		self.strSel_nextStep = @"enter91Platform";
	}
}

- (void)buyVirtualGoodsWithId:(NSString*)productId  count:(int)count
{
	if ([productId length] <= 0 || count < 0) {
		return ;
	}
	
	self.strProductId = productId;
	self.nProductCount = count;
	[self buyVirtualGoods];
}

- (void)buyVirtualGoods
{
	if ([[NdComPlatform defaultPlatform] isLogined]) {
		int nRes = [[NdComPlatform defaultPlatform] NdGetCommodityInfo:strProductId delegate:self];
		if (nRes < 0) {
			[self isNotBeginner:@"请求商品信息失败"];
		}
		else {
			[self startWaitingView];
		}
	}
	else {
		[[NdComPlatform defaultPlatform] NdLogin:0];
		self.strSel_nextStep = @"buyVirtualGoods";
	}
}

- (void)getCommodityInfoDidFinish:(int)error commodityInfo:(NdVGCommodityInfo*)commodityInfo
{
	BOOL bNeedStopWaiting = YES;
	if (error >= 0 && commodityInfo) {
		CFUUIDRef theUUID = CFUUIDCreate(NULL); 
		CFStringRef guid = CFUUIDCreateString(NULL, theUUID); 
		CFRelease(theUUID); 
		NSString *uuidString = [((NSString *)guid) stringByReplacingOccurrencesOfString:@"-" withString:@""];
		CFRelease(guid);
		
		NdBuyInfo *buyInfo = [[NdBuyInfo new] autorelease];
		buyInfo.cooOrderSerial = [uuidString lowercaseString];
		buyInfo.productId = commodityInfo.strProductId;
		buyInfo.productName = commodityInfo.strProductName;
		buyInfo.productPrice = [commodityInfo.strSalePrice floatValue];
		buyInfo.productOrignalPrice = [commodityInfo.strOriginPrice floatValue];
		buyInfo.productCount = self.nProductCount;
		buyInfo.payDescription = @"buy commodity";

		int nRes = [[NdComPlatform defaultPlatform] NdUniPay:buyInfo];
		if (nRes >= 0) {
			bNeedStopWaiting = NO;
			[self willPayWithOrderInfo:buyInfo.cooOrderSerial productId:buyInfo.productId  productCount:buyInfo.productCount];
		}
	}
	else {
		[self isNotBeginner:@"获取商品信息失败"];
	}
	if (bNeedStopWaiting) {
		[self stopWaitingView];
	}
}


- (void)willPayWithOrderInfo:(NSString*)orderSerial  productId:(NSString*)productId  productCount:(int)productCount
{
	[self appendOrderInfo:orderSerial productId:productId productCount:productCount];
}

- (void)didPayWithOrderInfo:(NSString*)orderSerial  success:(BOOL)bSuccess
{
	[self removeOrderInfo:orderSerial];
	if (bSuccess) {
		[self userDidPaySuccessWithProduct:self.strProductId productCount:self.nProductCount];
	}
}

- (NSString*)fileForAbnormalOrderInfo
{
	if ([strFileForOrderInfo length] <= 0) {
		NSString* strFilePath = nil;
		NSArray *documentArr = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		if ([documentArr count] > 0) {
			strFilePath = [documentArr objectAtIndex:0];
		}
		else {
			strFilePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"Documents"];
			if (![[NSFileManager defaultManager] fileExistsAtPath:strFilePath]) {
				[[NSFileManager defaultManager] createDirectoryAtPath:strFilePath withIntermediateDirectories:YES attributes:nil error:NULL];
			}
		}
		self.strFileForOrderInfo = [strFilePath stringByAppendingPathComponent:FILE_NAME_FOR_ORDER];
	}
	return strFileForOrderInfo;
	
}

- (void)saveOrderInfo
{
	//TODO: 使用你自己的缓存机制
//#error 使用你自己的缓存机制
	NSString* strFile = [self fileForAbnormalOrderInfo];
	[self.dicAbnormalOrderInfo writeToFile:strFile atomically:YES];
	NSDictionary* dicAttr = [[NSFileManager defaultManager] attributesOfItemAtPath:strFile error:NULL];
	if (dicAttr) {
		NSDate* dateMod = [dicAttr objectForKey:NSFileModificationDate];
		NSTimeInterval time = [dateMod timeIntervalSince1970];
		NSString* str = [NSString stringWithFormat:@"%.2f", time];
		[[NSUserDefaults standardUserDefaults] setObject:str forKey:KEY_91_FILE_FOR_ORDER];
	}
}

- (BOOL)isFileValidForAbnormalOrderInfo
{
	//TODO: 检查你的缓存文件是否被篡改
//#error 检查你的缓存文件是否被篡改
	NSString* strFile = [self fileForAbnormalOrderInfo];
	NSDictionary* dicAttr = [[NSFileManager defaultManager] attributesOfItemAtPath:strFile error:NULL];
	if (dicAttr) {
		NSDate* dateMod = [dicAttr objectForKey:NSFileModificationDate];
		NSTimeInterval time = [dateMod timeIntervalSince1970];
		NSString* str = [NSString stringWithFormat:@"%.2f", time];
		NSString* strOld = [[NSUserDefaults standardUserDefaults] objectForKey:KEY_91_FILE_FOR_ORDER];
		return [str isEqualToString:strOld];
	}
	return NO;
}

- (void)loadAbnormalOrderInfo
{
	//TODO: 如果缓存文件未被篡改，使用你的载入方式；否则，丢弃这些数据
//#error 如果缓存文件未被篡改，使用你的载入方式；否则，丢弃这些数据
	if ([self isFileValidForAbnormalOrderInfo]) {
		NSDictionary* dic = [NSDictionary dictionaryWithContentsOfFile:[self fileForAbnormalOrderInfo]];
		self.dicAbnormalOrderInfo = [NSMutableDictionary dictionaryWithDictionary:dic];
	}
	else {
		self.dicAbnormalOrderInfo = [NSMutableDictionary dictionaryWithCapacity:1];
	}
}

- (void)showTipForAbnormalOrderInfo
{
	//如果有漏单处理，弹出提示，引导用户登录指定的账号
	NSMutableString* strNames = [NSMutableString stringWithCapacity:1];
	
	NSMutableDictionary* dicCopyTmp = nil;
	for (NSString* uin in dicAbnormalOrderInfo) {
		NSMutableDictionary* dicUser = [dicAbnormalOrderInfo objectForKey:uin];
		NSMutableDictionary* dicOrders = [dicUser objectForKey:KEY_91_ORDERS_LIST];
		if ([dicOrders count] > 0) {
			NSString* strFmt = [strNames length] > 0 ? @", %@" : @"%@";
			[strNames appendFormat:strFmt, [dicUser objectForKey:KEY_91_NICKNAME]];
		}
		else {
			if (nil == dicCopyTmp) {
				dicCopyTmp = [[dicAbnormalOrderInfo mutableCopy] autorelease];
			}
			[dicCopyTmp removeObjectForKey:uin];
		}
	}

	if (dicCopyTmp) {
		self.dicAbnormalOrderInfo = dicCopyTmp;
		[self saveOrderInfo];
	}
	
	if ([strNames length] > 0) {
		NSString* str = [NSString stringWithFormat:@"账号(%@)在上次购买时发生异常，重新登录即可消除异常", strNames];
		UIAlertView* alert = [[[UIAlertView alloc] initWithTitle:nil
														message:str
													   delegate:self
											  cancelButtonTitle:@"取消"
											   otherButtonTitles:@"登录", nil] autorelease];
		[alert show];
	}
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
	if (buttonIndex != alertView.cancelButtonIndex) {
		[[NdComPlatform defaultPlatform] NdLogin:0];
	}
}

- (void)appendOrderInfo:(NSString*)orderSerial  productId:(NSString*)productId  productCount:(int)count
{
	if ([orderSerial length] <= 0 || [productId length] <= 0) {
		return;
	}
	
	if (nil == dicAbnormalOrderInfo) {
		self.dicAbnormalOrderInfo = [NSMutableDictionary dictionaryWithCapacity:1];
	}
	NSMutableDictionary* dicUser = [dicAbnormalOrderInfo objectForKey:self.strLoginUin];
	if (nil == dicUser) {
		dicUser = [NSMutableDictionary dictionaryWithCapacity:2];
		[dicAbnormalOrderInfo setObject:dicUser forKey:self.strLoginUin];
	}
	[dicUser setObject:self.strLoginNickName forKey:KEY_91_NICKNAME];

	NSMutableDictionary* dicOrders = [dicUser objectForKey:KEY_91_ORDERS_LIST];
	if (nil == dicOrders) {
		dicOrders = [NSMutableDictionary dictionaryWithCapacity:1];
		[dicUser setObject:dicOrders forKey:KEY_91_ORDERS_LIST];
	}
	NSDictionary* dicOrderInfo = [NSDictionary dictionaryWithObjectsAndKeys:
								  productId, KEY_91_PRODUCT_ID,
								  [NSNumber numberWithInt:count], KEY_91_PRODUCT_COUNT,
								  nil];
	[dicOrders setObject:dicOrderInfo forKey:orderSerial];
	
	[self saveOrderInfo];
}

- (void)removeOrderInfo:(NSString*)orderSerail
{
	if ([orderSerail length] <= 0) {
		return;
	}
	NSMutableDictionary* dicUser = [dicAbnormalOrderInfo objectForKey:self.strLoginUin];
	NSMutableDictionary* dicOrders = [dicUser objectForKey:KEY_91_ORDERS_LIST];
	[dicOrders removeObjectForKey:orderSerail];
	[self saveOrderInfo];
}

- (void)userDidPaySuccessWithProduct:(NSString*)productId  productCount:(int)productCount
{
	//TODO: 刷新你的道具信息（包含了正常购买的结果和漏单处理的结果）
//#error 购买成功，刷新你的道具
	NSLog(@"userDidPaySuccessWithProductId = %@, count = %d", productId, productCount);
}


- (void)checkAbnormalOrders
{
	if (![[NdComPlatform defaultPlatform] isLogined]) {
		return;
	}
	
	NSMutableDictionary* dicUser = [dicAbnormalOrderInfo objectForKey:self.strLoginUin];
	NSMutableDictionary* dicOrders = [dicUser objectForKey:KEY_91_ORDERS_LIST];
	if ([dicOrders count] > 0) {
		NSMutableDictionary* dicOrdersTmp = [[dicOrders mutableCopy] autorelease];
		for (NSString* order in dicOrdersTmp) {
			[[NdComPlatform defaultPlatform] NdCheckPaySuccess:order delegate:self];
		}
	}
}

- (void)checkPaySuccessDidFinish:(int)error cooOrderSerial:(NSString*)cooOrderSerial bSuccess:(BOOL)bSuccess
{
	if (error >= 0) {
		NSMutableDictionary* dicUser = [dicAbnormalOrderInfo objectForKey:self.strLoginUin];
		NSMutableDictionary* dicOrders = [dicUser objectForKey:KEY_91_ORDERS_LIST];
		NSMutableDictionary* dicOrderInfo = [dicOrders objectForKey:cooOrderSerial];
		NSString* productId = [dicOrderInfo objectForKey:KEY_91_PRODUCT_ID];
		int nCount = [[dicOrderInfo objectForKey:KEY_91_PRODUCT_COUNT] intValue];
		if ([productId length] > 0 && nCount > 0) {
			[self userDidPaySuccessWithProduct:productId productCount:nCount];
		}
		
		[self removeOrderInfo:cooOrderSerial];
	}
	else if (ND_COM_PLATFORM_ERROR_PAY_ORDER_NOT_EXIST == error) {
		[self removeOrderInfo:cooOrderSerial];
	}
}


/**
 @brief !!!:登录结果
 */
- (void)SNSLoginResult:(NSNotification *)notify
{
	NSDictionary *dict = [notify userInfo];
	BOOL success = [[dict objectForKey:@"result"] boolValue];
	
	//登录成功后处理
	if([[NdComPlatform defaultPlatform] isLogined] && success) {
		self.strLoginUin = [[NdComPlatform defaultPlatform] loginUin];
		self.strLoginNickName = [[NdComPlatform defaultPlatform] nickName];
		[self doNextOperationIfNeed];
		[self checkAbnormalOrders];
	}
	//登录失败处理和相应提示
	else {
		//TODO: 登录失败的处理
		int error = [[dict objectForKey:@"error"] intValue];
		NSString* strTip = [NSString stringWithFormat:@"登录失败, error=%d", error];
		switch (error) {
			case ND_COM_PLATFORM_ERROR_USER_CANCEL://用户取消登录
				strTip = @"用户未登录";
				break;
				
				// {{ 验证appId， appKey是否有效，如果成功登录过，可以删除这两个情况
			case ND_COM_PLATFORM_ERROR_APP_KEY_INVALID://appId未授权接入, 或appKey 无效
				strTip = @"登录失败, 请检查appId/appKey";
				break;
			case ND_COM_PLATFORM_ERROR_CLIENT_APP_ID_INVALID://无效的应用ID
				strTip = @"登录失败, 无效的应用ID";
				break;
				// }}
			default:
				break;
		}
	}
	self.strSel_nextStep = nil;
}

/**
 @brief !!!:购买结果
 */
- (void)SNSBuyResult:(NSNotification*)notify
{
	NSDictionary* dic = [notify userInfo];
	BOOL bSuccess = [[dic objectForKey:@"result"] boolValue];
	NSString* str = bSuccess ? @"购买成功" : @"购买失败";
	NdBuyInfo* buyInfo = (NdBuyInfo*)[dic objectForKey:@"buyInfo"];
	
	[self didPayWithOrderInfo:buyInfo.cooOrderSerial success:bSuccess];
	
	if (!bSuccess) {
		NSString* strError = nil;
		int nErrorCode = [[dic objectForKey:@"error"] intValue];
		switch (nErrorCode) {
			case ND_COM_PLATFORM_ERROR_USER_CANCEL:
				strError = @"用户取消操作";
				break;
			case ND_COM_PLATFORM_ERROR_NETWORK_FAIL:
				strError = @"网络连接错误";
				break;
			case ND_COM_PLATFORM_ERROR_SERVER_RETURN_ERROR:
				strError = @"服务端处理失败";
				break;
			default:
				strError = [NSString stringWithFormat:@"errorCode = %d", nErrorCode];
				break;
		}
		
		if ([strError length] > 0) {
			str = [str stringByAppendingFormat:@"\n%@", strError];
		}
		str = [str stringByAppendingFormat:@"\n<productId = %@, productCount = %d, cooOrderSerial = %@>", 
			   buyInfo.productId, buyInfo.productCount, buyInfo.cooOrderSerial];
		NSLog(@"%@", str);
	}
    else
    {
        string str = [buyInfo.productId UTF8String];
        CNd91Mng::BuyResult( str );
    }
}

- (bool)isLogin
{
    if ([[NdComPlatform defaultPlatform] isLogined]) {
    
        return true;
	}
    return false;
}

-(void)upLoadLeaderBoard :(int)count : (int)Id
{
    int nLeaderBoardId = Id;
    int nCurrentScore = count;
    NSString * displayText = nil;
    [[NdComPlatform defaultPlatform] NdSubmitScore:nLeaderBoardId
     //排行榜id //分值,不能是负数 //如果不想自定义文本传nil
                                             score:nCurrentScore displayText:displayText delegate:nil];
}

-(void) NdUnLockAchievement : (int)achiveId
{
    int nAchievementId = achiveId;
    int nCurrentValue = 100;
    
    NSString* displayText = nil;
    
    [[NdComPlatform defaultPlatform] NdUnLockAchievement: nAchievementId value:nCurrentValue displayText: displayText delegate: nil];
}

- (void)login91Platform
{
	if (![[NdComPlatform defaultPlatform] isLogined]) {
		[[NdComPlatform defaultPlatform] NdLogin:0];
		self.strSel_nextStep = nil;
	}
}

- (void)enterUserFeedBack
{
	if ([[NdComPlatform defaultPlatform] isLogined]) {
		[[NdComPlatform defaultPlatform] NdUserFeedBack];
	}
	else {
		[[NdComPlatform defaultPlatform] NdLogin:0];
		self.strSel_nextStep = @"enterUserFeedBack";
	}
}

- (void)enter91Recharge
{
	if ([[NdComPlatform defaultPlatform] isLogined]) {
		[[NdComPlatform defaultPlatform] NdEnterRecharge:0 content:nil];
	}
	else {
		[[NdComPlatform defaultPlatform] NdLogin:0];
		self.strSel_nextStep = @"enter91Recharge";
	}
}

- (NSString*)NdGetMyInfo
{
    NdMyUserInfo *info = [[NdComPlatform defaultPlatform] NdGetMyInfo];
    
    NSString *buffer = [NSString stringWithFormat:@"%@,%@",info.baseInfo.uin,info.baseInfo.nickName];
    
    NSLog(buffer);
    
    return buffer;
}

-(void)NdGetUserLeaderBoardInfoList :(NSString*)playeruin : (int)leadId
{
    //NdPagination* pagination = [[NdPagination new] autorelease];
    //pagination.pageIndex = 1;
    //pagination.pageSize = 10;
    //[[NdComPlatform defaultPlatform] NdGetAppUserList:pagination delegate:self];
    
    int nLeaderBoardId = leadId; //排行榜 id
    NSString* uin = playeruin; //参见该 api 头文件注释
    int userType = 0; //参见该 api 头文件注释
    int subType = 0; //参见该 api 头文件注释
    NdPagination* pagination = [[NdPagination new] autorelease];
    pagination.pageSize = 40; //5 的倍数,不要超过 50
    pagination.pageIndex = 1; //获取页的索引值,从 1 开始,总共的页数可以通过返回接口里
    //result.totalCount = 1; //字段计算出来,第一次请求用 1
    [[NdComPlatform defaultPlatform] NdGetUserLeaderBoardInfoList:uin leaderBoardId:nLeaderBoardId type:userType subType:subType
                                                       pagination:pagination delegate:self];
}

- (void)getLeaderBoardWithBoardIdDidFinish:(int)error
                     type:(NSUInteger)userType subType:(NSUInteger)subType result:(NdUserInfoOfLeaderBoardList*)userInfoOfLeaderBoardList
{
    if (error < 0)
    {
        //TODO: 下载列表信息失败 
        //NdComPlatformError;
        
        //[NdComPlatformError isLogin];
    }
    else 
    {
            //TODO: 处理信息列表
            //计算出总的页数,可以用来处理下一页的请求,如果nPageCount为0,可能需要特殊 处理
            //NdPagination *pagination = userInfoList.pagination;
            //int nPageCount = (userInfoOfLeaderBoardList.totalCount + pagination.pageSize - 1) / pagination.pageSize;
            
            //NSUInteger i = 0;
            //for (NdStrangerUserInfoList *info in userInfoOfLeaderBoardList.records) 
            //{
            //    NSString *strUin = [[[userInfoList.records objectAtIndex: i ] baseUserInfo] uin];
            //    NSString *strName = [[[userInfoList.records objectAtIndex: i ] baseUserInfo] nickName];
            //    i = i + 1;
            
            //    CNd91Mng::onGetPlayerInfoCallBack([strUin UTF8String], [strName UTF8String] );
            //}
        NdPagination* pagination = userInfoOfLeaderBoardList.pagination;
        int nPageCount = (userInfoOfLeaderBoardList.totalCount + pagination.pageSize - 1) /
        pagination.pageSize;
        //userInfoOfLeaderBoardList.myself //我自己的排行榜信息
         NSUInteger i = 0;
        for (NdUserInfoOfLeaderBoard* userInfo in userInfoOfLeaderBoardList.records) 
        {
             if (i >=[userInfoOfLeaderBoardList.records count ] )
             {
                 int c = 0;
                 
                 c++;
                 break;
                
             }
            
             NSString *strUin = [[userInfoOfLeaderBoardList.records objectAtIndex: i ] uin];
             NSString *strName = [[userInfoOfLeaderBoardList.records objectAtIndex: i ] nickName];
             CNd91Mng::onGetPlayerInfoCallBack([strUin UTF8String], [strName UTF8String] );
             i = i + 1;
        }
        CNd91Mng::onGetPlayerFinish();
    }
}

- (void)getPortraitDidFinish:(int)error uin:(NSString *)uin portrait:(UIImage *)portrait checkSum:(NSString *)checkSum
{
	if ([uin length] > 0 && portrait) 
    {
      
	}
	else 
    {
		NSLog(@"Demo UserList, error icon: %@, %@", uin, portrait);
	}
}

-(void)NdShareToThirdPlatform:(NSString*) nsStr : (NSString*) nsPath
{
    NSDate* date = [NSDate date];
    NSString* str = [NSString stringWithFormat:nsStr, [date description]]; [[NdComPlatform defaultPlatform] NdShareToThirdPlatform: str
                                                                                                                        imageInfo : [NdImageInfo imageInfoWithFile:nsPath] ] ;
}

@end
