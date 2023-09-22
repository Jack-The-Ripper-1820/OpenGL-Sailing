#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.hpp"
#include "Model.hpp"
#include "Shader.hpp"

class Renderer {
public:
	void Render(vector<Mesh*> meshList, Shader &shader);
};