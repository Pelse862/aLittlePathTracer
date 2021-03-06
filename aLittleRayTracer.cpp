#pragma once

#include "SceneManager.h"
#include <random>
#include "Helpers.h"


const int C_MULTISAMPLE_COUNT = 10;
const int C_MAX_BOUNCE = 10;
const int C_IMAGE_SIDE = 256;
const int C_IMAGE_SIZE = (C_IMAGE_SIDE * C_IMAGE_SIDE);



void renderImage();
void saveImage(glm::vec3  image[],  int size_2 );

//use sphere picking to create an "out" ray
glm::vec3 getRayBounceLambert();
//angle in = angle out
glm::vec3 getRayBounceSpecular(glm::vec3 * dir, glm::vec3 * normal);
//recursive function
glm::vec3 getPixelVal(glm::vec3* start, glm::vec3* dir, Triangle *scene, int bounceCounter,int numberTriangles);

int main()
{
	std::cout << "Hello human" << std::endl;
	renderImage();
	std::cout << "Done... human" << std::endl;

	int a;
	//std::cin >> a;
	return 0;
}

void renderImage()
{
	
	glm::vec3 *image = new glm::vec3[C_IMAGE_SIZE];
	Camera c;
	int size_2 = C_IMAGE_SIZE / C_IMAGE_SIDE;
	SceneManager Scene;
	int numberTriangles = Scene.getNumberTriangles();

	Triangle *sceneTriangles = Scene.getScene();
	glm::vec3* startPoint = &c.position;
	glm::vec3* direction;
	float n = 0;
	float k = 0;
	float dirY = 0;
	float dirZ = 0;
	int count = 0;
	float maxR = 0, maxG = 0, maxB = 0;
	//main render loop
	#pragma omp parallel for
	for (int i = 0; i < C_IMAGE_SIZE ; ++i)
	{	
		++n;
		if (n == size_2)
		{
			n = 0;
			++k;
		}
		

		#pragma omp parallel for
		for (int multiSampleCount = 0; multiSampleCount < C_MULTISAMPLE_COUNT; ++multiSampleCount)
		{
			dirY = -1.f + (2.f*(n / size_2)) + getRandomsStepVal(n / size_2);
			dirZ = -1.f + (2.f*(k / size_2)) + getRandomsStepVal(k / size_2);
			direction = &glm::normalize(glm::vec3(0.f, dirY, dirZ) - *startPoint);
	
			
			image[i] += getPixelVal(startPoint, direction, sceneTriangles, count, numberTriangles);
		}
		maxR = image[i].x > maxR ? image[i].x : maxR;
		maxG = image[i].y > maxG ? image[i].y : maxG;
		maxB = image[i].z > maxB ? image[i].z : maxB;
	}
	normalizeImage(image, maxR, maxG, maxB, C_IMAGE_SIZE);
	saveImage( image, C_IMAGE_SIZE/ C_IMAGE_SIDE);
	
	delete[] image;
}


glm::vec3 getPixelVal(glm::vec3* start, glm::vec3* dir, Triangle *scene, int count,int numberTriangles)
{
	//This is a simplified model, The ray should "carry" the importance 
	float importanceVal = 0.75f / (count+1);
	if (count >= C_MAX_BOUNCE ||( (count>= C_MAX_BOUNCE/2) 
		&& getRandomFloatAzi()*2 < importanceVal))return glm::vec3(0.f,0.f,0.f);
	++count;

	glm::vec3 colorLocal = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 newStart = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 normal = glm::vec3(0.f, 0.f, 0.f);
	float dimVal = 1.f;
	Material material;
	colorLocal = triangleIntersect(start, dir, newStart, normal, scene, material, numberTriangles);
	bool hitLight = castShadowRay(&newStart, scene, dimVal, numberTriangles);
	colorLocal = hitLight ? colorLocal * Light::getLightstrength() * dimVal : glm::vec3(0.f, 0.f, 0.f);
	
	if (material.isLambert)
	{
		dir = &getRayBounceLambert();
		return colorLocal + importanceVal*getPixelVal(&newStart, dir, scene, count, numberTriangles);
	}
	else if (material.isSpecular)
	{
		
		dir = &getRayBounceSpecular(dir, &normal);
		return colorLocal + getPixelVal(&newStart, dir, scene, count, numberTriangles);
	}

	return colorLocal + importanceVal*getPixelVal(&newStart, dir, scene, count, numberTriangles);
}






glm::vec3 getRayBounceLambert()
{
	
	glm::vec3 v1;
	float teta, phi;

	float randomValInc = getRandomFloatInc();
	float randomValAzi = getRandomFloatAzi();
	teta = randomValInc*M_PI * 2.f;
	phi = acos(randomValAzi);
	v1 = glm::vec3(cos(teta)*sin(phi), sin(phi)*sin(teta), cos(phi));
	return v1;
	

}
glm::vec3 getRayBounceSpecular(glm::vec3 * dir, glm::vec3 * normal)
{
	glm::vec3 directionIn = glm::normalize(*dir);
	return (directionIn - 2 * glm::dot(directionIn, *normal)*(*normal));
}

