#pragma once
#pragma warning( disable  : 4996 )
#include <random>

#include <iostream>
#include "glm.hpp"

#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

void saveImage(glm::vec3 image[], int size_2);

void printVec(glm::vec3 v);
void normalizeImage(glm::vec3 image[], float maxR, float maxG, float maxB, int imageSize);
float getRandomFloatInc();
float getRandomFloatAzi();
float getRandomsStepVal(int maxStep);

struct Camera
{
	glm::vec3 position = glm::vec3(-1.0f, 0.f, 0.f);
};
struct Light
{
	glm::vec3 position = glm::vec3(3.0f, 2.f, 0.f);
	glm::vec3 strength = glm::vec3(0.3, 0.3, 0.3);
};

#endif