#pragma once
#pragma warning( disable  : 4996 )
#include <random>

#include <iostream>
#include "glm.hpp"
#include <iostream>
#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#define EPSILON 0.00000000000000001
#define M_PI 3.14159265358979323846  /* pi */
struct Material;
struct Triangle;
//a few functions that just do some small things
void saveImage(glm::vec3 image[], int size_2);
void printVec(glm::vec3 v);
void normalizeImage(glm::vec3 image[], float maxR, float maxG, float maxB, int imageSize);
float getRandomFloatInc();
float getRandomFloatAzi();
float getRandomsStepVal(int maxStep);

bool castShadowRay(glm::vec3 * triangleHitPosition, Triangle *triangles,float &dimVal);

struct Camera
{
	glm::vec3 position = glm::vec3(-1.0f, 0.f, 0.f);
};
static struct Light
{
	static glm::vec3 getLightPosition() { return glm::vec3(3.0f, 0.f, 0.0f); }
	static glm::vec3 getLightstrength() { return glm::vec3(0.8f, 0.8f, 0.8f); }
	
};

//möller-trumbore
glm::vec3 triangleIntersect(glm::vec3* start, glm::vec3* dir, glm::vec3 &newStartPos, glm::vec3 &normal, Triangle *triangles, Material &material);
#endif