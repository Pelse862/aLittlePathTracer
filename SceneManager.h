#pragma once
#include "glm.hpp"
#include <iostream>
const int roomTriangleSize = 12;

struct Triangle
{
	glm::vec3 vertex1, vertex2, vertex3;
	glm::vec3 color;
	glm::vec3 normal;
};

class SceneManager
{
public:
	SceneManager()
	{
		createBasicRoom();
	}

	void createBasicRoom();

	~SceneManager();
	Triangle* getScene() { return roomTriangles; }
private:
	Triangle roomTriangles[roomTriangleSize];
};


