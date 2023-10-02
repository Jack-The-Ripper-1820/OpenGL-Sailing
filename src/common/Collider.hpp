#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Collider {
private:
	float maxX, maxY, maxZ;
	float minX, minY, minZ;
public:
	void SetCollider(GLfloat* vertices, unsigned int vertexCount, unsigned int vertexOffset);

	float GetMaxX() { return maxX; }
	float GetMaxY() { return maxY; }
	float GetMaxZ() { return maxZ; }
	float GetMinX() { return minX; }
	float GetMinY() { return minY; }
	float GetMinZ() { return minZ; }
};