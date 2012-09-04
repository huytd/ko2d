#pragma once
#include "Objects.h"
#include "GL2D.h"

class Particle: public Objects
{
public:
	Particle(Vector3 pos, bool loop = false);
	~Particle(void);

	float timer;
	int texX, texY;
	bool isPlaying, isLoop, isFinished;

	void Draw();
};

