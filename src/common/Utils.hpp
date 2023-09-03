#pragma once

#include <vector>

#include <gl\glew.h>
#include <glm/glm.hpp>

#include <Mesh.hpp>
#include <Shader.hpp>

class Utils {
public:
	static void calcAverageNormal(unsigned int* indices, unsigned int indexCount,
		GLfloat* vertices, unsigned int vertexCount, unsigned int vertexLength,
		unsigned int normalOffset);
};