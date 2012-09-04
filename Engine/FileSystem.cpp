#include "FileSystem.h"
#include "Engine.h"

using namespace std;

FileSystem::FileSystem(void)
{
	basePath = "";
}


FileSystem::~FileSystem(void)
{
}

bool FileSystem::Init(const char *res)
{
	basePath = res;
	return true;
}

char* FileSystem::GetContent(const char *fileName)
{	
	string fullPath = GetPath(fileName);
	FILE * pf = fopen(fullPath.data(), "rb" );
	if (pf == 0)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * buffer = new char[size + 1];
	fread(buffer, sizeof(char), size, pf);
	buffer[size] = 0;
	fclose(pf);

	return buffer;
}


string FileSystem::GetPath(const char *fileName)
{
	string fullPath = basePath + "/";
	fullPath += fileName;
	return fullPath;
}