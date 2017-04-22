#pragma once
#include "glm.hpp"

const int maxNumberTriangles = 300;
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
	int getNumberTriangles() { return numberTriangles; }
	void createBasicRoom();
	void addTetrahedron(glm::vec3 pos);
	~SceneManager();
	Triangle* getScene() { return roomTriangles; }
private:
	int numberTriangles;
	Triangle roomTriangles[maxNumberTriangles];
};


