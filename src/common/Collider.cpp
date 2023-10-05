#include "Collider.hpp"
#include <iostream>
#include <utility>

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

	//if (printFlag) {
		/*std::cout << "in colliding: " << ID << std::endl;
		std::cout << minBounds.x << " " << minBounds.y << " " << minBounds.z << std::endl;
		std::cout << maxBounds.x << " " << maxBounds.y << " " << maxBounds.z << std::endl;
		std::cout << other.minBounds.x << " " << other.minBounds.y << " " << other.minBounds.z << std::endl;
		std::cout << other.maxBounds.x << " " << other.maxBounds.y << " " << other.maxBounds.z << std::endl;
		printFlag = false;*/
	//}
	auto inRange = [](float value, float min, float max) {
		return value >= std::min(min, max) && value <= std::max(min, max);
	};

	/*return (inRange(minBounds.x, other.minBounds.x, other.maxBounds.x) ||
		inRange(maxBounds.x, other.minBounds.x, other.maxBounds.x)) &&
		(inRange(minBounds.y, other.minBounds.y, other.maxBounds.y) ||
			inRange(maxBounds.y, other.minBounds.y, other.maxBounds.y)) &&
		(inRange(minBounds.z, other.minBounds.z, other.maxBounds.z) ||
			inRange(maxBounds.z, other.minBounds.z, other.maxBounds.z));*/

	/*return (minBounds.x <= other.maxBounds.x && maxBounds.x >= other.minBounds.x) &&
		(minBounds.y <= other.maxBounds.y && maxBounds.y >= other.minBounds.y) &&
		(minBounds.z <= other.maxBounds.z && maxBounds.z >= other.minBounds.z);*/

	return ConvexHullCollision(other);
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

	/*std::cout << "TRANSFORM" << std::endl;

	std::cout << minBounds.x << " " << minBounds.y << " " << minBounds.z << std::endl;
	std::cout << maxBounds.x << " " << maxBounds.y << " " << maxBounds.z << std::endl << std::endl;*/

	//minBounds.z += 

	//if (cnt < 3) {
	//	std::cout << minBounds.x << " " << minBounds.y << " " << minBounds.z << std::endl;
	//	std::cout << maxBounds.x << " " << maxBounds.y << " " << maxBounds.z << std::endl << std::endl;
	//	cnt++;
	//}

	transformEdges(transform);
}

void Collider::Translate(glm::vec3 const& dVector)
{
	minBounds += dVector;
	maxBounds += dVector;

	for (auto& edge : edges) {
		edge.first += dVector;
		edge.second += dVector;
	}
}


bool Collider::NoOverlap(const glm::vec2& a, const glm::vec2& b) {
	return a.y < b.x || b.y < a.x;
}

glm::vec2 Collider::Project(const std::vector<Edge>& hull, const glm::vec3& axis) {
	float min = glm::dot(axis, hull[0].first);
	float max = min;
	for (const Edge& edge : hull) {
		float p1 = glm::dot(axis, edge.first);
		float p2 = glm::dot(axis, edge.second);
		if (p1 < min) min = p1;
		if (p2 < min) min = p2;
		if (p1 > max) max = p1;
		if (p2 > max) max = p2;
	}
	return glm::vec2(min, max);
}

bool Collider::ConvexHullCollision(const Collider& other) {
	// Project onto normals from hull1 edges

	const std::vector<Edge>& hull1 = this->edges;
	const std::vector<Edge>& hull2 = other.edges;

	for (const Edge& edge : hull1) {
		glm::vec3 edgeDir = edge.second - edge.first;
		glm::vec3 normal = glm::normalize(glm::vec3(-edgeDir.y, edgeDir.x, 0.0f));
		if (NoOverlap(Project(hull1, normal), Project(hull2, normal))) {
			return false;
		}
	}

	// Project onto normals from hull2 edges
	for (const Edge& edge : hull2) {
		glm::vec3 edgeDir = edge.second - edge.first;
		glm::vec3 normal = glm::normalize(glm::vec3(-edgeDir.y, edgeDir.x, 0.0f));
		if (NoOverlap(Project(hull1, normal), Project(hull2, normal))) {
			return false;
		}
	}

	// Project onto cross products of edges
	for (const Edge& edge1 : hull1) {
		for (const Edge& edge2 : hull2) {
			glm::vec3 axis = glm::cross(edge1.second - edge1.first, edge2.second - edge2.first);
			if (glm::length(axis) != 0) { // Ensure it's not the zero vector
				axis = glm::normalize(axis);
				if (NoOverlap(Project(hull1, axis), Project(hull2, axis))) {
					return false;
				}
			}
		}
	}

	// No separating axis found
	return true;
}

Edge Collider::transformEdge(const Edge& edge, const glm::mat4& transformationMatrix) {
	glm::vec4 transformedStart = transformationMatrix * glm::vec4(edge.first, 1.0f);
	glm::vec4 transformedEnd = transformationMatrix * glm::vec4(edge.second, 1.0f);

	return { glm::vec3(transformedStart), glm::vec3(transformedEnd) };
}

void Collider::transformEdges(const glm::mat4& transformationMatrix) {
	//std::vector<Edge> transformedEdges;
	//transformedEdges.reserve(edges.size());

	for (Edge& edge : edges) {
		edge = transformEdge(edge, transformationMatrix);
	}

	//return transformedEdges;
}
