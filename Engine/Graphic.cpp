#include "Graphic.h"
#include "FileSystem.h"
#include <fstream>
using namespace std;

Graphic::Graphic(void)
{

}


Graphic::~Graphic(void)
{

}

char* readFileText(const char* fileName)
{
	FILE * pf = fopen(fileName, "rb" );
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

/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint Graphic::LoadShader ( GLenum type, const char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	return 0;

	char* fileToRead = (char*)filename;
	#ifdef _ANDROID
	LOGI("\nAndroid detected\n");
	fileToRead[strlen(fileToRead)-1] = '\0';
	LOGI("File to open: '%s' \n", fileToRead);
	LOGI("Last char: %d (%c)\n", fileToRead[strlen(fileToRead) - 1], fileToRead[strlen(fileToRead) - 1]);
	#endif

	char * shaderSrc = readFileText(fileToRead);
	LOGI("\n[Shader] File: %s", fileToRead);
	LOGI("\n[Shader] Content: \n%s\n", shaderSrc);

	glShaderSource ( shader, 1, (const char **)&shaderSrc, NULL );
	delete [] shaderSrc;

	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];


			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			LOGI ( "Error compiling shader:\n%s\n", infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}

	return shader;
}

/// \brief Create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure

GLuint Graphic::LoadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];


			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			LOGI ( "Error linking program:\n%s\n", infoLog );            

			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}