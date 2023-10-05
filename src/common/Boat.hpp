#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <Utils.hpp>
#include <Mesh.hpp>

class Boat {
	unsigned int boatIndices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat boatVertices[] = {
		//  x      y      z         r,g,b        u      v       nx,    ny,   nz
			-1.0f, -1.0f, -0.6f,   1,1,1,   0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,     1,1,1,   0.5f, 0.f,	  0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,    1,1,1,    1.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,      1,1,1,     0.5f, 1.0f,	  0.0f, 0.0f, 0.0f,
	};




};