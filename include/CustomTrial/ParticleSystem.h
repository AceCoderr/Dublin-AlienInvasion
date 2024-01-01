#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include <vector>

class ParticleSystem
{
public:
	unsigned int particleVAO_ID;
	unsigned int parseTexture;
	unsigned int playerTex;
	unsigned int poinTex;

	// Particle System properties
	glm::vec3 gravity;
	int lastTime;
	float deltaTime;
	float friction;
	float invCount;

	std::vector<Particle> particles;
	bool isInitiated;

	ParticleSystem();
	void setParticles();
	void update();


};

#endif
