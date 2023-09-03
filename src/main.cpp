// OpenGL-Sailing.cpp : Defines the entry point for the application.
//

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <assimp/Importer.hpp>

#include <Window.hpp>
#include <Texture.hpp>
#include <Camera.hpp>
#include <Utils.hpp>

using namespace std;

Window mainWindow;
Camera camera;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;


GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

GLfloat deltaTime = 0.f;
GLfloat lastTime = 0.f;


const int gridSize = 4;
const float gridSpacing = 1.0f;
GLfloat oceanVertices[(gridSize + 1) * (gridSize + 1) * 11];
unsigned int oceanIndices[gridSize * gridSize * 6];

unsigned int axesIndices[] = {
		0, 1, // X-axis
		2, 3, // Y-axis
		4, 5  // Z-axis
};

GLfloat axesVertices[] = {
	0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
	1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
	0.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
	0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
	0.0f,  0.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.f, 0.f, 0.f, 0.f, 0.f,
	0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.f, 0.f, 0.f, 0.f, 0.f,

};


void CreateObjects()
{
	
	int vertexIndex = 0;
	int index = 0;
	const float waveAmplitude = 0.2f; // Amplitude of the lifted points

	for (int z = 0; z <= gridSize; ++z) {
		for (int x = 0; x <= gridSize; ++x) {
			float xPos = static_cast<float>(x) * gridSpacing;
			float zPos = static_cast<float>(z) * gridSpacing;

			// Vertex position (x, y, z)
			oceanVertices[vertexIndex++] = 1.0f + xPos;
			oceanVertices[vertexIndex++] = (x + z) % 2 ? waveAmplitude * sin(xPos  + zPos) : 0.f;
			oceanVertices[vertexIndex++] = 1.0f + zPos;

			// Colors (r, g, b)
			oceanVertices[vertexIndex++] = 0.0f; // r
			oceanVertices[vertexIndex++] = 0.0f; // g
			oceanVertices[vertexIndex++] = 1.0f; // b

			// Texture coordinates (u, v)
			oceanVertices[vertexIndex++] = 0.0f; // u
			oceanVertices[vertexIndex++] = 0.0f; // v

			// Normals (nx, ny, nz)
			oceanVertices[vertexIndex++] = 0.0f; // nx
			oceanVertices[vertexIndex++] = 0.0f; // ny
			oceanVertices[vertexIndex++] = 0.0f; // nz

			

			// Indices for triangles
			if (x < gridSize && z < gridSize) {
				int topLeft = (z * (gridSize + 1)) + x;
				int topRight = topLeft + 1;
				int bottomLeft = ((z + 1) * (gridSize + 1)) + x;
				int bottomRight = bottomLeft + 1;

				// First triangle in the quad
				oceanIndices[index++] = topLeft;
				oceanIndices[index++] = bottomLeft;
				oceanIndices[index++] = topRight;

				// Second triangle in the quad
				oceanIndices[index++] = topRight;
				oceanIndices[index++] = bottomLeft;
				oceanIndices[index++] = bottomRight;
			}
		}
	}

	Mesh* axesMesh = new Mesh();
	axesMesh->CreateMesh(axesVertices, axesIndices, 66, 6);
	meshList.push_back(axesMesh);

	Mesh* oceanMesh = new Mesh();
	oceanMesh->CreateMesh(oceanVertices, oceanIndices, (gridSize + 1) * (gridSize + 1) * 11, gridSize * gridSize * 6);
	meshList.push_back(oceanMesh);
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles("shaders/vertex.glsl", "shaders/fragment.glsl");
	shaderList.push_back(*shader1);

	/*Shader* shaderOcean = new Shader();
	shaderOcean->CreateFromFiles("shaders/nurbs_vertex.glsl", "shaders/nurbs_tcs.glsl", "shaders/nurbs_tes.glsl", "shaders/nurbs_fragment.glsl");
	shaderList.push_back(*shaderOcean);*/
}

void RenderScene() {

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[0]->RenderMeshLines(); //axes

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	meshList[1]->RenderMeshTriangles(); //ocean
}

void AxesRenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	glViewport(0, 0, 1366, 768);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	shaderList[0].Validate();

	RenderScene();

	//glm::mat4 model(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	//meshList[0]->RenderMeshLines(); //axes
}

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(11.0f, 2.0f, 11.0f), glm::vec3(0.0f, 1.0f, 0.0f), -135.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);


	while (!mainWindow.getShouldClose()) {
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		AxesRenderPass(camera.calculateViewMatrix(), projection);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}





