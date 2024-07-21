#include "Scene.h"


Scene::Scene()
{
	m_mainCamera = new Camera();
	m_lightingShader = new Shader("res/shaders/Lighting.shader");
}
Scene::~Scene()
{
	delete(m_mainCamera);
	delete(m_lightingShader);

	for (auto& obj : m_objects)
		delete(obj);
}

void Scene::DrawObject(Object* obj, Shader* shader)
{
	obj->UseVertexArray();
	shader->use();
	glDrawArrays(GL_TRIANGLES, 0, obj->NumVertices);
}
void Scene::DrawObjects()
{
	// Lighting
	m_lightingShader->setVec3("lightPos", m_light->position);

	m_lightingShader->setVec3("light.ambient", m_light->m_lightAmbient);
	m_lightingShader->setVec3("light.diffuse", m_light->m_lightDiffuse);
	m_lightingShader->setVec3("light.specular", m_light->m_lightSpecular);

	// Calculate and set view matrix
	glm::mat4 view = m_mainCamera->GetViewMatrix();
	m_lightingShader->setMat4("view", view, false);
	m_lightingShader->setVec3("viewPos", m_mainCamera->Position);

	// Calculate and set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 500.0f);
	m_lightingShader->setMat4("projection", projection, false);

	// Draw objects
	for (const auto& obj : m_objects)
	{
		// Calculate and set model matrix per object
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, obj->position);
		model = glm::scale(model, obj->scale);
		m_lightingShader->setMat4("model", model, false);

		// Set material attributes
		m_lightingShader->setVec3("material.ambient", obj->materialAmbient);
		m_lightingShader->setVec3("material.diffuse", obj->materialDiffuse);
		m_lightingShader->setVec3("material.specular", obj->materialSpecular);
		m_lightingShader->setFloat("material.shininess", obj->materialShininess);

		// Draw
		m_lightingShader->use();
		obj->UseVertexArray();
		glDrawArrays(GL_TRIANGLES, 0, obj->NumVertices);
	}

	// Draw light
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_light->position);
	model = glm::scale(model, glm::vec3(0.2f));

	m_light->m_lightSourceShader->setMat4("view", view, false);
	m_light->m_lightSourceShader->setMat4("projection", projection, false);
	m_light->m_lightSourceShader->setMat4("model", model, false);

	m_light->m_lightSourceShader->use();
	m_light->UseVertexArray();

}

void Scene::RegisterObject(Object* obj)
{
	if (std::find(m_objects.begin(), m_objects.end(), obj) != m_objects.end())	// object already registered
		return;

	obj->id = m_objects.size();
	m_objects.push_back(obj);
}