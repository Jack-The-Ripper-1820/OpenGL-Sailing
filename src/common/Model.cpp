#include <Model.hpp>

Model::Model() : model(1.f), minBounds(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()), 
	maxBounds(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min())
{
	model = glm::mat4(1.f);
	position = glm::vec3(0.f, 0.f, 0.f);


}

Model::Model(int ID) : Model()
{
	this->ID = ID;
	cout << "Model: " << ID <<  endl;
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {
		unsigned int materialIndex = meshToTexture[i];

		if (materialIndex < textureList.size() && textureList[materialIndex]) {
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::RenderModelPatches(bool bWireFrame)
{
	for (size_t i = 0; i < meshList.size(); i++) {
		unsigned int materialIndex = meshToTexture[i];

		if (materialIndex < textureList.size() && textureList[materialIndex]) {
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMeshPatches(bWireFrame);
	}
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		printf("Model %s failed to load: %s \n", fileName.c_str(), importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);

	printf("Model %s loaded \n", fileName.c_str());

}


void Model::SetPosition(float x, float y, float z, GLuint const& uniformModel)
{
	model = glm::mat4(1.f);

	model = glm::translate(model, glm::vec3(x, y, z));
	posX = x, posY = y, posZ = z;

	collider.minX = model[2][0] * collider.minX + model[2][1] * collider.minX + model[2][2] * collider.minX + model[2][3];
	collider.minY = model[2][0] * collider.minY + model[2][1] * collider.minY + model[2][2] * collider.minY + model[2][3];
	collider.minZ = model[2][0] * collider.minZ + model[2][1] * collider.minZ + model[2][2] * collider.minZ + model[2][3];


	collider.maxX = model[2][0] * collider.maxX + model[2][1] * collider.maxX + model[2][2] * collider.maxX + model[2][3];
	collider.maxY = model[2][0] * collider.maxY + model[2][1] * collider.maxY + model[2][2] * collider.maxY + model[2][3];
	collider.maxZ = model[2][0] * collider.maxZ + model[2][1] * collider.maxZ + model[2][2] * collider.maxZ + model[2][3];
	
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void Model::SetRotation(float angle, glm::vec3 axis, GLuint const& uniformModel)
{
	model = glm::mat4(1.f);

	model = glm::rotate(model, glm::radians(angle), axis);

	collider.minX = model[2][0] * collider.minX + model[2][1] * collider.minX + model[2][2] * collider.minX + model[2][3];
	collider.minY = model[2][0] * collider.minY + model[2][1] * collider.minY + model[2][2] * collider.minY + model[2][3];
	collider.minZ = model[2][0] * collider.minZ + model[2][1] * collider.minZ + model[2][2] * collider.minZ + model[2][3];


	collider.maxX = model[2][0] * collider.maxX + model[2][1] * collider.maxX + model[2][2] * collider.maxX + model[2][3];
	collider.maxY = model[2][0] * collider.maxY + model[2][1] * collider.maxY + model[2][2] * collider.maxY + model[2][3];
	collider.maxZ = model[2][0] * collider.maxZ + model[2][1] * collider.maxZ + model[2][2] * collider.maxZ + model[2][3];

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void Model::SetScale(float x, float y, float z, GLuint const& uniformModel)
{
	model = glm::mat4(1.f);

	model = glm::scale(model, glm::vec3(x, y, z));

	collider.minX = model[2][0] * collider.minX + model[2][1] * collider.minX + model[2][2] * collider.minX + model[2][3];
	collider.minY = model[2][0] * collider.minY + model[2][1] * collider.minY + model[2][2] * collider.minY + model[2][3];
	collider.minZ = model[2][0] * collider.minZ + model[2][1] * collider.minZ + model[2][2] * collider.minZ + model[2][3];


	collider.maxX = model[2][0] * collider.maxX + model[2][1] * collider.maxX + model[2][2] * collider.maxX + model[2][3];
	collider.maxY = model[2][0] * collider.maxY + model[2][1] * collider.maxY + model[2][2] * collider.maxY + model[2][3];
	collider.maxZ = model[2][0] * collider.maxZ + model[2][1] * collider.maxZ + model[2][2] * collider.maxZ + model[2][3];

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

bool Model::IsColliding(const Model& other) const
{
	/*for (const auto& meshA : meshList) {
		for (const auto& meshB : other.meshList) {
			if (meshA->GetCollider()->IsColliding(*meshB->GetCollider())) {
				return true;
			}
		}
	}
	return false;*/

	std::cout << "in colliding" << std::endl;
	std::cout << "ID: " << ID << std::endl << minBounds.x << " " << minBounds.y << " " << minBounds.z << std::endl;
	std::cout << maxBounds.x << " " << maxBounds.y << " " << maxBounds.z << std::endl;
	std::cout << "other ID: " << other.ID << std::endl << other.minBounds.x << " " << other.minBounds.y << " " << other.minBounds.z << std::endl;
	std::cout << other.maxBounds.x << " " << other.maxBounds.y << " " << other.maxBounds.z << std::endl;

	return (minBounds.x <= other.maxBounds.x && maxBounds.x >= other.minBounds.x) &&
		(minBounds.y <= other.maxBounds.y && maxBounds.y >= other.minBounds.y) &&
		(minBounds.z <= other.maxBounds.z && maxBounds.z >= other.minBounds.z);
}

void Model::TransformColliders(glm::mat4 const& transformMatrix)
{

	for (const auto& meshA : meshList) {
		meshA->GetCollider()->Transform(transformMatrix);
	}

	minBounds = glm::vec3(transformMatrix * glm::vec4(minBounds, 1.0f));
	maxBounds = glm::vec3(transformMatrix * glm::vec4(maxBounds, 1.0f));

	//if (cnt < 3) {
	//	std::cout << "ID: " << ID << std::endl << minBounds.x << " " << minBounds.y << " " << minBounds.z << std::endl;
	//	std::cout << maxBounds.x << " " << maxBounds.y << " " << maxBounds.z << std::endl << std::endl;
	//	//cnt++;
	//}
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		
		if (mesh->mColors[0]) {
			vertices.insert(vertices.end(), { mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b});
		}

		else {
			vertices.insert(vertices.end(), { 1.0f, 1.0f, 1.0f });
		}

		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		
		//std::cout << " " << - mesh->mVertices[i].x << " " << -mesh->mVertices[i].y << " " << -mesh->mVertices[i].z << std::endl;

		//std::cout << " " << - mesh->mNormals[i].x << " " << -mesh->mNormals[i].y << " " << -mesh->mNormals[i].z << std::endl;
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });

	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh(ID);
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTexture.push_back(mesh->mMaterialIndex);
	minBounds = glm::min(minBounds, newMesh->minBounds);
	maxBounds = glm::max(maxBounds, newMesh->maxBounds);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTexture())
				{
					printf("Failed to load texture at: %s\n", texPath.c_str());
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i]) {
			textureList[i] = new Texture("textures/plain.png");
			textureList[i]->LoadTextureA();
		}
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++) {
		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
}
