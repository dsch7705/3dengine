#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Object.h"

class Light
{
public:
	Light(glm::vec3 scale, unsigned int vao, unsigned int vbo, 
		Shader* shader = nullptr,
		glm::vec3 ambient = glm::vec3(141.0f / 255.0f, 141.0f / 255.0f, 141.0f / 255.0f),
		glm::vec3 diffuse = glm::vec3(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f),
		glm::vec3 specular = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f));

	~Light();

public:
	glm::vec3 m_lightAmbient;
	glm::vec3 m_lightDiffuse;
	glm::vec3 m_lightSpecular;

	Shader* m_lightSourceShader;
};