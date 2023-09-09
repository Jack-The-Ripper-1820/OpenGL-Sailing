#pragma once

#include <vector>
#include <string>

#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <Constants.hpp>
#include <Mesh.hpp>
#include <Shader.hpp>

class Skybox {
public:
	Skybox();

	Skybox(std::vector<std::string> const& faceLocations);

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectioMatrix);

	~Skybox();

private:
	Mesh* skyMesh;
	Shader* skyShader;

	GLuint textureId;
	GLuint uniformProjection, uniformView;

};