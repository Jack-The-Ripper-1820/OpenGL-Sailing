#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
#endif

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
#include <WaterFrameBuffers.hpp>

bool f = true;

std::vector<Mesh*> meshList;

std::vector<Shader*> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Skybox skyBox;

Window mainWindow;
Camera camera;

Material glossyMaterial, matteMaterial;

Model boat1, boat2;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture oceanTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

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

float moveFactor = 0;

static const char* vShader = "shaders/vertex.glsl";
static const char* tcs = "shaders/tcs.glsl";
static const char* tes = "shaders/tes.glsl";
static const char* fShader = "shaders/fragment.glsl";
static const char* pTtcs = "shaders/tri_tcs.glsl";
static const char* pTtes = "shaders/tri_tes.glsl";
static const char* ptv = "shaders/tess_vert.glsl";
static const char* pQtcs = "shaders/quad_tcs.glsl";
static const char* pQtes = "shaders/quad_tes.glsl";
static const char* nurbsTcs = "shaders/nurbs_tcs.glsl";
static const char* nurbsTes = "shaders/nurbs_tes.glsl";
static const char* colVS = "shaders/color_vertex.glsl";
static const char* colFS = "shaders/color_fragment.glsl";
static const char* waterVert = "shaders/water_vert.glsl";
static const char* waterFrag = "shaders/water_frag.glsl";

float scalex = 1, scaley = 1;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

bool bWireFrame = false;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0,
uniformOmniLightPos = 0, uniformFarPlane = 0, uniformTessellationLevel = 0,
uniformTime = 0;

void CreateObjects() {
	GLfloat controlpoints[] = {
	200, 100, 0, 0,
	222, 407, 0, 0,
	315, 587, 0, 0,
	684, 304, 0, 0,

	963, 387, 0, 0,
	1090, 615, 0, 0,
	671, 688, 0, 0,
	710, 507, 0, 0
	};



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
		//  x      y      z         r,g,b        u      v       nx,    ny,   nz
			-1.0f, -1.0f, -0.6f,   1,1,1,   0.0f, 0.0f,    0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,     1,1,1,   0.5f, 0.f,	  0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,    1,1,1,    1.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,      1,1,1,     0.5f, 1.0f,	  0.0f, 0.0f, 0.0f,
	};

	GLfloat oceanVertices[] = {
		//  x    y    z    r    g    b        u    v         nx    ny   nz
		1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.25f, 0.0f, 0.0f, -1.0f, 0.0f,
		3.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.50f, 0.0f, 0.0f, -1.0f, 0.0f,
		4.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,

		1.0f, 0.0f, 2.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.25f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 2.0f,  1.0f, 1.0f, 1.0f, 0.25f, 0.25f, 0.0f, -1.0f, 0.0f,
		3.0f, 0.0f, 2.0f,  1.0f, 1.0f, 1.0f, 0.50f, 0.25f, 0.0f, -1.0f, 0.0f,
		4.0f, 0.0f, 2.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f, -1.0f, 0.0f,

		1.0f, 0.0f, 3.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.50f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 3.0f,  1.0f, 1.0f, 1.0f, 0.25f, 0.50f, 0.0f, -1.0f, 0.0f,
		3.0f, 0.0f, 3.0f,  1.0f, 1.0f, 1.0f, 0.50f, 0.50f, 0.0f, -1.0f, 0.0f,
		4.0f, 0.0f, 3.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.50f, 0.0f, -1.0f, 0.0f,

		1.0f, 0.0f, 4.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 4.0f,  1.0f, 1.0f, 1.0f, 0.25f, 1.0f, 0.0f, -1.0f, 0.0f,
		3.0f, 0.0f, 4.0f,  1.0f, 1.0f, 1.0f, 0.50f, 1.0f, 0.0f, -1.0f, 0.0f,
		4.0f, 0.0f, 4.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f
	};


	GLuint oceanIndices[] = {
	0, 1, 5,
	0, 5, 4,
	1, 2, 6,
	1, 6, 5,
	2, 3, 7,
	2, 7, 6,
	4, 5, 9,
	4, 9, 8,
	5, 6, 10,
	5, 10, 9,
	6, 7, 11,
	6, 11, 10,
	8, 9, 13,
	8, 13, 12,
	9, 10, 14,
	9, 14, 13,
	10, 11, 15,
	10, 15, 14
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

	unsigned int axesIndices[] = {
		0, 1, // X-axis
		2, 3, // Y-axis
		4, 5  // Z-axis
	};

	GLfloat axesVertices[] = {
		// x   y      z     r      g     b     u    v    nx   ny   nz
		0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 1.f, 1.f, 0.f, 0.f, 0.f,
		1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
		0.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
		0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
		0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
		0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.f, 0.f, 0.f, 0.f, 0.f,

	};

	Utils::calcAverageNormal(indices, 12, vertices, 44, 11, 8);
	Utils::calcAverageNormal(boatIndices, 12, boatVertices, 11 * 12, 11, 8);
	Utils::calcAverageNormal(oceanIndices, 18 * 3, oceanVertices, 11 * 16, 11, 8);


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 44, 12);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 44, 12);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 44, 6);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(boatVertices, boatIndices, 11 * 12, 12);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(oceanVertices, oceanIndices, 11 * 16, 18 * 3);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(axesVertices, axesIndices, 11 * 6, 6);

	meshList.push_back(obj1);
	meshList.push_back(obj2);
	meshList.push_back(obj3);
	meshList.push_back(obj4);
	meshList.push_back(obj5);
	meshList.push_back(obj6);


}

