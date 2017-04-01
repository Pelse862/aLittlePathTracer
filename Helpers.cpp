#include "Helpers.h"



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
float getRandomsStepVal(int maxStep) 
{
	std::random_device generator;
	std::mt19937 distribution(generator());
	std::uniform_real_distribution<float> distance(0.0f, maxStep);
	return distance(generator);
}