#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <stdio.h>
#include <vector>

using namespace std;

class Mesh {
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh(bool bWireFrame = false);
	void RenderMeshPatches(bool bWireFrame = false);
	void RenderMeshLines(bool bWireFrame = false);
	void ClearMesh();
	void CreateMeshFromControlPoints(GLfloat* controlpoints, unsigned int numcontrolpoints);
	void ConvertPointsToPatches(GLfloat* controlpoints);
	void CreateWaterMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	pair<glm::vec3, glm::vec3> GetBoundaries();

	~Mesh();

	glm::mat4 model;

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	GLuint controlpointVBO;
	GLuint controlpointVAO;
	float minX = numeric_limits<float>::max();
	float minY = numeric_limits<float>::max();
	float minZ = numeric_limits<float>::max();
	float maxX = numeric_limits<float>::min(); 
	float maxY = numeric_limits<float>::min();  
	float maxZ = numeric_limits<float>::min();
};