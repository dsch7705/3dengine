#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>


struct Mesh {
	std::string name;
	std::vector<float> data;

	std::vector<unsigned int> vertexAttribSizes;
	unsigned int vertexAttribStride = 0;
	unsigned int numVertices = 0;
	unsigned int VAO, VBO;

	Mesh(std::vector<float> vdata, std::vector<unsigned int> sizes);
	~Mesh();
	void LoadMesh();
	void use();

	static inline Mesh* defaultMesh;
	static inline std::vector<Mesh*> list;
};