#include "Scene.h"


Scene::Scene()
{
	m_mainCamera = new Camera();

	m_lightingShader = new Shader("res/shaders/Lighting.shader");
	m_lightSourceShader = new Shader("res/shaders/LightCube.shader");
	m_outlineShader = new Shader("res/shaders/outline.shader");

	m_lightingShader->setMat4("projection", projectionMatrix, false);
	m_lightSourceShader->setMat4("projection", projectionMatrix, false);
	m_outlineShader->setMat4("projection", projectionMatrix, false);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw objects
	for (const auto& obj : m_objects)
	{
		glStencilMask(0x00);
		// Calculate and set model matrix per object
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, obj->position);
		model = glm::scale(model, obj->scale);
		m_lightingShader->setMat4("model", model, false);

		// Set material attributes
		obj->mat->diffuseMap->SetTextureUnit(GL_TEXTURE0);
		obj->mat->specularMap->SetTextureUnit(GL_TEXTURE1);
		m_lightingShader->setInt("material.diffuse", 0);
		m_lightingShader->setInt("material.specular", 1);

		m_lightingShader->setFloat("material.shininess", obj->mat->shininess);
		m_lightingShader->setVec3("material.tintColor", obj->mat->tint);

		// Draw
		m_lightingShader->use();
		obj->mesh->use();
		if (obj == Editor::selected)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glDrawArrays(GL_TRIANGLES, 0, obj->mesh->numVertices);

			continue;
		}
		glDrawArrays(GL_TRIANGLES, 0, obj->mesh->numVertices);
	}

	Editor::pickObjectPass();
	if (Editor::selectedObject != nullptr && Editor::selected == Editor::selectedObject)
	{

		glDisable(GL_DEPTH_TEST);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Editor::selectedObject->position);
		model = glm::scale(model, Editor::selectedObject->scale * 1.025f);
		m_outlineShader->setMat4("model", model, false);
		m_outlineShader->use();
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDrawArrays(GL_TRIANGLES, 0, Editor::selectedObject->mesh->numVertices);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}
}

void Scene::RegisterObject(Object* obj)
{
	if (std::find(m_objects.begin(), m_objects.end(), obj) != m_objects.end())	// object already registered
		return;

	obj->id = m_objects.size();
	obj->name = std::string("Object ").append(std::to_string(obj->id));
	m_objects.push_back(obj);
}
void Scene::UpdateCamera(float deltaTime)
{
	m_mainCamera->ProcessGamepadInput(deltaTime);
	viewMatrix = m_mainCamera->GetViewMatrix();

	m_lightingShader->setMat4("view", viewMatrix, false);
	m_lightingShader->setVec3("viewPos", m_mainCamera->Position);

	m_lightSourceShader->setMat4("view", viewMatrix, false);

	m_outlineShader->setMat4("view", viewMatrix, false);
}
void Scene::UpdateProjectionMatrix(float fov, float aspectRatio)
{
	projectionMatrix = glm::perspective(fov, aspectRatio, 0.1f, 500.0f);

	m_lightingShader->setMat4("projection", projectionMatrix, false);
	m_lightSourceShader->setMat4("projection", projectionMatrix, false);
	m_outlineShader->setMat4("projection", projectionMatrix, false);
}