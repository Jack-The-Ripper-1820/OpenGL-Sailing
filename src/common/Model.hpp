#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <limits>

#include <Mesh.hpp>
#include <Texture.hpp>
#include <Collider.hpp>

class Model
{
public:
	Model();
	Model(int ID);
	int cnt = 0;
	glm::vec3 minBounds, maxBounds;
	void LoadModel(const std::string& fileName);
	void RenderModel();
	void RenderModelPatches(bool bWireFrame);
	void ClearModel();
	void SetModelMatrix(glm::mat4 const& matrix) { model = matrix; }
	~Model();

	glm::mat4 model;
	Collider collider;

	void SetPosition(float x, float y, float z, GLuint const& uniformModel);
	void SetRotation(float angle, glm::vec3 axis, GLuint const& uniformModel);
	void SetScale(float x, float y, float z, GLuint const& uniformModel); 
	float posX = 0, posY = 0, posZ = 0;

	bool IsColliding(const Model& other) const;
	void TransformColliders(glm::mat4 const& transformMatrix);
	int ID;

private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

	bool InitFlag = true;
	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTexture;
	glm::vec3 position;
	glm::vec4 scale;
};

