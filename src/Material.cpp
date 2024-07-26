#include "Material.h"


Material::Material(Texture* diffuseMap, Texture* specularMap, float shininess, glm::vec3 tint)
{
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;
	this->shininess = shininess;
	this->tint = tint;
	this->name = "Material " + std::to_string(list.size());
	list.push_back(this);
}
Material::~Material()
{
	list.erase(std::remove(list.begin(), list.end(), this), list.end());
}