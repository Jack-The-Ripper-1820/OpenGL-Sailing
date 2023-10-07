#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <Constants.hpp>
#include <DirectionalLight.hpp>
#include <PointLight.hpp>

class Shader {
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* tessControlLocation, const char* tessEvalLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* computeLocation);

	void Validate();

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetEyePositionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetPointLightCount();
	GLuint GetOmniLightPosLocation();
	GLuint GetFarPlaneLocation();
	GLuint GetTesslationLevelLocation();
	GLuint GetShaderID();
	GLuint GetTime();
	GLuint GetClipPlaneLocation();
	GLuint GetMoveFactorLocation();

	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLights(PointLight* pointLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lightTransform);
	void SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices);
	void SetTessellationLevel(float level);
	void SetTime(float time);
	void SetClipPlane(glm::vec4 clipPlane);
	void SetReflectionTexture(GLuint textureUnit);
	void SetRefractionTexture(GLuint textureUnit);
	void SetMoveFactor(float moveFactor);

	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount, spotLightCount;

	GLuint shaderID, uniformProjection, uniformModel, uniformView,
		uniformEyePosition, uniformSpecularIntensity, uniformShininess,
		uniformPointLightCount,
		uniformDirectionalShadowMap,
		uniformDirectionalLightTransform,
		uniformTexture,
		uniformOmniLightPos,
		uniformFarPlane,
		uniformTessellationLevel,
		uniformTime,
		uniformClipPlane,
		uniformReflectionTexture,
		uniformRefractionTexture,
		uniformMoveFactor;

	GLuint uniformLightMatrices[6];

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	struct {
		GLuint uniformShadowMap;
		GLuint uniformFarPlane;
	} uniformOmniShadowMap[MAX_POINT_LIGHTS];

	void CompileProgram();
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* tessControlCode, const char* tessEvalCode, const char* fragmentCode);
	void CompileShader(const char* computeCode);
	GLuint AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};