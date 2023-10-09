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
#include <Skybox.hpp>

std::vector<Mesh*> meshList;

Shader pntShader;
Shader pnqShader;
Shader directionalShadowShader;
Shader omniShadowShader;
Shader waterShader;
Shader axisShader;

Skybox skyBox;

Window mainWindow;
Camera camera;

Material glossyMaterial, matteMaterial;

Texture woodTexture;
Texture plainTexture;
Texture oceanTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;
float tessLevel = 1.f;

float moveFactor = 0;

static const char* vShader = "shaders/vertex.glsl";
static const char* fShader = "shaders/fragment.glsl";
static const char* pTtcs = "shaders/tri_tcs.glsl";
static const char* pTtes = "shaders/tri_tes.glsl";
static const char* ptv = "shaders/tess_vert.glsl";
static const char* pQtcs = "shaders/quad_tcs.glsl";
static const char* pQtes = "shaders/quad_tes.glsl";
static const char* colVS = "shaders/color_vertex.glsl";
static const char* colFS = "shaders/color_fragment.glsl";
static const char* waterVert = "shaders/water_vert.glsl";
static const char* waterFrag = "shaders/water_frag.glsl";
static const char* dirShadVert = "shaders/directional_shadow_map_vertex.glsl";
static const char* dirShadFrag = "shaders/directional_shadow_map_fragment.glsl";
static const char* oDirShadVert = "shaders/omni_directional_shadow_map_vertex.glsl";
static const char* oDirShadGeom = "shaders/omni_directional_shadow_map_geometry.glsl";
static const char* oDirShadFrag = "shaders/omni_directional_shadow_map_fragment.glsl";

unsigned int pointLightCount = 0;

bool bWireFrame = false;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0,
uniformOmniLightPos = 0, uniformFarPlane = 0, uniformTessellationLevel = 0,
uniformTime = 0;

