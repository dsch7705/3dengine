#include "ObjectManager.h"

void ObjectManager::Draw(glm::mat4& view, glm::mat4& projection)
{
	lightingShader->setMat4("view", view, false);
	lightingShader->setMat4("projection", projection, false);

	for (auto& obj : objects)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, obj->position);
		model = glm::scale(model, obj->scale);
		lightingShader->setMat4("model", model, false);

		lightingShader->setVec3("material.ambient", obj->materialAmbient);
		lightingShader->setVec3("material.diffuse", obj->materialDiffuse);
		lightingShader->setVec3("material.specular", obj->materialSpecular);
		lightingShader->setFloat("material.shininess", obj->materialShininess);

		//lightingShader->setVec3("objectColor", obj->objectColor);

		obj->UseVertexArray();
		glDrawArrays(GL_TRIANGLES, 0, obj->NumVertices);
	}
}

void ObjectManager::RegisterObject(Object* obj)
{
	if (std::find(objects.begin(), objects.end(), obj) != objects.end())	// object already registered
		return;

	obj->id = objects.size();
	objects.push_back(obj);
}