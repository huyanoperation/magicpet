#ifndef __HELLOWORLD_FILE_OPERATION__
#define __HELLOWORLD_FILE_OPERATION__

#include <string>
#include "../GameConstant.h"

class FileOperation 
{
public:
	static void saveFile(const char *buf );
	static string readFile(void);
	static std::string getFilePath();
};

#endif
