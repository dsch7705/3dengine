#include "Mesh.h"


Mesh::Mesh(std::vector<float> vdata, std::vector<unsigned int> sizes)
{
	data = vdata;
	vertexAttribSizes = sizes;

	for (const unsigned int& size : vertexAttribSizes)
		vertexAttribStride += size;

	numVertices = data.size() / vertexAttribStride;

	LoadMesh();
	list.push_back(this);
}
Mesh::~Mesh()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

	list.erase(std::remove(list.begin(), list.end(), this), list.end());
}

void Mesh::LoadMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), &data.front(), GL_STATIC_DRAW);

	int offset = 0;
	for (unsigned int i = 0; i < vertexAttribSizes.size(); i++)
	{
		glVertexAttribPointer(i, vertexAttribSizes[i], GL_FLOAT, GL_FALSE, vertexAttribStride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);

		offset += vertexAttribSizes[i];
	}
}
void Mesh::use()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}