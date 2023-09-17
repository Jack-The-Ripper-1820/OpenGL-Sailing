#pragma once

#include <GL\glew.h>
#include <stdio.h>

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

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	GLuint controlpointVBO;
	GLuint controlpointVAO;


};