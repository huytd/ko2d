#include "Texture.h"

Texture::Texture(void)
{
}


Texture::~Texture(void)
{
}

bool Texture::init(const char* fileName, const char* tiling)
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	int imgWidth, imgHeight, imgBpp;
	char* fileToRead = (char*)fileName;
	#ifdef _ANDROID
	LOGI("\nAndroid detected\n");
	fileToRead[strlen(fileToRead)-1] = '\0';
	#endif
	char* imgData = LoadTGA(fileToRead, &imgWidth, &imgHeight, &imgBpp);
	LOGI("Texture %s loaded\nWidth: %d\nHeight: %d\nBpp: %d\n", fileName, imgWidth, imgHeight, imgBpp);
	glTexImage2D(GL_TEXTURE_2D, 0, (imgBpp==32)?GL_RGBA:GL_RGB, imgWidth, imgHeight, 0, (imgBpp==32)?GL_RGBA:GL_RGB, GL_UNSIGNED_BYTE, imgData);

	if (tiling == "REPEAT")
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	}
	else
	//if (tiling == "CLAMP")
	{
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	}	
	
	// not using mipmap
	/*glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);*/
	// using mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] imgData;
	return true;
}