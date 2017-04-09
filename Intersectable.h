#pragma once
#include "Helpers.h"

#define EPSILON 0.00000000000000001

enum State { sphere, triangle };

struct Sphere : public Intersectable {
	glm::vec3 color;
	float radius;
	glm::vec3 position;
	State state;
};
struct Triangle : public Intersectable {
	glm::vec3 vertex1, vertex2, vertex3;
	glm::vec3 color;
	glm::vec3 normal;
	State state;
};

class Intersectable
{
public:


	Intersectable();
	glm::vec3 intersect(Intersectable* obj);

	~Intersectable();
private:
};



inline glm::vec3 intersectTriangle(glm::vec3* start, glm::vec3* dir, glm::vec3 &newStartPos, glm::vec3 &normal, Triangle *object);
inline glm::vec3 intersectSphere(glm::vec3* start, glm::vec3* dir, glm::vec3 &newStartPos, glm::vec3 &normal, Sphere *object);