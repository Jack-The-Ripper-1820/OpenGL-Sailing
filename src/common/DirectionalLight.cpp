#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho(-5.f, 5.f, -5.f, 5.f, 0.1f, 20.f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLuint shadowWidth, GLuint shadowHeight)
	: Light(red, green, blue, aIntensity, dIntensity, shadowWidth, shadowHeight)
{
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-20.f, 20.f, -20.f, 20.f, 0.1f, 100.f);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	Light::UseLight(ambientIntensityLocation, ambientColorLocation, diffuseIntensityLocation);
	
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

glm::mat4 DirectionalLight::CalcLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
}

DirectionalLight::~DirectionalLight()
{
}
