#include "Editor.h"

Scene* Editor::currentScene = nullptr;

void Editor::setCurrentScene(Scene* scene)
{
	currentScene = scene;
}

void Editor::Draw(float deltaTime)
{
	if (currentScene == nullptr)
	{
		std::cout << "No scene selected in editor!" << std::endl;
		return;
	}

	ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
	// Inspector window
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSizeConstraints(ImVec2(viewportSize.x / 6, viewportSize.y - consoleHeight), ImVec2(viewportSize.x / 4, viewportSize.y - consoleHeight));
	ImGui::Begin("Inspector", (bool*)0, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::Button("Object")) { currentScene->RegisterObject(Object::defaultObject->Instantiate(glm::vec3(0.0f))); }
			if (ImGui::Button("Material")) { new Material(Material::defaultMaterial->diffuseMap, Material::defaultMaterial->specularMap, Material::defaultMaterial->shininess); }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Separator();
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Objects"))
	{
		ImGui::Indent(7.5f);

		for (const auto& obj : currentScene->m_objects)
		{
			if (ImGui::Selectable(obj->name.c_str(), obj == selected))
			{
				selectedObject = obj;
				selected = obj;
			}
		}

		ImGui::TreePop();
		ImGui::Unindent(7.5f);
	}
	ImGui::Separator();
	if (ImGui::TreeNode("Materials"))
	{
		ImGui::Indent(7.5f);

		for (const auto& mat : Material::list)
		{
			if (ImGui::Selectable(mat->name.c_str(), mat == selected))
			{
				selectedMaterial = mat;
				selected = mat;
			}
		}

		ImGui::TreePop();
		ImGui::Unindent(7.5f);
	}

	inspectorSize.x = ImGui::GetWindowWidth();
	ImGui::End();

	// Editor window
	ImGui::SetNextWindowPos(ImVec2(viewportSize.x - editorSize.x, 0.0f));
	ImGui::SetNextWindowSizeConstraints(ImVec2(viewportSize.x / 6, viewportSize.y - consoleHeight), ImVec2(viewportSize.x / 4, viewportSize.y - consoleHeight));
	ImGui::Begin("Editor");

	if (selected != nullptr)
	{
		// Object selected
		if (selected == selectedObject)
		{
			std::string nameLabel = "'" + selectedObject->name + "'";
			nameLabel.append(" properties");
			ImGui::SeparatorText(nameLabel.c_str());

			ImGui::SetNextItemOpen(true, ImGuiCond_Once);
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNode("Position"))
				{
					ImGui::DragFloat3("", glm::value_ptr(selectedObject->position), 0.05f);

					ImGui::TreePop();
				}
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNode("Scale"))
				{
					ImGui::DragFloat3("", glm::value_ptr(selectedObject->scale), 0.05f);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Material"))
			{
				if (ImGui::BeginCombo("##mat", selectedObject->mat->name.c_str()))
				{
					for (const auto& mat : Material::list)
					{
						if (ImGui::Selectable(mat->name.c_str(), selectedObject->mat == mat))
							selectedObject->mat = mat;
					}
					ImGui::EndCombo();
				}
				ImGui::TreePop();
			}
		}

		// Material selected
		else if (selected == selectedMaterial)
		{
			std::string nameLabel = "'" + selectedMaterial->name + "'";
			nameLabel.append(" properties");
			ImGui::SeparatorText(nameLabel.c_str());

			if (ImGui::BeginCombo("Diffuse Map", selectedMaterial->diffuseMap->filename.c_str()))
			{
				for (const auto& tx : Texture::list)
				{
					if (ImGui::Selectable(tx->filename.c_str(), tx == selectedMaterial->diffuseMap))
						selectedMaterial->diffuseMap = tx;
				}

				ImGui::EndCombo();
			}
			if (ImGui::BeginCombo("Specular Map", selectedMaterial->specularMap->filename.c_str()))
			{
				for (const auto& tx : Texture::list)
				{
					if (ImGui::Selectable(tx->filename.c_str(), tx == selectedMaterial->specularMap))
						selectedMaterial->specularMap = tx;
				}

				ImGui::EndCombo();
			}
			ImGui::ColorEdit3("Tint", glm::value_ptr(selectedMaterial->tint));
			ImGui::SliderFloat("Shininess", &selectedMaterial->shininess, 1.0f, 512.0f);
		}
	}

	editorSize.x = ImGui::GetWindowWidth();
	ImGui::End();

	// Console window
	ImGui::SetNextWindowPos(ImVec2(0.0f, viewportSize.y - consoleHeight));
	ImGui::SetNextWindowSizeConstraints(ImVec2(viewportSize.x, viewportSize.y / 6), ImVec2(viewportSize.x, viewportSize.y / 4));
	ImGui::Begin("Console");

	// Update frame time
	nFrametimeSamples++;
	float ft = deltaTime * 1000.0f;
	frametimeSampleSum += ft;
	timeSinceLastPoll += ft;
	if (timeSinceLastPoll > pollingRate)
	{
		frameTime = frametimeSampleSum / (float)nFrametimeSamples;
		nFrametimeSamples = 0;
		frametimeSampleSum = 0.0f;
		timeSinceLastPoll = 0.0f;
	}

	std::string fpsText = "Frame time: " + std::to_string(frameTime) + " ms (" + std::to_string(1.0f / (frameTime / 1000.0f)) + " FPS)";
	ImGui::Text(fpsText.c_str());

	consoleHeight = ImGui::GetWindowHeight();
	ImGui::End();
}

void Editor::genPickFramebuffer()
{
	glGenFramebuffers(1, &pickObjFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, pickObjFramebuffer);

	glGenTextures(1, &pickObjTx);
	glBindTexture(GL_TEXTURE_2D, pickObjTx);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickObjTx, 0);

	glGenTextures(1, &pickObjDTx);
	glBindTexture(GL_TEXTURE_2D, pickObjDTx);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pickObjDTx, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error: " << status << std::endl;
		exit(-1);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	pickObjShader->setMat4("projection", Scene::projectionMatrix, false);
}
void Editor::pickObjectPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pickObjFramebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < currentScene->m_objects.size(); i++)
	{
		Object* obj = currentScene->m_objects[i];
		pickObjShader->use();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, obj->position);
		model = glm::scale(model, obj->scale);
		pickObjShader->setMat4("model", model, false);
		pickObjShader->setMat4("view", currentScene->viewMatrix, false);
		pickObjShader->setInt("objId", i);

		currentScene->m_lightingShader->use();
		obj->mesh->use();
		glDrawArrays(GL_TRIANGLES, 0, obj->mesh->numVertices);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}