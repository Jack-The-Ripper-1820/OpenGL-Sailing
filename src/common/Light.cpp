#include <Light.hpp>

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.f;
	diffuseIntensity = 0.f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity,
	GLuint shadowWidth, GLuint shadowHeight)
{
	color = glm::vec3(glm::clamp(red, 0.0f, 1.0f),
		glm::clamp(green, 0.0f, 1.0f),
		glm::clamp(blue, 0.0f, 1.0f));

	this->ambientIntensity = ambientIntensity;
	this->diffuseIntensity = diffuseIntensity;
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

Light::~Light()
{
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
