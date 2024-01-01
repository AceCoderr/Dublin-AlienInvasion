#ifndef RENDERER_H
#define RENDERER_H


#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <sstream>
#include "ParticleSystem.h"
#include "Helper.h"
#include <GLFW/glfw3.h>

using namespace std;

class Renderer
{
public:

	ParticleSystem *particleSystem;
	GLuint shaderModelMat;
	GLuint shaderViewMat;
	GLuint shaderProjMat;
	GLuint shaderOrthoMat;
	GLuint colourRedVar;
	GLuint colourGreenVar;
	GLuint colourBlueVar;
	GLuint pFixed;
	glm::mat4 perspective;
	glm::mat4 ortho;
	int numFrames;
	int lastTime;

	void setShaders();
	void renderParticles();
};

#endif
