#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(void)
{
	
}


ParticleSystem::~ParticleSystem(void)
{
}

void ParticleSystem::Update(float deltaTime)
{
}

void ParticleSystem::clear()
{
	particles.clear();
}

void ParticleSystem::Draw()
{
	vector<Particle*>::iterator i = particles.begin();
	while (i != particles.end())
	{
		LOGI("\nIs Finished: %d", (*i)->isFinished);
		if (!(*i)->isFinished || (*i)->isPlaying)
		{
			(*i)->Draw();
			++i;
		}
		else
		{
			i = particles.erase(i);
		}
	}
}

void ParticleSystem::addParticle(Particle* obj)
{
	particles.push_back(obj);
}