void CreateShaders() {
	

	Shader* shaderPNT = new Shader();
	shaderPNT->CreateFromFiles(ptv, pTtcs, pTtes, fShader);
	shaderList.push_back(shaderPNT);

	Shader* shaderPNQ = new Shader();
	shaderPNQ->CreateFromFiles(ptv, pQtcs, pQtes, fShader);
	shaderList.push_back(shaderPNQ);

	Shader* shaderNurbs = new Shader();
	shaderNurbs->CreateFromFiles(ptv, nurbsTcs, nurbsTes, fShader);
	shaderList.push_back(shaderNurbs);

	Shader* axisShader = new Shader();
	axisShader->CreateFromFiles(colVS, colFS);
	shaderList.push_back(axisShader);

	Shader* waterShader = new Shader();
	waterShader->CreateFromFiles(waterVert, pTtcs, pTtes, waterFrag);
	shaderList.push_back(waterShader);

	/*Shader* waterShader = new Shader();
	waterShader->CreateFromFiles(waterVert, nurbsTcs, nurbsTes, fShader);
	shaderList.push_back(waterShader);*/

	/*Shader* axisShader1 = new Shader();
	axisShader1->CreateFromFiles(colVS, colFS);*/
	//shaderList.push_back(shaderPNQ);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("shaders/directional_shadow_map_vertex.glsl", "shaders/directional_shadow_map_fragment.glsl");
	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("shaders/omni_directional_shadow_map_vertex.glsl", "shaders/omni_directional_shadow_map_geometry.glsl",
		"shaders/omni_directional_shadow_map_fragment.glsl");
}

void RenderScene() {
	glm::mat4 model(1.0f);

	/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMesh();*/

	/*model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	matteMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMesh();*/

	/*model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMesh();*/

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-7.0f, 1.0f, 10.0f));
	//model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	boat1.RenderModel();


	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(7.0f, 1.0f, 10.0f));
	//model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	boat2.RenderModel();

	/*model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMesh();*/

	glUseProgram(0);
}

float boat1Z = 10.f, boat2Z = -10.f;
float speed = 0.1f;
bool firstRender = false;

