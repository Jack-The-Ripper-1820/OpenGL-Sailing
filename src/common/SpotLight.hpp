#pragma once

#include <PointLight.hpp>

class SpotLight : public PointLight {
public:
	SpotLight();

	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat ambientIntensity, GLfloat diffuseIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat constant, GLfloat linear, GLfloat exponent,
		GLfloat edgeAngle, GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far);


	virtual void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint directionLocation,
		GLuint edgeLocation);

	void SetFlash(glm::vec3 const& position, glm::vec3 const& direction);

	void Toggle() { isOn = !isOn; }

	~SpotLight();


private:
	glm::vec3 direction;

	GLfloat edgeAngle, procEdgeAngle;

	bool isOn;
};