#include "Light.h"


Light::Light(glm::vec3 s, unsigned int vao, unsigned int vbo, Shader* shader, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
	if (shader == nullptr)
		m_lightSourceShader = new Shader("res/shaders/LightCube.shader");
	else
		m_lightSourceShader = shader;

	m_lightAmbient = ambient;
	m_lightDiffuse = diffuse;
	m_lightSpecular = specular;

}
Light::~Light()
{
}

