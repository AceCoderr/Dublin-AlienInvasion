#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <fstream>
#include <glad.h>
#include "glm\glm.hpp"

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma);
void mapTexture(unsigned int &tex, const char *name);
float randomFloatBetween(float low, float high);
float isInBetween(float _number);
string loadShader(const string &file_name);
glm::vec3 computeVelocity(glm::vec3 u, glm::vec3 a, float t);
glm::vec3 computeEuler(glm::vec3 s, glm::vec3 u, glm::vec3 a, float t);

#endif
