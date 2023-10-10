#include "Collider.hpp"
#include <iostream>
#include <utility>

bool Collider::IsColliding(const Collider& other)
{
	return ConvexHullCollision(other);
}

Collider::Collider()
{
}

Collider::Collider(int ID) : ID(ID)
{

}

void Collider::Transform(const glm::mat4& transform)
{
	transformEdges(transform);
}

void Collider::Translate(glm::vec3 const& dVector)
{
	for (auto& edge : edges) {
		edge.first += dVector;
		edge.second += dVector;
	}
}


bool Collider::NoOverlap(const glm::vec2& a, const glm::vec2& b) {
	return a.y + 0.025 < b.x || b.y + 0.025 < a.x;
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
	for (Edge& edge : edges) {
		edge = transformEdge(edge, transformationMatrix);
	}
}
