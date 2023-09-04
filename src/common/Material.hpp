#pragma once

#include <GL\glew.h>

class Material {
public:
	Material();
	Material(GLfloat specularIntensity, GLfloat shininess);
	
	~Material();

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:
	GLfloat shininess;
	GLfloat specularIntensity;
};