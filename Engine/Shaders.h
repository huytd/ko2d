#pragma once
#include "Engine.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	
	int Init(const char * fileVertexShader, const char * fileFragmentShader);
	~Shaders();
};