#pragma once
#include "Helpers.h"
#include "SceneManager.h"

//save image to a .ppm
void saveImage(glm::vec3 image[],int size_2)
{
	
	FILE *fp = fopen("image.ppm", "wb"); /* b - binary mode*/
	(void)fprintf(fp, "P6\n%d %d\n255\n", size_2, size_2);

	int y = size_2-1, z = 0;
	for (int i = size_2 - 1; i >= 0; i--)
	{
		for (int n = size_2 - 1; n >= 0; n--)
		{
			static unsigned char color[3];
			color[0] = image[y + (z*size_2)].x;  /* red*/
			color[1] = image[y + (z*size_2)].y;  /* green */
			color[2] = image[y + (z*size_2)].z;  /* blue */
												 //std::cout << i << " i : n " << n << std::endl;
			--y;
			(void)fwrite(color, sizeof(char), 3.f, fp);
		}
		y = size_2-1;
		++z;
	}

	(void)fclose(fp);
}

void normalizeImage(glm::vec3 image[], float maxR, float maxG, float maxB, int imageSize)
{
	#pragma omp parallel for
	for (int i = 0; i < imageSize; ++i)
	{
		image[i].x = 255 * (image[i].x / maxR);
		image[i].y = 255 * (image[i].y / maxG);
		image[i].z = 255 * (image[i].z / maxB);
	}
}


void printVec(glm::vec3 v)
{
	std::cout << v.x << " " << v.y << " " << v.z << std::endl;
}
//sphere picking inclination, should fix so that it is not uniform
float getRandomFloatInc()
{
	std::random_device generator;
	std::mt19937 distribution(generator());
	std::uniform_real_distribution<float> distance(0.05f, 0.95);


	return distance(generator);
}
//sphere picking azimuth
float getRandomFloatAzi()
{
	std::random_device generator;
	std::mt19937 distribution(generator());
	std::uniform_real_distribution<float> distance(0.0f, 1);
	return distance(generator);
}
//multisampling random values
float getRandomsStepVal(int maxStep) 
{
	std::random_device generator;
	std::mt19937 distribution(generator());
	std::uniform_real_distribution<float> distance(0.0f, maxStep);
	return distance(generator);
}

bool castShadowRay(glm::vec3 * triangleHitPosition, Triangle *triangles,float &dimVal,int numberTriangles)
{
	glm::vec3 LightPos = Light::getLightPosition();
	Material material;
	glm::vec3 shadowRayDir = glm::normalize(LightPos - *triangleHitPosition );
	glm::vec3 newHitPos, Normal;
	float lenLight2Intersect = 0;
	triangleIntersect(triangleHitPosition, &shadowRayDir, newHitPos, Normal, triangles, material, numberTriangles);
	lenLight2Intersect = glm::length(LightPos - *triangleHitPosition);
	dimVal = 1 - lenLight2Intersect / 9.6f;
	bool hit = lenLight2Intersect < glm::length(*triangleHitPosition - newHitPos)
				? true : false;
	return hit;
}
glm::vec3 triangleIntersect(glm::vec3* start, glm::vec3* dir, glm::vec3 &newStartPos, glm::vec3 &normal, Triangle *triangles, Material &material,int numberTriangles)
{
	glm::vec3 e1 = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 e2 = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 P = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Q = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 T = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 pixelcolor = glm::vec3(0, 0, 0);
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
	bool closer = false;
	
	for (int i = 0; i < numberTriangles; ++i)
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
		if(t < t2) closer = true;
		else closer = false;
		if ((t) > EPSILON && closer) 
		{ 
			//std::cout << "sdsd";
			pos.x = (1 - u - v)*triangle.vertex1.x + u*triangle.vertex2.x + v*triangle.vertex3.x;
			pos.y = (1 - u - v)*triangle.vertex1.y + u*triangle.vertex2.y + v*triangle.vertex3.y;
			pos.z = (1 - u - v)*triangle.vertex1.z + u*triangle.vertex2.z + v*triangle.vertex3.z;
			pixelcolor = triangle.color;

			newStartPos = pos + triangle.normal*0.1f;
			normal = triangle.normal;
			material = triangle.material;
			hitIs = i + 1;
			t2 = t;
		}


	}
	//printVec(pixelcolor);
	return pixelcolor;
}
