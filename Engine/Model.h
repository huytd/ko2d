#pragma once
#include "Engine.h"
#include "Vertex.h"
#include <fstream>
#include <string>
using namespace std;

struct FACE_LINE
{
	int v1; int t1; int n1;
	int v2; int t2; int n2;
	int v3; int t3; int n3;
};

struct frameModel
{
	float max_x;
	float max_y;
	float max_z;
	float min_x;
	float min_y;
	float min_z;
};
class Model
{
public:
	GLuint vboID;
	int numCount;
	frameModel frM;
	Model(void);
	~Model(void);
	bool init(const char* fileName);
};

