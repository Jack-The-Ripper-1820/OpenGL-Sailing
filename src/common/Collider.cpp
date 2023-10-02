#include "Collider.hpp"

void Collider::SetCollider(GLfloat* vertices, unsigned int vertexCount, unsigned int vertexOffset)
{
	for (unsigned int i = 0; i < vertexCount; i += vertexOffset) {
		minX = std::min(minX, vertices[i]);
		maxX = std::max(maxX, vertices[i]);

		minY = std::min(minY, vertices[i + 1]);
		maxY = std::max(maxY, vertices[i + 1]);

		minZ = std::min(minZ, vertices[i + 2]);
		maxZ = std::max(maxZ, vertices[i + 2]);
	}
}
