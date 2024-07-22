#include "Scene.h"


Scene::Scene()
{
	m_mainCamera = new Camera();

	m_lightingShader = new Shader("res/shaders/Lighting.shader");
	m_lightSourceShader = new Shader("res/shaders/LightCube.shader");

	m_lightingShader->setMat4("projection", projectionMatrix, false);
	m_lightSourceShader->setMat4("projection", projectionMatrix, false);
}
Scene::~Scene()
{
	delete(m_mainCamera);
	delete(m_lightingShader);
	delete(m_lightSourceShader);

	for (auto& obj : m_objects)
		delete(obj);
}

void Scene::DrawObjects()
{
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
}
void Scene::Draw()
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// update lighting shader
	m_lightingShader->setVec3("lightPos", m_lightPosition);
	m_lightingShader->setVec3("light.ambient", m_lightAmbient);
	m_lightingShader->setVec3("light.diffuse", m_lightDiffuse);
	m_lightingShader->setVec3("light.specular", m_lightSpecular);

	DrawObjects();

	// Draw light
	if (m_objects.empty())
		return;

	m_lightSourceShader->use();
	m_lightSourceShader->setVec3("lightColor", m_lightDiffuse);

	m_objects[0]->UseVertexArray();
	glDrawArrays(GL_TRIANGLES, 0, m_objects[0]->NumVertices);
}

void Scene::RegisterObject(Object* obj)
{
	if (std::find(m_objects.begin(), m_objects.end(), obj) != m_objects.end())	// object already registered
		return;

	obj->id = m_objects.size();
	m_objects.push_back(obj);
}
void Scene::UpdateCamera(float deltaTime)
{
	m_mainCamera->ProcessGamepadInput(deltaTime);
	viewMatrix = m_mainCamera->GetViewMatrix();

	m_lightingShader->setMat4("view", viewMatrix, false);
	m_lightingShader->setVec3("viewPos", m_mainCamera->Position);

	m_lightSourceShader->setMat4("view", viewMatrix, false);

}