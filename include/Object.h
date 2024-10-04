#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "Mesh.h"
#include "Material.h"


class Object
{
public:
	//Object();
	Object(Mesh* mesh, Material* mat, glm::vec3 pos);
	~Object();

	Object* Instantiate(glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f));
	void ResetOrientation(float seconds = 0.0f);

public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 delta_rotation = glm::vec3(0.0f);
	glm::quat orientation;// = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	bool resettingOrientation;
	float timeToReset;
	float currentResetTime;

	Mesh* mesh;
	Material* mat;

	std::string name;
	unsigned int id;


	static inline Object* defaultObject;
};