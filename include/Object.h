#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"


class Object
{
public:
	//Object();
	Object(Mesh* mesh, Material* mat, glm::vec3 pos);
	~Object();

	Object* Instantiate(glm::vec3 position);

public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	Mesh* mesh;
	Material* mat;

	std::string name;
	unsigned int id;


	static inline Object* defaultObject;
};