#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <assimp/Importer.hpp>

#include <Window.hpp>
#include <Shader.hpp>
#include <Mesh.hpp>
#include <Camera.hpp>
#include <Texture.hpp>
#include <Light.hpp>
#include <Utils.hpp>
#include <Material.hpp>
#include <Constants.hpp>
#include <Model.hpp>
#include <Skybox.hpp>

bool f = true;

std::vector<Mesh*> meshList;

std::vector<Shader*> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Skybox skyBox;

Window mainWindow;
Camera camera;

Material glossyMaterial, matteMaterial;

Model boat;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

DirectionalLight mainLight;
PointLight pointLights[N_POINT_LIGHTS];
SpotLight spotLights[N_SPOT_LIGHTS];

GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;
float tessLevel = 1.f;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;
float curAngle = 0.f;

bool sizeDirection = true;

float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

static const char* vShader = "shaders/vertex.glsl";
static const char* tcs = "shaders/tcs.glsl";
static const char* tes = "shaders/tes.glsl";
static const char* fShader = "shaders/fragment.glsl";
static const char* pTtcs = "shaders/tri_tcs.glsl";
static const char* pTtes = "shaders/tri_tes.glsl";
static const char* ptv = "shaders/tess_vert.glsl";
static const char* pQtcs = "shaders/quad_tcs.glsl";
static const char* pQtes = "shaders/quad_tes.glsl";

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

bool bWireFrame = false;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0,
uniformOmniLightPos = 0, uniformFarPlane = 0, uniformTessellationLevel = 0;

void CreateObjects() {
	GLfloat boatVertices[] = {
		// Base (Triangle 1)
		0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

		// Left Side (Triangle 2)
		0.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		// Right Side (Triangle 3)
		0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		// Back (Triangle 4)
		-0.5f, 0.0f, -0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, -0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f,
	};

	unsigned int boatIndices[] = {
		0, 1, 2, // Triangle 1 (Base)
		3, 4, 5, // Triangle 2 (Left Side)
		6, 7, 8, // Triangle 3 (Right Side)
		9, 10, 11, // Triangle 4 (Back)
	};





	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//  x      y      z                 u      v       nx,    ny,   nz
			-1.0f, -1.0f, -0.6f,   1,1,1,   0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,     1,1,1,   0.5f, 0.f,	  0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,    1,1,1,    1.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,      1,1,1,     0.5f, 1.0f,	  0.0f, 0.0f, 0.0f,
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	1,1,1,    0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1,1,1,    10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	1,1,1,    0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1,1,1,    10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};


	Utils::calcAverageNormal(indices, 12, vertices, 44, 11, 8);
	Utils::calcAverageNormal(boatIndices, 12, boatVertices, 11 * 12, 11, 8);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 44, 12);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 44, 12);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 44, 6);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(boatVertices, boatIndices, 11 * 12, 12);



	meshList.push_back(obj1);
	meshList.push_back(obj2);
	meshList.push_back(obj3);
	meshList.push_back(obj4);
}

void CreateShaders() {
	

	Shader* shaderPNT = new Shader();
	shaderPNT->CreateFromFiles(ptv, pTtcs, pTtes, fShader);
	shaderList.push_back(shaderPNT);

	Shader* shaderPNQ = new Shader();
	shaderPNQ->CreateFromFiles(ptv, pQtcs, pQtes, fShader);
	shaderList.push_back(shaderPNQ);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("shaders/directional_shadow_map_vertex.glsl", "shaders/directional_shadow_map_fragment.glsl");
	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("shaders/omni_directional_shadow_map_vertex.glsl", "shaders/omni_directional_shadow_map_geometry.glsl",
		"shaders/omni_directional_shadow_map_fragment.glsl");
}

void RenderScene() {
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	matteMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	//model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	boat.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();

	glUseProgram(0);
}

void RenderSceneTess() {
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMeshPatches(bWireFrame);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	matteMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMeshPatches(bWireFrame);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMeshPatches(bWireFrame);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	//model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	boat.RenderModelPatches(bWireFrame);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMeshPatches();

	glUseProgram(0);
}

