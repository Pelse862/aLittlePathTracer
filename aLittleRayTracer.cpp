#include <iostream>
#include "glm.hpp"
#include "SceneManager.h"
#include <random>
#include "Helpers.h"

#define EPSILON 0.00000000000000001
#define M_PI 3.14159265358979323846  /* pi */

const int D_MAX_BOUNCE = 16;
const int size = (256 * 256);



void renderImage();
void saveImage(glm::vec3  image[],  int size_2 );
float getRandomFloatInc();
float getRandomFloatAzi();

glm::vec3 getRayBounceLambert();
glm::vec3 getRayBounceSpecular(glm::vec3 * dir, glm::vec3 * normal);

glm::vec3 getPixelVal(glm::vec3* start, glm::vec3* dir, Triangle *scene, int bounceCounter);
glm::vec3 triangleIntersect(glm::vec3* start, glm::vec3* dir, glm::vec3 &newStartPos, glm::vec3 &normal, Triangle *t);

int main()
{
	
	std::cout << "hello human" << std::endl;
	renderImage();
	
	int a;
	std::cin >> a;
	return 0;
}

void renderImage()
{
	
	glm::vec3 image[size];
	Camera c;
	int j = 0;
	int size_2 = size / 256;
	SceneManager Scene;
	Triangle *sceneTriangles = Scene.getScene();
	glm::vec3* startPoint = &c.position;
	glm::vec3* direction;
	float n = 0;
	float k = 0;
	float dirY = 0;
	float dirZ = 0;
	int count = 0;
	float maxR = 0, maxG = 0, maxB = 0;
	#pragma omp parallel for
	for (int i = 0; i < size ; ++i)
	{	
		++n;
		if (n == size_2)
		{
			n = 0;
			++k;
		}
		dirY = -1.f +( 2.f*(n / size_2) );
		dirZ = -1.f +( 2.f*(k / size_2) );
		
		direction = &glm::normalize(glm::vec3( 0.f , dirY, dirZ) - *startPoint);
		
		image[i] = getPixelVal(startPoint, direction, sceneTriangles, count);
		
		
		maxR = image[i].x > maxR ? image[i].x : maxR;
		maxG = image[i].y > maxG ? image[i].y : maxG;
		maxB = image[i].z > maxB ? image[i].z : maxB;
	}

	normalizeImage(image, maxR, maxG, maxB, size);
	saveImage( image, size/256);
}


glm::vec3 getPixelVal(glm::vec3* start, glm::vec3* dir, Triangle *scene, int count)
{	
	if (count >= D_MAX_BOUNCE)return glm::vec3(0.f,0.f,0.f);
	++count;

	glm::vec3 colorLocal = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 newStart = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 normal = glm::vec3(0.f, 0.f, 0.f);
	//printVec(*dir);
	//printVec(*start);
	
	//printVec(*triangleIntersect(start, dir, scene));
	//colorLocal = *triangleIntersect(start, dir, newPos, normal, scene);
	
	//start = &newPos;
	//dir = &getRayBounceLambert(dir, &normal);
	//printVec(*triangleIntersect(start, dir, newStart, normal, scene));
	colorLocal = triangleIntersect(start, dir, newStart, normal, scene);


	dir = &getRayBounceLambert();

	return colorLocal + (0.5f/count)*getPixelVal(&newStart, dir, scene, count);
}




glm::vec3 triangleIntersect( glm::vec3* start, glm::vec3* dir, glm::vec3 &newStartPos, glm::vec3 &normal, Triangle *triangles)
{
	glm::vec3 e1 = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 e2 = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 P = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Q = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 T = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 pixelcolor = glm::vec3(0,0,0);
	glm::vec3 pos;
	float t = 0;
	float t2 = 1000000;
	float det = 0;
	float inv_det = 0;
	float u = 0;
	float v = 0;
	bool hit = true;
	glm::vec3 D = glm::normalize(*dir);
	int hitIs = 0;
	for (int i = 0; i < 12; ++i) 
	{
		Triangle triangle = triangles[i];

		//find vectors for 2 edges sharing V1
		e1 = triangle.vertex2 - triangle.vertex1;
		e2 = triangle.vertex3 - triangle.vertex1;

		P = glm::cross(D, e2);
		det = glm::dot(e1, P);


		if (det > -EPSILON && det < EPSILON) continue;

		inv_det = 1.f / det;

		T = *start - triangle.vertex1;

		u = glm::dot(T, P) * inv_det;

		if (u < 0.f || u > 1.f) continue;

		Q = glm::cross(T, e1);

		//Calculate V parameter and test bound
		v = glm::dot(D, Q) * inv_det;

		//The intersection lies outside of the triangle
		if (v < 0.f || u + v  > 1.f) continue;

		t = glm::dot(e2, Q) * inv_det;

		if ((t) > EPSILON) { //ray intersection
	
			pos.x = (1 - u - v)*triangle.vertex1.x + u*triangle.vertex2.x + v*triangle.vertex3.x;
			pos.y = (1 - u - v)*triangle.vertex1.y + u*triangle.vertex2.y + v*triangle.vertex3.y;
			pos.z = (1 - u - v)*triangle.vertex1.z + u*triangle.vertex2.z + v*triangle.vertex3.z;
			pixelcolor = triangle.color;
			
			newStartPos = pos + triangle.normal*0.01f;
			normal = triangle.normal;
			hitIs = i + 1;
			t2 = t;
		}
	

	}
	//printVec(pixelcolor);
	return pixelcolor;
}

float getRandomFloatInc()
{
	std::random_device generator;
	std::mt19937 distribution(generator());
	std::uniform_real_distribution<float> distance(0.05f, 0.95);


	return distance(generator);
}
float getRandomFloatAzi()
{
	std::random_device generator;
	std::mt19937 distribution(generator());
	std::uniform_real_distribution<float> distance(0.0f, 1);


	return distance(generator);
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