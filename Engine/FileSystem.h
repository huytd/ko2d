#pragma once
#include <cstring>
#include <string>
#include <sstream>
#include <fstream>
#include "Singleton.h"

class FileSystem : public Singleton<FileSystem>
{
	
public:
	std::string basePath;	
	FileSystem(void);
	~FileSystem(void);		
	bool Init(const char *);
	char* GetContent(const char *);
	std::string GetPath(const char *);	
};

