
#pragma once
#include "SceneManager.h"



void SceneManager::createBasicRoom()
{
	glm::vec3 vertic[] =
	{
		glm::vec3(-3.f,-6.f,6.f),glm::vec3(-3.f,6.f,6.f) ,glm::vec3(6.f,-6.f,6.f),
		glm::vec3(-3.f,6.f,6.f) ,glm::vec3(6.f,-6.f,6.f) ,glm::vec3(6.f,6.f,6.f),

		glm::vec3(6.f,-6.f,6.f) ,glm::vec3(6.f,6.f,6.f),glm::vec3(6.f,-6.f,-6.f),
		glm::vec3(6.f,6.f,6.f),glm::vec3(6.f,-6.f,-6.f), glm::vec3(6.f,6.f,-6.f),

		glm::vec3(6.f,-6.f,-6.f), glm::vec3(6.f,6.f,-6.f) ,glm::vec3(-3.f,-6.f,-6.f),
		glm::vec3(6.f,6.f,-6.f) ,glm::vec3(-3.f,-6.f,-6.f) ,glm::vec3(-3.f,6.f,-6.f),

		glm::vec3(-3.f,-6.f,-6.f),glm::vec3(-3.f,6.f,-6.f) ,glm::vec3(-3.f,-6.f,-6.f),
		glm::vec3(-3.f,6.f,-6.f),glm::vec3(-3.f,-6.f,6.f) ,glm::vec3(-3.f,6.f,6.f),

		glm::vec3(-3.f,6.f,-6.f),glm::vec3(6.f,6.f,-6.f) ,glm::vec3(-3.f,6.f,6.f),
		glm::vec3(6.f,6.f,-6.f),glm::vec3(6.f,6.f,6.f) ,glm::vec3(-3.f,6.f,6.f),

		glm::vec3(-3.f,-6.f,-6.f),glm::vec3(-3.f,-6.f,6.f),glm::vec3(6.f,-6.f,-6.f),
		glm::vec3(6.f,-6.f,-6.f),glm::vec3(-3.f,-6.f,6.f),glm::vec3(6.f,-6.f,6.f)
	};
	glm::vec3 color[] =
	{ 
		glm::vec3(255.f,0.f,0.f),
		glm::vec3(255.f,0.f,0.f),
		glm::vec3(0.f,255.f,0.f),
		glm::vec3(0.f,255.f,0.f),
		glm::vec3(0.f,0.f,255.f),
		glm::vec3(0.f,0.f,255.f),
		glm::vec3(255.f,255.f,0.f),
		glm::vec3(255.f,255.f,0.f),
		glm::vec3(255.f,255.f,255.f),
		glm::vec3(255.f,255.f,255.f),
		glm::vec3(255.f,255.f,255.f),
		glm::vec3(255.f,255.f,255.f)
	};

	glm::vec3 u, v, vecTemp;

	glm::vec3 tempRayPos(0, 0, 0);
	for (int i = 0; i < 36; i+=3)
	{
		roomTriangles[i/3].vertex1 = vertic[i];
		roomTriangles[i/3].vertex2 = vertic[i+1];
		roomTriangles[i/3].vertex3 = vertic[i+2];
		roomTriangles[i/3].color = color[i / 3];
		
		roomTriangles[i / 3].material.isLambert = true;
		//if(i == 9)roomTriangles[i / 3].material.isSpecular = true;
	
		//std::cout << color[i / 3].x << std::endl;
		u = roomTriangles[i / 3].vertex2 - roomTriangles[i / 3].vertex1;
		v = roomTriangles[i / 3].vertex3 - roomTriangles[i / 3].vertex2;

		roomTriangles[i / 3].normal = glm::normalize(glm::cross(v, u));
		vecTemp = glm::normalize(roomTriangles[i / 3].vertex1 - tempRayPos);
		if ((glm::dot(vecTemp, roomTriangles[i / 3].normal)) >= 0)roomTriangles[i / 3].normal = -roomTriangles[i / 3].normal;
	}
	numberTriangles = 12;
}

void SceneManager::addTetrahedron(glm::vec3 pos)
{
	
	
}

SceneManager::~SceneManager()
{
}
