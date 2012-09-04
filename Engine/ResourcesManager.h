#pragma once
#include "Engine.h"
#include "Singleton.h"
#include <string>
#include <fstream>
#include <vector>
#include "Texture.h"
#include "Model.h"
#include "Shaders.h"

using namespace std;

class ResourcesManager: public Singleton<ResourcesManager>
{
private:
	Model*		_modelList;
	int			_modelCount;
	Texture*	_textureList;
	int			_textureCount;
	Shaders*	_shaderList;
	int			_shaderCount;
public:
	ResourcesManager(void);
	~ResourcesManager(void);
	void LoadResouces(const char* fileName);
	frameModel * getFrameModel ( int id );
	Model* getModel(int id);
	Texture* getTexture(int id);
	Shaders* getShader(int id);

	int loadingCount, loadingTotal;

};

