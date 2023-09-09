#pragma once

#include <glm\gtc\matrix_transform.hpp>

#include <Light.hpp>

class DirectionalLight : public Light {
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLuint shadowWidth, GLuint shadowHeight);

	virtual void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

	glm::mat4 CalcLightTransform();

	~DirectionalLight();

private:
	glm::vec3 direction;

};