void CreateObjects() {
	unsigned int indices[] = {
	0, 2, 1,
	2, 4, 1,
	4, 3, 1,
	3, 0, 1,
	0, 4, 2,
	//4, 3, 0
	};


	GLfloat vertices[] = {
		//  x      y      z         r,g,b        u      v       nx,    ny,   nz
			0.0f, 0.0f, -4.0f,     1,1,1,   0.5f, 1.0f,    0.0f, 0.0f, 0.0f,
			-0.5f, 0.0f, -1.0f,      1,1,1,   0.25f, 0.5f,	  0.0f, 0.0f, 0.0f,
			0.5f, 0.0f, -1.0f,      1,1,1,   0.75f, 0.5f,	  0.0f, 0.0f, 0.0f,
			-1.0f, 0.5f, -1.0f,      1,1,1,   0.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
			1.0f, 0.5f, -1.0f,       1,1,1,   1.0f, 0.0f,	  0.0f, 0.0f, 0.0f,
	};

	vector<pair<glm::vec3, glm::vec3>> edges1 = {
		{ glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(-0.5f, 0.0f, -1.0f)}, // 0 1
		{ glm::vec3(-0.5f, 0.0f, -1.0f), glm::vec3(0.5f, 0.0f, -1.0f)}, // 1 2
		{ glm::vec3(0.5f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -4.0f)}, // 2 0
		{ glm::vec3(-0.5f, 0.0f, -1.0f), glm::vec3(0.5f, 0.0f, -1.0f)}, // 1 2
		{ glm::vec3(0.5f, 0.0f, -1.0f), glm::vec3(1.0f, 0.5f, -1.0f)}, // 2 4
		{ glm::vec3(1.0f, 0.5f, -1.0f), glm::vec3(-1.0f, 0.5f, -1.0f)}, // 4 3
		{ glm::vec3(-1.0f, 0.5f, -1.0f), glm::vec3(-0.5f, 0.0f, -1.0f)}, // 3 1
		{ glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(-1.0f, 0.5f, -1.0f) }, // 0 3
		{ glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(1.0f, 0.5f, -1.0f)}, // 0 4
	};

	vector<pair<glm::vec3, glm::vec3>> edges2 = edges1;

	unsigned int sailIndices[] = {
		0,1,2,3
		//2,0,3
	};

	unsigned int sail2Indices[] = {
		0,1,2,3
		//2,0,3
	};


	GLfloat sailVertices[] = {
		0.0, 0.0, 0.0,  1.0,1.0,1.0,  0.0,0.0,  0.0,0.0,0.0,
		-1.0, 0.25, 1.0,  1.0,1.0,1.0,  0.0,0.5,  0.0,0.0,0.0,
		0.0, 0.0, 2.0,  1.0,1.0,1.0,  0.5,0.5,  0.0,0.0,0.0,
		1.0, 0.25, 1.0, 1.0,1.0,1.0,  1.0,1.0,  0.0,0.0,0.0
	};

	GLfloat sail2Vertices[] = {
		0.0, 0.0, 0.0,  1.0,1.0,1.0,  0.0,0.0,  0.0,0.0,0.0,
		-1.0, 0.25, 1.0,  1.0,1.0,1.0,  0.0,0.5,  0.0,0.0,0.0,
		0.0, 0.0, 2.0,  1.0,1.0,1.0,  0.5,0.5,  0.0,0.0,0.0,
		1.0, 0.25, 1.0, 1.0,1.0,1.0,  1.0,1.0,  0.0,0.0,0.0
	};

	 

	GLfloat oceanVertices[] = {
		//  x    y    z    r    g    b        u    v         nx    ny   nz
		-1.0f, 0.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f, 0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.50f, 0.0f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,

		-1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.25f, 0.0f, -1.0f, 0.0f,
		 0.0f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.25f, 0.0f, -1.0f, 0.0f,
		1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.50f, 0.25f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.0f, -1.0f, 0.0f,

		-1.0f, 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.50f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.25f, 0.50f, 0.0f, -1.0f, 0.0f,
		1.0f, 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.50f, 0.50f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 0.50f, 0.0f, -1.0f, 0.0f,

		-1.0f, 0.0f,  2.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f,  2.0f,  1.0f, 1.0f, 1.0f, 0.25f, 1.0f, 0.0f, -1.0f, 0.0f,
		1.0f, 0.0f,  2.0f,  1.0f, 1.0f, 1.0f, 0.50f, 1.0f, 0.0f, -1.0f, 0.0f,
		2.0f, 0.0f,  2.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f
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


	Utils::calcAverageNormal(indices, 3 * 5, vertices, 5 * 11, 11, 8);

	Utils::calcAverageNormal(oceanIndices, 18 * 3, oceanVertices, 11 * 16, 11, 8);

	Utils::calcAverageNormalForQuads(sailIndices, 4, sailVertices, 11 * 4, 11, 8);
	Utils::calcAverageNormalForQuads(sail2Indices, 4, sail2Vertices, 11 * 4, 11, 8);


	Mesh* obj1 = new Mesh(0);

	obj1->CreateMesh(vertices, indices, 5 * 11, 3 * 5);

	obj1->SetEdges(edges1);

	Mesh* obj2 = new Mesh(1);

	obj2->CreateMesh(vertices, indices, 5 * 11, 3 * 5);

	obj2->SetEdges(edges2);

	Mesh* obj3 = new Mesh(4);
	obj3->CreateMesh(oceanVertices, oceanIndices, 11 * 16, 18 * 3);

	Mesh* obj4 = new Mesh(5);
	obj4->CreateMesh(axesVertices, axesIndices, 11 * 6, 6);

	Mesh* obj5 = new Mesh(6);

	obj5->CreateMesh(sailVertices, sailIndices, 11 * 4, 4 * 1);

	Mesh* obj6 = new Mesh(7);

	obj6->CreateMesh(sail2Vertices, sail2Indices, 11 * 4, 4 * 1);


	meshList.push_back(obj1);
	meshList.push_back(obj2);
	meshList.push_back(obj3);
	meshList.push_back(obj4);
	meshList.push_back(obj5);
	meshList.push_back(obj6);

}

void CreateShaders() {
	pntShader = Shader();
	pntShader.CreateFromFiles(ptv, pTtcs, pTtes, fShader);

	pnqShader = Shader();
	pnqShader.CreateFromFiles(ptv, pQtcs, pQtes, fShader);

	axisShader = Shader();
	axisShader.CreateFromFiles(colVS, colFS);

	waterShader =  Shader();
	waterShader.CreateFromFiles(waterVert, pTtcs, pTtes, waterFrag);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles(dirShadVert, dirShadFrag);

	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles(oDirShadVert, oDirShadGeom, oDirShadFrag);
}

float boat1X = 12.0f, boat1Y = 0.5f, boat1Z = 2.f;
float boat2X = 2.0f, boat2Y = 0.5f, boat2Z = -10.f;
float speed = 0.1f;
bool bFirstRender = true;
bool bCollidedBoat1 = false, bCollidedBoat2 = false;

void RenderSceneTriTess() {
	glm::mat4 model(1.0f);

	float d1X = -speed * deltaTime;
	float d2Z = speed * deltaTime;
	float newBoat1X = boat1X + d1X;
	float newBoat2Z = boat2Z + d2Z;

	glm::mat4 model1(1.0f);
	model1 = glm::translate(model1, glm::vec3(newBoat1X, boat1Y, boat1Z));
	model1 = glm::rotate(model1, glm::radians(90.f), glm::vec3(0, 1, 0));

	glm::mat4 model2(1.0f);
	model2 = glm::translate(model2, glm::vec3(boat2X, boat2Y, newBoat2Z));
	model2 = glm::rotate(model2, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

	if (bFirstRender && !bCollidedBoat1 && !bCollidedBoat2) {
		meshList[0]->TransformCollider(model1);
		meshList[1]->TransformCollider(model2);
		bFirstRender = false;
	} 
	
	else if(!bCollidedBoat1 && !bCollidedBoat2) {
		meshList[0]->TranslateCollider(glm::vec3(d1X, 0, 0));
		meshList[1]->TranslateCollider(glm::vec3(0, 0, d2Z));
	}

	if (!bCollidedBoat1 && !bCollidedBoat2 && !meshList[0]->IsColliding(meshList[1])) {
		boat1X = newBoat1X;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));
	}

	else {
		model = glm::translate(model, glm::vec3(boat1X, boat1Y, boat1Z));
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(0, 1, 0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bCollidedBoat1 = true;
	}

	woodTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[0]->RenderMeshPatches(bWireFrame);


	if(!bCollidedBoat1 && !bCollidedBoat2 && !meshList[1]->IsColliding(meshList[0])) {
		boat2Z = newBoat2Z;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
	}

	else {
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(boat2X, boat2Y, boat2Z));
		model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bCollidedBoat2 = true;
	}

	woodTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[1]->RenderMeshPatches(bWireFrame);
}

