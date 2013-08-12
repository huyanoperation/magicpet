//
//  magicpetAppController.h
//  magicpet
//
//  Created by jia huyan on 11-11-21.
//  Copyright 道锋互动（北京）科技有限责任公司 2011年. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
}

@end

