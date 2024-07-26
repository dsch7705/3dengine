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

Object* Object::Instantiate(glm::vec3 position)
{
	return new Object(mesh, mat, position);
}