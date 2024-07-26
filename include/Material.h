#pragma once
#include <glm/glm.hpp>
#include "Texture.h"


struct Material
{
public:
	std::string name = "";
	Texture* diffuseMap;
	Texture* specularMap;
	float shininess;
	glm::vec3 tint;

	Material(Texture* diffuseMap, Texture* specularMap, float shininess, glm::vec3 tint = glm::vec3(1.0f));
	~Material();

	static inline Material* defaultMaterial;
	static inline std::vector<Material*> list;
};