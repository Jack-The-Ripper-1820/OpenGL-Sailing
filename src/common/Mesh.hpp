#pragma once

#include <GL\glew.h>

class Mesh {
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh(bool bWireFrame = false);
	void RenderMeshPatches(bool bWireFrame = false);
	void RenderMeshLines(bool bWireFrame = false);
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};