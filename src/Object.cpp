#include "Object.h"

Object::Object(Mesh* mesh, Material* mat, glm::vec3 pos)
{
	this->mesh = mesh;
	this->mat = mat;
	position = pos;
}
Object::~Object()
{
}

Object* Object::Instantiate(glm::vec3 position, glm::vec3 scale)
{
	Object* obj = new Object(mesh, mat, position);
	obj->scale = scale;

	return obj;
}

void Object::ResetOrientation(float seconds)
{
	timeToReset = seconds;
	currentResetTime = 0.0f;
	resettingOrientation = true;
}