#include "Renderer.h"


void Renderer::Setup(unsigned int viewW, unsigned int viewH)
{
	m_viewportWidth = viewW;
	m_viewportHeight = viewH;

	m_lightingShader		=  new Shader("res/shaders/Lighting.shader");
	m_lightSourceShader		=  new Shader("res/shaders/LightCube.shader");
	m_outlineShader			=  new Shader("res/shaders/outline.shader");
	m_pickObjectShader		=  new Shader("res/shaders/pick.shader");

	SetShadersProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)m_viewportWidth / (float)m_viewportHeight, 0.1f, 500.0f));

	m_lightingShader->setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	m_lightingShader->setVec3("light.ambient", glm::vec3(0.6f, 0.6f, 0.6f));
	m_lightingShader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	m_lightingShader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// Set up frame buffer for object picking
	glGenFramebuffers(1, &m_pickObjectFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_pickObjectFBO);

	glGenTextures(1, &m_pickObjectColorTex);
	glBindTexture(GL_TEXTURE_2D, m_pickObjectColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, m_viewportWidth, m_viewportHeight, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_viewportWidth, m_viewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickObjectColorTex, 0);

	glGenTextures(1, &m_pickObjectDepthTex);
	glBindTexture(GL_TEXTURE_2D, m_pickObjectDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_viewportWidth, m_viewportHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pickObjectDepthTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error (pick object)" << std::endl;

	Editor::pickObjFramebuffer = m_pickObjectFBO;
	
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void Renderer::SetCurrentScene(Scene* scene)
{
	m_currentScene = scene;
}

void Renderer::RenderScene(Scene* scene)
{
	if (scene == nullptr)
	{
		std::cout << "No current scene set OR scene object no longer exists.\n";
		return;
	}

	SetShadersViewMatrix(scene->m_mainCamera->GetViewMatrix());

	glm::vec3 lightDirection = glm::vec3(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f) * glm::toMat4(glm::quat(glm::radians(scene->m_dirLight.eulerAngles))));
	m_lightingShader->setVec3("dirLight.direction", lightDirection);
	m_lightingShader->setVec3("dirLight.ambient", scene->m_dirLight.ambient);
	m_lightingShader->setVec3("dirLight.diffuse", scene->m_dirLight.diffuse);
	m_lightingShader->setVec3("dirLight.specular", scene->m_dirLight.specular);

	m_lightingShader->setInt("numPointLights", scene->m_pointLights.size());
	for (int i = 0; i < scene->m_pointLights.size(); ++i)
	{
		std::string uniformName = "pointLights[" + std::to_string(i) + "].";
		m_lightingShader->setVec3((uniformName + "position").c_str(), scene->m_pointLights[i].position);
		m_lightingShader->setFloat((uniformName + "radius").c_str(), scene->m_pointLights[i].radius);

		m_lightingShader->setVec3((uniformName + "ambient").c_str(), scene->m_pointLights[i].ambient);
		m_lightingShader->setVec3((uniformName + "diffuse").c_str(), scene->m_pointLights[i].diffuse);
		m_lightingShader->setVec3((uniformName + "specular").c_str(), scene->m_pointLights[i].specular);
	}

	// Objects
	for (const auto& obj : scene->m_objects)
	{
		glStencilMask(0x00);

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		for (int i = 0; i < 3; i++)
		{
			glm::vec3 axis(0.0f);
			axis[i] = 1.0f;
			glm::quat q1(cos(glm::radians(obj->delta_rotation[i]) / 2.0f), sin(glm::radians(obj->delta_rotation[i]) / 2.0f) * axis);

			obj->orientation = q1 * obj->orientation;
		}
		obj->orientation = glm::normalize(obj->orientation);
		obj->delta_rotation = glm::vec3(0.0f);

		glm::quat orientation = obj->orientation;
		if (obj->resettingOrientation)
		{
			if (obj->currentResetTime >= obj->timeToReset)
			{
				obj->resettingOrientation = false;
				obj->currentResetTime = 0.0f;
				obj->timeToReset = 0.0f;

				orientation = glm::quat();
				obj->orientation = orientation;
			}
			else
			{
				orientation = glm::normalize(orientation);
				orientation = glm::slerp(obj->orientation, glm::quat(), obj->currentResetTime / obj->timeToReset);
				obj->currentResetTime += scene->deltaTime;
			}
		}

		modelMatrix = glm::translate(modelMatrix, obj->position);
		modelMatrix *= glm::toMat4(orientation);
		modelMatrix = glm::scale(modelMatrix, obj->scale);
		SetShadersModelMatrix(modelMatrix);
		
		// Load object material data into lighting shader
		obj->mat->diffuseMap->SetTextureUnit(GL_TEXTURE0);
		obj->mat->specularMap->SetTextureUnit(GL_TEXTURE1);
		m_lightingShader->setInt("material.diffuse", 0);
		m_lightingShader->setInt("material.specular", 1);
		m_lightingShader->setFloat("material.shininess", obj->mat->shininess);
		m_lightingShader->setVec3("material.tintColor", obj->mat->tint);

		// Load camera data into lighting shader
		m_lightingShader->setVec3("viewPos", scene->m_mainCamera->Position);

		// Draw object
		m_lightingShader->use();
		obj->mesh->use();

		if (obj == Editor::selectedObject)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}

		glDrawArrays(GL_TRIANGLES, 0, obj->mesh->numVertices);
	}
}
void Renderer::RenderPickObject(Scene* scene)
{
	if (scene == nullptr)
	{
		std::cout << "No current scene set OR scene object no longer exists.\n";
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_pickObjectFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& obj : scene->m_objects)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, obj->position);
		model *= glm::toMat4(obj->orientation);
		model = glm::scale(model, obj->scale);
		m_pickObjectShader->setMat4("model", model, false);
		m_pickObjectShader->setInt("objId", obj->id);

		m_pickObjectShader->use();
		obj->mesh->use();
		glDrawArrays(GL_TRIANGLES, 0, obj->mesh->numVertices);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Renderer::RenderDebug(Scene* scene)
{
	if (scene == nullptr)
	{
		std::cout << "No current scene set OR scene object no longer exists.\n";
		return;
	}

	if (Editor::selectedObject != nullptr)
	{
		glDisable(GL_DEPTH_TEST);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, Editor::selectedObject->position);
		model *= glm::toMat4(Editor::selectedObject->orientation);
		model = glm::scale(model, Editor::selectedObject->scale * (Editor::outlineDist + 1));
		m_outlineShader->setMat4("model", model, false);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDrawArrays(GL_TRIANGLES, 0, Editor::selectedObject->mesh->numVertices);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glEnable(GL_DEPTH_TEST);
	}
}

