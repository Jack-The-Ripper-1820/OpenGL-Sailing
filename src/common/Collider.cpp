#include "Collider.hpp"
#include <iostream>

void Collider::InitCollider(GLfloat* vertices, unsigned int vertexCount, unsigned int vertexOffset)
{
	for (unsigned int i = 0; i < vertexCount; i += vertexOffset) {
		minX = std::min(minX, vertices[i]);
		maxX = std::max(maxX, vertices[i]);

		minY = std::min(minY, vertices[i + 1]);
		maxY = std::max(maxY, vertices[i + 1]);

		minZ = std::min(minZ, vertices[i + 2]);
		maxZ = std::max(maxZ, vertices[i + 2]);
	}
}

void Collider::SetCollider(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	this->minX = minX;
	this->minY = minY;
	this->minZ = minZ;

	this->maxX = maxX;
	this->maxY = maxY;
	this->maxZ = maxZ;
}

//bool Collider::IsColliding(Collider const& other, float offset = 0.f)
//{
//	if (this->maxX + offset < other.minX || this->minX - offset > other.maxX) return false;
//	if (this->maxY + offset < other.minY || this->minY - offset > other.maxY) return false;
//	if (this->maxZ + offset < other.minZ || this->minZ - offset > other.maxZ) return false;
//
//	return true;  // No separating axis found, therefore they collide
//}

bool Collider::IsColliding(const Collider& other)
{
	//std::cout << minBounds.x << " " << other.minBounds.x << std::endl;

	if (printFlag) {
		std::cout << "in colliding: " << ID << std::endl;
		std::cout << minBounds.x << " " << minBounds.y << " " << minBounds.z << std::endl;
		std::cout << maxBounds.x << " " << maxBounds.y << " " << maxBounds.z << std::endl;
		std::cout << other.minBounds.x << " " << other.minBounds.y << " " << other.minBounds.z << std::endl;
		std::cout << other.maxBounds.x << " " << other.maxBounds.y << " " << other.maxBounds.z << std::endl;
		printFlag = false;
	}
	auto inRange = [](float value, float min, float max) {
		return value >= min && value <= max;
	};

	return (inRange(minBounds.x, other.minBounds.x, other.maxBounds.x) ||
		inRange(maxBounds.x, other.maxBounds.x, other.maxBounds.x)) &&
		(inRange(minBounds.y, other.minBounds.y, other.maxBounds.y) ||
			inRange(maxBounds.y, other.maxBounds.y, other.maxBounds.y)) &&
		(inRange(minBounds.z, other.minBounds.z, other.maxBounds.z) ||
			inRange(maxBounds.z, other.maxBounds.z, other.maxBounds.z));

	/*return (minBounds.x <= other.maxBounds.x && maxBounds.x >= other.minBounds.x) &&
		(minBounds.y <= other.maxBounds.y && maxBounds.y >= other.minBounds.y) &&
		(minBounds.z <= other.maxBounds.z && maxBounds.z >= other.minBounds.z);*/
}

Collider::Collider()
{
}

Collider::Collider(glm::vec3 min, glm::vec3 max, int ID) : minBounds(min), maxBounds(max), ID(ID)
{

}

void Collider::Transform(const glm::mat4& transform)
{
	minBounds = glm::vec3(transform * glm::vec4(minBounds, 1.0f));
	maxBounds = glm::vec3(transform * glm::vec4(maxBounds, 1.0f));

	//minBounds.z += 

	//if (cnt < 3) {
	//	std::cout << minBounds.x << " " << minBounds.y << " " << minBounds.z << std::endl;
	//	std::cout << maxBounds.x << " " << maxBounds.y << " " << maxBounds.z << std::endl << std::endl;
	//	cnt++;
	//}
}

void Collider::Translate(glm::vec3 const& dVector)
{
	minBounds += dVector;
	maxBounds += dVector;
}

