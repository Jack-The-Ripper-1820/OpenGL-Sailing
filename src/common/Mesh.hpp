#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <vector>

#include "Collider.hpp"

using namespace std;

class Mesh {
public:

	Mesh();
	Mesh(int ID);
	int ID;
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh(bool bWireFrame = false);
	void RenderMeshPatches(bool bWireFrame = false);
	void RenderMeshLines(bool bWireFrame = false);
	void ClearMesh();

	Collider* GetCollider() const { return collider; }

	bool IsColliding(const Mesh* other);
	void TransformCollider(glm::mat4 const& transform);
	void TranslateCollider(glm::vec3 const& dVector);

	~Mesh();

	glm::mat4 model;	

	void SetEdges(vector<pair<glm::vec3, glm::vec3>> &edges) {
		this->collider->edges = edges;
	}

private:
	Collider* collider;
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};