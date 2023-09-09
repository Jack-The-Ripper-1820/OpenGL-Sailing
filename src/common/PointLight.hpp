#pragma once

#include <vector>
#include <Light.hpp>
#include <OmniShadowMap.hpp>

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat ambientIntensity, GLfloat diffuseIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat constant, GLfloat linear, GLfloat exponent, GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far);

	virtual void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalcLightTransform();

	GLfloat GetFarPlane();

	glm::vec3 GetPosition() { return position; }

	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;

	GLfloat farPlane;
};

