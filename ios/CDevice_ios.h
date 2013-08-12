//
//  CDevice.h
//  magicpet
//
//  Created by jia huyan on 11-11-23.
//  Copyright (c) 2011年 道锋互动（北京）科技有限责任公司. All rights reserved.
//

#include <string>

using namespace std;

class CDevice
{
public:
    static string GetUDID();  
    
    static bool FileExist(const char* filePaht);
    
    static double GetAvaliableMemory();
    
    static string GetDeviceType();
    
    static string strToUTF8(const char* u);
    
	static int GetLocalID();
    
    static int CheckNet();
    
	static int m_localID;
};