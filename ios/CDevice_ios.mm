//
//  CDevice.m
//  magicpet
//
//  Created by jia huyan on 11-11-23.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#import "CDevice_ios.h"

#include "Device_ios.h"

#import <mach/mach.h>

#include "CCApplication.h"

#import "ReachabilityAppDelegate.h"

string CDevice::GetUDID()
{
    
#ifndef CREATE_ROBOTER
    NSString* ns = [[NSString alloc ] init];
    
    ns = [[DeviceDetection  sharedUtil] GetUDID];
    
    string s ;
    
    s = [ns cStringUsingEncoding:[NSString defaultCStringEncoding]];    
    
    return s;
#else
    return "20110815-inferno-06";
#endif
}


bool CDevice::FileExist(const char *filePath)
{
    NSString* path = [NSString stringWithUTF8String:filePath];
    return [[NSFileManager defaultManager] fileExistsAtPath:path];
}

int CDevice::CheckNet()
{
    //check net state init
    if( nil == g_pReachabilityAppDelegate )
    {
        g_pReachabilityAppDelegate = [[ReachabilityAppDelegate alloc] init];
        [ g_pReachabilityAppDelegate start ];
    }
    
    int type = [g_pReachabilityAppDelegate getNetType];

    return type;
}

double CDevice::GetAvaliableMemory()
{
    double mem = 0;
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infocount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infocount);
    
    if(kernReturn != KERN_SUCCESS)
    {
        return NSNotFound;
    }
    
    mem = (double) ((vm_page_size * vmStats.free_count) / 1024.0)/1024.0;
    
    double activeMem = (double) ((vm_page_size * (vmStats.active_count + vmStats.inactive_count + vmStats.wire_count)) / 1024.0)/1024.0;
    
    NSLog(@"[System]:Avaliable Memory:%f,%f",mem,activeMem);  
    if(mem < 10)
    {
        NSLog(@"[System]:%f,%f, the memory is not enough!",mem,activeMem);
    }
    
    return mem;
    
}

string CDevice::GetDeviceType() 
{
    return [[[DeviceDetection sharedUtil] returnDeviceName] UTF8String];
}


string CDevice::strToUTF8(const char *u)
{
    NSString *str = [NSString stringWithUTF8String:u];
    
    string s = [str UTF8String];
    
    return s;
    
}