void Renderer::SetShadersViewMatrix(glm::mat4 view)
{
	m_lightingShader	->setMat4("view", view, false);
	m_lightSourceShader	->setMat4("view", view, false);
	m_outlineShader		->setMat4("view", view, false);
	m_pickObjectShader	->setMat4("view", view, false);
}
void Renderer::SetShadersModelMatrix(glm::mat4 model)
{
	m_lightingShader	->setMat4("model", model, false);
	m_lightSourceShader	->setMat4("model", model, false);
	m_outlineShader		->setMat4("model", model, false);
	m_pickObjectShader	->setMat4("model", model, false);
}
void Renderer::SetShadersProjectionMatrix(glm::mat4 projection)
{
	m_lightingShader	->setMat4("projection", projection, false);
	m_lightSourceShader	->setMat4("projection", projection, false);
	m_outlineShader		->setMat4("projection", projection, false);
	m_pickObjectShader	->setMat4("projection", projection, false);
}

void Renderer::FramebufferCallback(GLFWwindow* window, int w, int h)
{
	m_viewportWidth  = w;
	m_viewportHeight = h;

	SetShadersProjectionMatrix(glm::perspective(glm::radians(45.0f), static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight), 0.1f, 500.0f));
	glBindFramebuffer(GL_FRAMEBUFFER, m_pickObjectFBO);
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);

	RenderScene();
	RenderPickObject();
	RenderDebug();
}