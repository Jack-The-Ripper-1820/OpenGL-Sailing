#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <Mesh.hpp>
#include <Texture.hpp>

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void RenderModelPatches(bool bWireFrame);
	void ClearModel();
	void SetModelMatrix(glm::mat4 const& matrix) { model = matrix; }
	~Model();

	glm::mat4 model;

	float minX = numeric_limits<float>::max();
	float minY = numeric_limits<float>::max();
	float minZ = numeric_limits<float>::max();
	float maxX = numeric_limits<float>::min();
	float maxY = numeric_limits<float>::min();
	float maxZ = numeric_limits<float>::min();

	float GetWidth() {
		return abs(maxZ - minZ) + 0.2f;
	}

	float GetHeight() {
		return abs(maxY - minY) + 0.2f;
	}

	float GetLength() {
		return abs(maxX - minX) + 0.2f;
	}

private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTexture;
	glm::vec3 position;
	glm::vec4 scale;
};