void RenderSceneQuadTess() { 
	glm::mat4 model(1.f);

	model = glm::translate(model, glm::vec3(boat1X - 2, boat1Y, boat1Z));
	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[4]->RenderMeshPatches(bWireFrame);


	model = glm::mat4(1.f);

	model = glm::translate(model, glm::vec3(boat2X, boat2Y, boat2Z + 2));
	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	plainTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[5]->RenderMeshPatches(bWireFrame);
}

void RenderAxes() {
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[3]->RenderMeshLines();
}

void RenderOceanTess() {
	glm::mat4 model(1.0f);

	model = glm::scale(model, glm::vec3(10.5f, 1.f, 10.f));
	//model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	oceanTexture.UseTexture();
	glossyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[2]->RenderMeshPatches(bWireFrame);
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
	RenderSceneTriTess();
	RenderSceneQuadTess();
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
	RenderSceneTriTess();
	RenderSceneQuadTess();
	RenderOceanTess();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec4 clipPlane) {
	glViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skyBox.DrawSkybox(viewMatrix, projectionMatrix);

	//axes
	axisShader.UseShader();

	uniformModel = axisShader.GetModelLocation();
	uniformProjection = axisShader.GetProjectionLocation();
	uniformView = axisShader.GetViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	RenderAxes();

	glUseProgram(0);

	// pn triangles
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	pntShader.UseShader();

	uniformModel = pntShader.GetModelLocation();
	uniformProjection = pntShader.GetProjectionLocation();
	uniformView = pntShader.GetViewLocation();
	uniformEyePosition = pntShader.GetEyePositionLocation();
	uniformSpecularIntensity = pntShader.GetSpecularIntensityLocation();
	uniformShininess = pntShader.GetShininessLocation();
	uniformTessellationLevel = pntShader.GetTesslationLevelLocation();


	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
	
	pntShader.SetTessellationLevel(tessLevel);

	pntShader.SetDirectionalLight(&mainLight);
	pntShader.SetPointLights(pointLights, pointLightCount, 3, 0);
	auto lightTansform = mainLight.CalcLightTransform();
	pntShader.SetDirectionalLightTransform(&lightTansform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	pntShader.SetTexture(1);
	pntShader.SetDirectionalShadowMap(2);

	pntShader.SetClipPlane(clipPlane);

	pntShader.Validate();

	RenderSceneTriTess();

	glUseProgram(0);

	// pn quads
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	pnqShader.UseShader();

	uniformModel = pnqShader.GetModelLocation();
	uniformProjection = pnqShader.GetProjectionLocation();
	uniformView = pnqShader.GetViewLocation();
	uniformEyePosition = pnqShader.GetEyePositionLocation();
	uniformSpecularIntensity = pnqShader.GetSpecularIntensityLocation();
	uniformShininess = pnqShader.GetShininessLocation();
	uniformTessellationLevel = pnqShader.GetTesslationLevelLocation();


	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	pnqShader.SetTessellationLevel(tessLevel);

	pnqShader.SetDirectionalLight(&mainLight);
	pnqShader.SetPointLights(pointLights, pointLightCount, 3, 0);
	lightTansform = mainLight.CalcLightTransform();
	pnqShader.SetDirectionalLightTransform(&lightTansform);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	pnqShader.SetTexture(1);
	pnqShader.SetDirectionalShadowMap(2);


	pnqShader.SetClipPlane(clipPlane);

	pnqShader.Validate();

	RenderSceneQuadTess();

	glUseProgram(0);
}


void OceanRenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	glPatchParameteri(GL_PATCH_VERTICES, 3);

	waterShader.UseShader();

	uniformModel = waterShader.GetModelLocation();
	uniformProjection = waterShader.GetProjectionLocation();
	uniformView = waterShader.GetViewLocation();
	uniformEyePosition = waterShader.GetEyePositionLocation();
	uniformSpecularIntensity = waterShader.GetSpecularIntensityLocation();
	uniformShininess = waterShader.GetShininessLocation();
	uniformTessellationLevel = waterShader.GetTesslationLevelLocation();
	uniformTime = waterShader.GetTime();
	
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	waterShader.SetTime(glfwGetTime());

	waterShader.SetTessellationLevel(20.f);

	waterShader.SetDirectionalLight(&mainLight);
	waterShader.SetPointLights(pointLights, pointLightCount, 3, 0);
	auto lightTansform = mainLight.CalcLightTransform();
	waterShader.SetDirectionalLightTransform(&lightTansform);

	moveFactor += WAVE_SPEED * glfwGetTime() * 0.0001;
	moveFactor = fmod(moveFactor, 1.0);

	waterShader.SetMoveFactor(moveFactor);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);

	waterShader.SetTexture(1);
	waterShader.SetDirectionalShadowMap(2);

	waterShader.SetCameraPosition(camera.getCameraPosition());

	waterShader.Validate();

	RenderOceanTess();

	glUseProgram(0);
}