void OmniShadowMapPass(PointLight* light) {
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	omniShadowShader.UseShader();
	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	light->GetShadowMap()->Write();

	glClear(GL_DEPTH_BUFFER_BIT);

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetOmniLightMatrices(light->CalcLightTransform());

	omniShadowShader.Validate();
	RenderSceneTess();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalShadowMapPass(DirectionalLight* light) {
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 lightTransform = light->CalcLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);

	directionalShadowShader.Validate();
	RenderSceneTess();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	glViewport(0, 0, 1366, 768);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skyBox.DrawSkybox(viewMatrix, projectionMatrix);

	//glUseProgram(0);

	glPatchParameteri(GL_PATCH_VERTICES, 3);

	shaderList[0]->UseShader();

	if(f)
		std::cout << "shader id in render" << shaderList[0]->GetShaderID() << std::endl, f = false;

	uniformModel = shaderList[0]->GetModelLocation();
	uniformProjection = shaderList[0]->GetProjectionLocation();
	uniformView = shaderList[0]->GetViewLocation();
	uniformModel = shaderList[0]->GetModelLocation();
	uniformEyePosition = shaderList[0]->GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
	uniformShininess = shaderList[0]->GetShininessLocation();
	uniformTessellationLevel = shaderList[0]->GetTesslationLevelLocation();


	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	
	std::cout << tessLevel << std::endl;

	shaderList[0]->SetTessellationLevel(tessLevel);
	//glUniform1f(uniformTessellationLevel, tessLevel);

	shaderList[0]->SetDirectionalLight(&mainLight);
	shaderList[0]->SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0]->SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	auto lightTansform = mainLight.CalcLightTransform();
	shaderList[0]->SetDirectionalLightTransform(&lightTansform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0]->SetTexture(1);
	shaderList[0]->SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0]->Validate();

	RenderSceneTess();
}


int main() {
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("textures/plain.png");
	plainTexture.LoadTextureA();

	glossyMaterial = Material(4.0f, 256);
	matteMaterial = Material(0.3f, 4);

	boat = Model();
	boat.LoadModel("models/boat.obj");

	/*mech = Model();
	mech.LoadModel("models/Kaiser.obj");*/

	mainLight = DirectionalLight(
		0.678f, 0.847f, 0.902f,
		0.1f, 0.9f,
		-10.0f, -12.0f, -18.5f,
		2048, 2048);

	pointLights[1] = PointLight(
		0.678, 0.847, 0.902,
		0.0f, 0.4f,
		2.0f, 2.0f, 0.0f,
		0.3f, 0.01f, 0.01f,
		1024, 1024,
		0.1f, 100.0f);

	pointLightCount++;

	pointLights[0] = PointLight(
		0.400, 0.600, 1.000,
		0.0f, 0.4f,
		-2.0f, 2.0f, 0.0f,
		0.3f, 0.01f, 0.01f,
		1024, 1024,
		0.1f, 100.0f);

	pointLightCount++;

	spotLights[0] = SpotLight(
		0.635, 0.482, 0.933,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f,
		1024, 1024,
		0.1f, 100.0f);

	spotLightCount++;

	spotLights[1] = SpotLight(
		0.400, 0.600, 1.000,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-100.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f,
		1024, 1024,
		0.1f, 100.0f);

	spotLightCount++;

	std::vector<std::string> skyBoxFaces;

	skyBoxFaces.push_back("textures/lightblue/right.tga");
	skyBoxFaces.push_back("textures/lightblue/left.tga");
	skyBoxFaces.push_back("textures/lightblue/top.tga");
	skyBoxFaces.push_back("textures/lightblue/bot.tga");
	skyBoxFaces.push_back("textures/lightblue/back.tga");
	skyBoxFaces.push_back("textures/lightblue/front.tga");


	skyBox = Skybox(skyBoxFaces);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		if (mainWindow.getKeys()[GLFW_KEY_L]) {
			spotLights[0].Toggle();
			mainWindow.getKeys()[GLFW_KEY_L] = false;
		}

		if (mainWindow.getKeys()[GLFW_KEY_UP]) {
			tessLevel = std::min(tessLevel + 1, 16.f);
			mainWindow.getKeys()[GLFW_KEY_UP] = false;
		}

		if (mainWindow.getKeys()[GLFW_KEY_DOWN]) {
			tessLevel = std::max(tessLevel - 1, 0.f);
			mainWindow.getKeys()[GLFW_KEY_DOWN] = false;
		}

		if (mainWindow.getKeys()[GLFW_KEY_B]) {
			bWireFrame = !bWireFrame;
			mainWindow.getKeys()[GLFW_KEY_B] = false;
		}



		DirectionalShadowMapPass(&mainLight);

		for (size_t i = 0; i < pointLightCount; i++) {
			OmniShadowMapPass(&pointLights[i]);
		}

		for (size_t i = 0; i < spotLightCount; i++) {
			OmniShadowMapPass(&spotLights[i]);
		}

		RenderPass(camera.calculateViewMatrix(), projection);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}