#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW/glfw3.h>



class Collider {
public:
	int ID;
	int cnt = 0;
	float maxX = 0, maxY = 0, maxZ = 0;
	float minX = 0, minY = 0, minZ = 0;

	void InitCollider(GLfloat* vertices, unsigned int vertexCount, unsigned int vertexOffset);
	void SetCollider(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

	//bool IsColliding(Collider const& collider, float offset = 0.f);

	bool IsColliding(const Collider& other);

	bool printFlag = true;

	float GetMaxX() { return maxX; }
	float GetMaxY() { return maxY; }
	float GetMaxZ() { return maxZ; }
	float GetMinX() { return minX; }
	float GetMinY() { return minY; }
	float GetMinZ() { return minZ; }

	glm::vec3 minBounds;
	glm::vec3 maxBounds;

	Collider();
	Collider(glm::vec3 min, glm::vec3 max, int ID);

	void Transform(const glm::mat4& transform);
	void Translate(glm::vec3 const& dVector);
};