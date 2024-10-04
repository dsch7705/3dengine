#include "Material.h"


Material::Material(Texture* diffuseMap, Texture* specularMap, float shininess, glm::vec3 tint)
{
	this->diffuseMap = diffuseMap;
	this->specularMap = specularMap;
	this->shininess = shininess;
	this->tint = tint;
	
	key.second = map.size();
	key.first = "Material " + std::to_string(key.second);
	//filepath = FileIO::materialDirectory + "/Material-" + std::to_string(map.size()) + ".mat";
	map[key] = this;
}
Material::~Material()
{
}
void Material::BatchDelete(std::vector<Material*> mats)
{
	std::map<std::pair<std::string, unsigned int>, Material*, keyCmp> updatedMap;
	for (auto it = map.begin(); it != map.end(); ++it)
	{
		const auto matitr = std::find(mats.begin(), mats.end(), it->second);
		if (matitr != mats.end() && *matitr != Material::defaultMaterial)
		{
			delete *matitr;
			continue;
		}

		it->second->key.second = updatedMap.size();
		updatedMap[it->second->key] = it->second;
	}

	map = updatedMap;
}
Material::Material()
{
	diffuseMap = Texture::defaultTexture;
	specularMap = Texture::defaultTexture;
	shininess = 32.0f;
	tint = glm::vec3(1.0f);

	key.second = map.size();
	key.first = "default";
	map[key] = this;
}

void Material::GenerateDefaultMaterial()
{
	if (defaultMaterial != nullptr)
	{
		std::cout << "Default material already initialized!\n";
		return;
	}
	if (Texture::defaultTexture == nullptr)
	{
		std::cout << "Default texture is uninitialized, cannot create default material!\n";
		return;
	}

	defaultMaterial = new Material();
}