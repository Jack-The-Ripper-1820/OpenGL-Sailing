#include <Mesh.hpp>
#include <iostream>

Mesh::Mesh() : model(1.f) {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

Mesh::Mesh(int ID) : Mesh()
{
	this->ID = ID;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	std::cout << minX << " " << minY << " " << minZ << std::endl <<
		maxX << " " << maxY << " " << maxZ << std::endl << std::endl;
	
	for (unsigned int i = 0; i < numOfVertices; i += 11) {
		minX = min(minX, vertices[i]);
		maxX = max(maxX, vertices[i]);

		minY = min(minY, vertices[i + 1]);
		maxY = max(maxY, vertices[i + 1]);

		minZ = min(minZ, vertices[i + 2]);
		maxZ = max(maxZ, vertices[i + 2]);
	}

	
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, 0);
	glEnableVertexAttribArray(0); // pos

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1); // color

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 6));
	glEnableVertexAttribArray(2); // texture

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 8));
	glEnableVertexAttribArray(3); // normal

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	minBounds = glm::vec3(minX, minY, minZ);
	maxBounds = glm::vec3(maxX, maxY, maxZ);

	minBounds = glm::vec3(glm::mat4(1.0) * glm::vec4(minBounds, 1));
	maxBounds = glm::vec3(glm::mat4(1.0) * glm::vec4(maxBounds, 1));


	collider = new Collider(minBounds, maxBounds, ID);  // compute minBounds and maxBounds based on the mesh's vertices
	std::cout << minX << " " << minY << " " << minZ << std::endl <<
		maxX << " " << maxY << " " << maxZ << std::endl << std::endl;

}


void Mesh::CreateWaterMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, 0);
	glEnableVertexAttribArray(0); // pos

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1); // color

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 6));
	glEnableVertexAttribArray(2); // texture

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 8));
	glEnableVertexAttribArray(3); // normal

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

pair<glm::vec3, glm::vec3> Mesh::GetBoundaries()
{
	return { glm::vec3(minX, minY, minZ), glm::vec3(maxX, maxY, maxZ) };
}




void Mesh::RenderMesh(bool bWireFrame) {
	bWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Mesh::RenderMeshPatches(bool bWireFrame) {
	bWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_PATCHES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Mesh::RenderMeshLines(bool bWireFrame) {
	bWireFrame ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Mesh::ClearMesh() {
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

void Mesh::CreateMeshFromControlPoints(GLfloat* controlpoints, unsigned int numcontrolpoints)
{
	glGenBuffers(1, &controlpointVBO);

	glBindBuffer(GL_ARRAY_BUFFER, controlpointVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(controlpoints[0]) * numcontrolpoints * 2, NULL, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &controlpointVAO);
	glBindVertexArray(controlpointVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, controlpointVBO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(controlpoints[0]) * 4, (const void*)0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::ConvertPointsToPatches(GLfloat* controlpoints)
{
	/*int numpoints = sizeof(controlpoints) / sizeof(controlpoints[0]);
	int numverts = numpoints * 2;

	glBindBuffer(GL_ARRAY_BUFFER, controlpointVBO);*/

}

bool Mesh::IsColliding(const Mesh* other)
{
	return collider->IsColliding(*(other->collider));
}

void Mesh::TransformCollider(glm::mat4 const& transform)
{
	collider->Transform(transform);
}

void Mesh::TranslateCollider(glm::vec3 const& dVector)
{
	collider->Translate(dVector);
}

Mesh::~Mesh() {
	ClearMesh();
	delete collider;
}