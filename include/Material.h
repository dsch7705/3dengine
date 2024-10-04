#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include "Texture.h"


struct Material
{
public:
	Texture* diffuseMap;
	Texture* specularMap;
	float shininess;
	glm::vec3 tint;

	Material(Texture* diffuseMap, Texture* specularMap, float shininess, glm::vec3 tint = glm::vec3(1.0f));
	~Material();
	static void BatchDelete(std::vector<Material*> mats);
	std::pair<std::string, unsigned int> GetKey() { return key; }

	static void GenerateDefaultMaterial();

	std::string filepath;
	static inline Material* defaultMaterial;

	struct keyCmp
	{
		bool operator()(std::pair<std::string, unsigned int> a, std::pair<std::string, unsigned int> b) const { return a.second < b.second; }
	};
	static inline std::map<std::pair<std::string, unsigned int>, Material*, keyCmp> map;

private:
	Material();
	std::pair<std::string, unsigned int> key;
};