#pragma once
#include "glm.hpp"

const int roomTriangleSize = 12;
struct Material
{
	bool isLambert = false;
	bool isSpecular = false;
};
struct Triangle
{
	glm::vec3 vertex1, vertex2, vertex3;
	glm::vec3 color;
	glm::vec3 normal;
	Material material;
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


