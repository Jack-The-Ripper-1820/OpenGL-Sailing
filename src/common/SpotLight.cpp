#include "SpotLight.hpp"
#include "SpotLight.hpp"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.f, -1.f, 0.f);
	edgeAngle = 0.f;
	procEdgeAngle = cosf(glm::radians(this->edgeAngle));

}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat constant, GLfloat linear, GLfloat exponent, GLfloat edgeAngle, GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far)
	: PointLight(red, green, blue, ambientIntensity, diffuseIntensity, xPos, yPos, zPos, constant, linear, exponent, shadowWidth, shadowHeight, near, far)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	this->edgeAngle = edgeAngle;
	procEdgeAngle = cosf(glm::radians(this->edgeAngle));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint directionLocation, GLuint edgeLocation)
{
	PointLight::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseIntensityLocation, positionLocation, constantLocation, linearLocation, exponentLocation);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdgeAngle);
}

void SpotLight::SetFlash(glm::vec3 const& position, glm::vec3 const& direction)
{
	this->position = position;
	this->direction = direction;
}

SpotLight::~SpotLight()
{
}
