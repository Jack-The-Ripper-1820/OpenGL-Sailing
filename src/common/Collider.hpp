#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>

using Edge = std::pair<glm::vec3, glm::vec3>;


class Collider {
public:
	int ID;
	int cnt = 0;

	bool IsColliding(const Collider& other);

	bool printFlag = true;

	std::vector<Edge> edges;

	Collider();
	Collider(int ID);

	void Transform(const glm::mat4& transform);
	void Translate(glm::vec3 const& dVector);

	bool ConvexHullCollision(const Collider& other);
	bool NoOverlap(const glm::vec2& a, const glm::vec2& b);
	glm::vec2 Project(const std::vector<Edge>& hull, const glm::vec3& axis);


	Edge transformEdge(const Edge& edge, const glm::mat4& transformationMatrix);
	void transformEdges(const glm::mat4& transformationMatrix);



	

	
};