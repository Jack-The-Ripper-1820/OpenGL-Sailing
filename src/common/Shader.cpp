#include <Shader.hpp>
#include <iostream>
#include <format>

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s : File doesn't exist \n", fileLocation);
		return std::string();
	}

	std::string line = "";

	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint Shader::GetProjectionLocation() {
	return uniformProjection;
}
GLuint Shader::GetModelLocation() {
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation()
{
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::GetPointLightCount()
{
	return uniformPointLightCount;
}

GLuint Shader::GetOmniLightPosLocation()
{
	return uniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation()
{
	return uniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight)
{
	directionalLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pointLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > N_POINT_LIGHTS) lightCount = N_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		pointLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

		pointLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].uniformFarPlane, pointLight[i].GetFarPlane());
	}
}

void Shader::SetSpotLights(SpotLight* spotLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > N_SPOT_LIGHTS) lightCount = N_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++) {
		spotLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformDirection, uniformSpotLight[i].uniformEdgeAngle);
	
		spotLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMap[i + offset].uniformShadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMap[i + offset].uniformFarPlane, spotLight[i].GetFarPlane());
	}
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lightTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lightTransform));
}

void Shader::SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++) {
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");

	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");

	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < N_POINT_LIGHTS; i++) {
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, std::format("pointLights[{}].base.color", i).c_str());
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, std::format("pointLights[{}].base.ambientIntensity", i).c_str());
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, std::format("pointLights[{}].base.diffuseIntensity", i).c_str());
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, std::format("pointLights[{}].position", i).c_str());
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, std::format("pointLights[{}].constant", i).c_str());
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, std::format("pointLights[{}].linear", i).c_str());
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, std::format("pointLights[{}].exponent", i).c_str());
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < N_SPOT_LIGHTS; i++) {
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, std::format("spotLights[{}].base.base.color", i).c_str());
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, std::format("spotLights[{}].base.base.ambientIntensity", i).c_str());
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, std::format("spotLights[{}].base.base.diffuseIntensity", i).c_str());
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, std::format("spotLights[{}].base.position", i).c_str());
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, std::format("spotLights[{}].base.constant", i).c_str());
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, std::format("spotLights[{}].base.linear", i).c_str());
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, std::format("spotLights[{}].base.exponent", i).c_str());
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, std::format("spotLights[{}].direction", i).c_str());
		uniformSpotLight[i].uniformEdgeAngle = glGetUniformLocation(shaderID, std::format("spotLights[{}].edgeAngle", i).c_str());
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; i++) {
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, std::format("lightMatrices[{}]", i).c_str());
	}

	for (size_t i = 0; i < N_POINT_LIGHTS + N_SPOT_LIGHTS; i++) {
		uniformOmniShadowMap[i].uniformShadowMap = glGetUniformLocation(shaderID, std::format("omniShadowMaps[{}].shadowMap", i).c_str());
		uniformOmniShadowMap[i].uniformFarPlane = glGetUniformLocation(shaderID, std::format("omniShadowMaps[{}].farPlane", i).c_str());

	}
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
	shaderID = glCreateProgram();

	if (!shaderID) {
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void Shader::UseShader() {
	glUseProgram(shaderID);
}

void Shader::ClearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader() {
	ClearShader();
}