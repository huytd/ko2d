#pragma once

#include "Singleton.h"
#include "Engine.h"

class Graphic : public Singleton<Graphic>
{
public:
	Graphic(void);
	virtual ~Graphic(void);
	GLuint LoadProgram ( GLuint vertexShader, GLuint fragmentShader );
	GLuint LoadShader ( GLenum type, const char * filename );
};

