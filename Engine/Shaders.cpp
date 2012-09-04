#include "Shaders.h"
#include "Graphic.h"

int Shaders::Init(const char * fileVertexShader, const char * fileFragmentShader)
{
	vertexShader = Graphic::GetInstance()->LoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;
	LOGI("\nVertex shader %s loaded", fileVertexShader);

	fragmentShader = Graphic::GetInstance()->LoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}
	LOGI("\nFragment shader loaded");

	program = Graphic::GetInstance()->LoadProgram(vertexShader, fragmentShader);
	LOGI("\nProgram compiled with ID: %d", program);

	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}