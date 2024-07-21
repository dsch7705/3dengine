#pragma once
#include <vector>
#include "Object.h"
#include "Shader.h"

class Object;

class ObjectManager
{
public:
	ObjectManager() {};

	void Draw(glm::mat4& view, glm::mat4& projection);
	void RegisterObject(Object* obj);

	std::vector<Object*> objects;
};