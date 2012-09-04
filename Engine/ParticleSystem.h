#pragma once
#include "Singleton.h"
#include "Particle.h"
#include <vector>
using namespace std;

class ParticleSystem: public Singleton<ParticleSystem>
{
public:
	ParticleSystem(void);
	~ParticleSystem(void);

	vector<Particle*> particles;
	void Update(float deltaTime);
	void Draw();
	void clear();
	void addParticle(Particle* obj);
};

