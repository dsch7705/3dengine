#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Shader.h"
#include "ObjectManager.h"

struct VertexData {
	std::vector<float> data;

	std::vector<unsigned int> vertexAttribSizes;
	unsigned int vertexAttribStride = 0;

	unsigned int numVertices = 0;

	VertexData() {};
	VertexData(std::vector<float> vdata, std::vector<unsigned int> sizes)
	{
		data = vdata;
		vertexAttribSizes = sizes;

		for (const unsigned int& size : vertexAttribSizes)
			vertexAttribStride += size;

		numVertices = data.size() / vertexAttribStride;
	}
};

class Object
{
public:
	Object();
	Object(unsigned int vao, unsigned int vbo);
	~Object();

	static Object* Instantiate(Object& base, glm::vec3 position);

	void LoadVertexData(VertexData& data);
	void UseVertexArray();
	void SetShader(Shader& shader);
	Shader& GetShader();

public:
	unsigned int VertexArrayObject;
	unsigned int VertexBufferObject;
	unsigned int NumVertices;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::vec3 materialAmbient = glm::vec3(4.0f / 255.0f, 124.0f / 255.0f, 148.0f / 255.0f);
	glm::vec3 materialDiffuse = glm::vec3(0.0f / 255.0f, 90.0f / 255.0f, 255.0f / 255.0f);
	glm::vec3 materialSpecular = glm::vec3(209.0f / 255.0f, 209.0f / 255.0f, 209.0f / 255.0f);
	float	  materialShininess = 290.0f;

	unsigned int id;

private:
	Object(unsigned int vao, unsigned int vbo);

private:

	Shader m_shader;
};

class Box : Object
{

};