int main() {
	mainWindow = Window(DISPLAY_WIDTH, DISPLAY_HEIGHT); 
	mainWindow.Initialize();

	CreateShaders();
	CreateObjects();


	camera = Camera(glm::vec3(-3.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 5.0f, 0.5f);

	woodTexture = Texture("textures/Wood.jfif");
	woodTexture.LoadTexture();
	plainTexture = Texture("textures/plain.png");
	plainTexture.LoadTextureA();
	oceanTexture = Texture("textures/ocean.png");
	oceanTexture.LoadTexture();

	glossyMaterial = Material(4.0f, 256);
	matteMaterial = Material(0.3f, 4);

	glm::vec3 skyblue(glm::clamp(135.f * 0.8f, 0.f, 1.f),
		glm::clamp(206.f * 0.8f, 0.f, 1.f),
		glm::clamp(255.f * 0.8f, 0.f, 1.f));

	mainLight = DirectionalLight(
		skyblue.r, skyblue.g, skyblue.b,
		0.6f, 0.9f,
		//-10.0f, -12.0f, -18.5f,
		-20.0f, -20.0f, 18.5f,
		2048, 2048);

	pointLights[0] = PointLight( 
		0.400, 0.600, 1.000,
		0.0f, 0.4f,
		-2.0f, 2.0f, 0.0f,
		0.3f, 0.01f, 0.01f,
		1024, 1024,
		0.1f, 100.0f);

	pointLightCount++;

	std::vector<std::string> skyBoxFaces;


	skyBoxFaces.push_back("textures/clouds/right.bmp");
	skyBoxFaces.push_back("textures/clouds/left.bmp");
	skyBoxFaces.push_back("textures/clouds/top.bmp");
	skyBoxFaces.push_back("textures/clouds/bot.bmp");
	skyBoxFaces.push_back("textures/clouds/back.bmp");
	skyBoxFaces.push_back("textures/clouds/front.bmp");


	skyBox = Skybox(skyBoxFaces);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	while (!mainWindow.getShouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();


		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		if (mainWindow.getKeys()[GLFW_KEY_UP]) {
			tessLevel = std::min(tessLevel + 1, 20.f);
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

		glDisable(GL_CLIP_DISTANCE0);

		RenderPass(camera.calculateViewMatrix(), projection, { 0, -1, 0, 10000 });
		OceanRenderPass(camera.calculateViewMatrix(), projection);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}