#pragma once
#include "Engine.h"
#include "TGA.h"

class Texture
{
public:
	GLuint texID;
	Texture(void);
	~Texture(void);
	bool init(const char* fileName, const char* tiling);
};

