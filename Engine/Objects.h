#pragma once

#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "Engine.h"
#include <math.h>

class Objects
{
	
public:
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
	int type;
	bool coll;
	Matrix mvpMatrix;
	float getDeltaTime;
	float speed; // speed of object
	GLuint modelID;
	int modelVerNum;
	GLuint textureID;
	GLuint shaderID;
	frameModel * frM, frame; // get values max vertex and min vertex of Model
	void UpdateFrame( void ); // change values max vertex and min vertex of Model at present time
	Objects(void);
	~Objects(void);
	virtual void init();
	void init(int _model, int _texture, int _shader);
	virtual void Draw();
	virtual void Update(float deltaTime);
	//void Destroy();

};

