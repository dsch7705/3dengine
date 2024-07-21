#include "Object.h"

Object::Object()
{
	//ObjectManager::RegisterObject(this);
}

Object::Object(unsigned int vao, unsigned int vbo)
{
	VertexArrayObject = vao;
	VertexBufferObject = vbo;

	//ObjectManager::RegisterObject(this);
}

Object::~Object()
{
}

Object* Object::Instantiate(Object& base, glm::vec3 position)
{
	Object* obj = new Object(base.VertexArrayObject, base.VertexBufferObject);
	obj->position = position;
	obj->NumVertices = base.NumVertices;
	obj->SetShader(base.GetShader());

	obj->materialAmbient = glm::vec3(base.materialAmbient);
	obj->materialDiffuse = glm::vec3(base.materialDiffuse);
	obj->materialSpecular = base.materialSpecular;
	obj->materialShininess = base.materialShininess;

	return obj;
}

void Object::LoadVertexData(VertexData& data)
{
	glGenVertexArrays(1, &VertexArrayObject);
	glGenBuffers(1, &VertexBufferObject);

	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.data.size(), &data.data.front(), GL_STATIC_DRAW);

	for (unsigned int i = 0; i < data.vertexAttribSizes.size(); i++)
	{
		int offset = (i == 0 ? 0 : data.vertexAttribSizes[i - 1]);
		glVertexAttribPointer(i, data.vertexAttribSizes[i], GL_FLOAT, GL_FALSE, data.vertexAttribStride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
	}

	NumVertices = data.numVertices;
}

void Object::UseVertexArray()
{
	glBindVertexArray(VertexArrayObject);
}

void Object::SetShader(Shader& shader)
{
	m_shader = shader;
}

Shader& Object::GetShader()
{
	return m_shader;
}