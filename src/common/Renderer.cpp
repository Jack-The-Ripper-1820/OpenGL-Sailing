#include "Renderer.hpp"

void Renderer::Render(vector<Mesh*> meshList, Shader &shader)
{
	shader.UseShader();

	for (size_t i = 0; i < meshList.size(); i++) {

	}

	glUseProgram(0);
}
