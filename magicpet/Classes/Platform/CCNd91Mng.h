//
//  CCNd91Mng.h
//  magicpet
//
//  Created by jia huyan on 12-3-22.
//  Copyright (c) 2012年 道锋互动（北京）科技有限责任公司. All rights reserved.
//


#pragma once


#include "CCPlatformConfig.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "../../ios/CNd91Mng_ios.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "../../win32/CNd91Mng_win32.h"
#endif