void RenderSceneTess() {
	if (firstRender) {
		if ((boat1Z - boat1.GetWidth()) - speed * deltaTime > boat2Z) {
			boat1Z -= speed * deltaTime;
			boat1.minZ += speed * deltaTime;
			boat1.maxZ += speed * deltaTime;
		}

		if ((boat2Z + boat2.GetWidth()) + speed * deltaTime < boat1Z) {
			boat2Z += speed * deltaTime;
			boat2.minZ -= speed * deltaTime;
			boat2.maxZ -= speed * deltaTime;
		}
	}
	
	
	else {
		boat1Z -= speed * deltaTime;
		boat2Z += speed * deltaTime;
	}

	//cout << "boat1: " << boat1.maxZ << " " << boat1.minZ << std::endl;
	//cout << "boat2: " << boat2.maxZ << " " << boat2.minZ << std::endl;

	/*if ((boat1Z - boat1.GetWidth()) - speed * deltaTime > boat2Z) {
		boat1Z -= speed * deltaTime;
		boat1.minZ -= speed * deltaTime;
		boat1.maxZ -= speed * deltaTime;
	}

	if ((boat2Z + boat2.GetWidth()) + speed * deltaTime < boat1Z) {
		boat2Z += speed * deltaTime;
		boat2.minZ += speed * deltaTime;
		boat2.maxZ += speed * deltaTime;
	}*/

	glm::mat4 model(1.0f);

	/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMeshPatches(bWireFrame);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	matteMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMeshPatches(bWireFrame);*/

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 1.0f, boat1Z));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	boat1.RenderModelPatches(bWireFrame);

	if (firstRender == false) {
		boat1.minZ = model[2][0] * boat1.minZ + model[2][1] * boat1.minZ + model[2][2] * boat1.minZ + model[2][3];
		boat1.maxZ = model[2][0] * boat1.maxZ + model[2][1] * boat1.maxZ + model[2][2] * boat1.maxZ + model[2][3];
	}

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 1.0f, boat2Z));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	boat2.RenderModelPatches(bWireFrame);

	if (firstRender == false) {
		boat2.minZ = model[2][0] * boat2.minZ + model[2][1] * boat2.minZ + model[2][2] * boat2.minZ + model[2][3];
		boat2.maxZ = model[2][0] * boat2.maxZ + model[2][1] * boat2.maxZ + model[2][2] * boat2.maxZ + model[2][3];
	}

	firstRender = true;


	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//plainTexture.UseTexture();
	//glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	//meshList[3]->RenderMeshPatches(bWireFrame);

	//model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//plainTexture.UseTexture();
	//glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	////meshList[4]->RenderMeshPatches(bWireFrame);

	glUseProgram(0);
}

void RenderAxes() {
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[5]->RenderMeshLines();
}

void RenderOceanTess() {
	glm::mat4 model(1.0f);

	model = glm::scale(model, glm::vec3(10.f, 1.f, 10.f));
	model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	oceanTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[4]->RenderMeshPatches(bWireFrame);
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
	RenderOceanTess();

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
	RenderOceanTess();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec4 &&clipPlane) {
	glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skyBox.DrawSkybox(viewMatrix, projectionMatrix);

	//axes
	shaderList[3]->UseShader();

	uniformModel = shaderList[3]->GetModelLocation();
	uniformProjection = shaderList[3]->GetProjectionLocation();
	uniformView = shaderList[3]->GetViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	RenderAxes();

	glUseProgram(0);

	// pn triangles
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	shaderList[0]->UseShader();

	uniformModel = shaderList[0]->GetModelLocation();
	uniformProjection = shaderList[0]->GetProjectionLocation();
	uniformView = shaderList[0]->GetViewLocation();
	uniformEyePosition = shaderList[0]->GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
	uniformShininess = shaderList[0]->GetShininessLocation();
	uniformTessellationLevel = shaderList[0]->GetTesslationLevelLocation();


	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	
	shaderList[0]->SetTessellationLevel(tessLevel);

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
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0]->SetClipPlane(std::move(clipPlane));

	shaderList[0]->Validate();

	RenderSceneTess();


	glUseProgram(0);
}


void OceanRenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, WaterFrameBuffers &waterFBO) {
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	shaderList[4]->UseShader();

	uniformModel = shaderList[4]->GetModelLocation();
	uniformProjection = shaderList[4]->GetProjectionLocation();
	uniformView = shaderList[4]->GetViewLocation();
	uniformEyePosition = shaderList[4]->GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[4]->GetSpecularIntensityLocation();
	uniformShininess = shaderList[4]->GetShininessLocation();
	uniformTessellationLevel = shaderList[4]->GetTesslationLevelLocation();
	uniformTime = shaderList[4]->GetTime();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[4]->SetTime(glfwGetTime());

	shaderList[4]->SetTessellationLevel(16);

	shaderList[4]->SetDirectionalLight(&mainLight);
	shaderList[4]->SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[4]->SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	auto lightTansform = mainLight.CalcLightTransform();
	shaderList[4]->SetDirectionalLightTransform(&lightTansform);
	
	moveFactor += WAVE_SPEED * glfwGetTime() * 0.0001;
	moveFactor = fmod(moveFactor, 1.0);

	shaderList[4]->SetMoveFactor(moveFactor);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	//waterFBO.ReadTextures();

	shaderList[4]->SetTexture(1);
	shaderList[4]->SetDirectionalShadowMap(2);

	//shaderList[4]->SetReflectionTexture(3);
	//shaderList[4]->SetRefractionTexture(4);

	auto lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[4]->Validate();

	RenderOceanTess();

	glUseProgram(0);
}

int main() {
	mainWindow = Window(DISPLAY_WIDTH, DISPLAY_HEIGHT); // 1280, 1024 or 1024, 768
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
	oceanTexture = Texture("textures/ocean.png");
	oceanTexture.LoadTexture();

	glossyMaterial = Material(4.0f, 256);
	matteMaterial = Material(0.3f, 4);

	boat1 = Model();
	boat1.LoadModel("models/boat.obj");

	boat2 = Model();
	boat2.LoadModel("models/boat.obj");

	mainLight = DirectionalLight(
		0.5f, 0.5f, 1.0f,
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


	skyBoxFaces.push_back("textures/clouds/right.bmp");
	skyBoxFaces.push_back("textures/clouds/left.bmp");
	skyBoxFaces.push_back("textures/clouds/top.bmp");
	skyBoxFaces.push_back("textures/clouds/bot.bmp");
	skyBoxFaces.push_back("textures/clouds/back.bmp");
	skyBoxFaces.push_back("textures/clouds/front.bmp");


	skyBox = Skybox(skyBoxFaces);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	WaterFrameBuffers waterFBO = WaterFrameBuffers();

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

		/*glEnable(GL_CLIP_DISTANCE0);

		waterFBO.BindReflectionFrameBuffer();
		
		glm::vec3 cameraPos = camera.getCameraPosition();
		float distance = 2 * cameraPos.y - 15;
		cameraPos.y -= distance;
		camera.setCameraPosition(cameraPos);
		camera.invertPitch();
		RenderPass(camera.calculateViewMatrix(), projection, { 0, -1, 0, 15 });
		
		waterFBO.UnbindCurrentFrameBuffer();

		cameraPos.y += distance;
		camera.setCameraPosition(cameraPos);
		camera.invertPitch();

		waterFBO.BindRefractionFrameBuffer();
		RenderPass(camera.calculateViewMatrix(), projection, { 0, -1, 0, 15 });
		waterFBO.UnbindCurrentFrameBuffer();*/


		glDisable(GL_CLIP_DISTANCE0);

		RenderPass(camera.calculateViewMatrix(), projection, { 0, -1, 0, 10000 });
		OceanRenderPass(camera.calculateViewMatrix(), projection, waterFBO);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}