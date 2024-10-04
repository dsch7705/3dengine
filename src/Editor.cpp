#include "Editor.h"

Scene* Editor::currentScene = nullptr;

void Editor::Setup()
{
	InputManager::RegisterButton("shift", GLFW_KEY_LEFT_SHIFT);
	InputManager::RegisterButton("control", GLFW_KEY_LEFT_CONTROL);
}

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

	// Menu bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save All", "CTRL+S"))
				FileIO::SerializeScene(currentScene, "res/scenes/default.scene");
			if (ImGui::MenuItem("Load Scene", "CTRL+O"))
				currentScene = FileIO::LoadScene("res/scenes/default.scene");

			ImGui::EndMenu();
		}

		menuBarHeight = ImGui::GetWindowSize().y;
		ImGui::EndMainMenuBar();
	}

	// Inspector window
	ImGui::SetNextWindowPos(ImVec2(0.0f, menuBarHeight));
	ImGui::SetNextWindowSizeConstraints(ImVec2(viewportSize.x / 6, viewportSize.y - consoleHeight - menuBarHeight), ImVec2(viewportSize.x / 4, viewportSize.y - consoleHeight - menuBarHeight));
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

		for (const auto& [key, mat] : Material::map)
		{
			ImGui::PushID(mat);

			bool isMultiSelected = key.second >= multiSelectBeginID && key.second <= multiSelectEndID;
			if (isMultiSelected)
				multiSelectedMaterials.push_back(mat);

			if (ImGui::Selectable(key.first.c_str(), isMultiSelected))
			{
				if (InputManager::GetButton("shift") == ButtonState::HELD && multiSelectBeginID != -1)
					multiSelectEndID = key.second;
				else
				{
					multiSelectBeginID = key.second;
					multiSelectEndID = key.second;
				}

				if (multiSelectBeginID != -1 && multiSelectEndID != -1 && multiSelectEndID < multiSelectBeginID)
					std::swap(multiSelectBeginID, multiSelectEndID);

				selectedMaterial = mat;
				selected = mat;
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup("mat_popup");
			}
			if (ImGui::BeginPopup("mat_popup"))
			{
				if (ImGui::Button("Delete"))
					shouldDeleteMaterials = true;

				ImGui::EndPopup();
			}
			ImGui::PopID();
		}

		ImGui::TreePop();
		ImGui::Unindent(7.5f);
	}

	inspectorSize.x = ImGui::GetWindowWidth();
	ImGui::End();

	// Editor window
	ImGui::SetNextWindowPos(ImVec2(viewportSize.x - editorSize.x, menuBarHeight));
	ImGui::SetNextWindowSizeConstraints(ImVec2(viewportSize.x / 6, viewportSize.y - consoleHeight - menuBarHeight), ImVec2(viewportSize.x / 4, viewportSize.y - consoleHeight - menuBarHeight));
	ImGui::Begin("Editor");

	if (ImGui::BeginTabBar("##editor_tabs"))
	{
		if (ImGui::BeginTabItem("Object/Material"))
		{
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
						float sensitivity = 0.01f;
						ImGui::SetNextItemOpen(true, ImGuiCond_Once);
						if (ImGui::TreeNode("Position"))
						{
							ImGui::DragFloat3("", glm::value_ptr(selectedObject->position), sensitivity);

							ImGui::TreePop();
						}
						ImGui::SetNextItemOpen(true, ImGuiCond_Once);
						if (ImGui::TreeNode("Scale"))
						{
							ImGui::DragFloat3("", glm::value_ptr(selectedObject->scale), sensitivity);

							ImGui::TreePop();
						}
						ImGui::SetNextItemOpen(true, ImGuiCond_Once);

						if (ImGui::TreeNode("Rotation"))
						{
							ImGui::DragFloat3("", glm::value_ptr(selectedObject->delta_rotation), sensitivity * 25.0f);
							//selectedObject->delta_rotation = selectedObject->rotation - lastRot;

							ImGui::TreePop();
						}
						if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
						{
							ImGui::OpenPopup("reset_orientation_popup");
						}
						if (ImGui::BeginPopup("reset_orientation_popup"))
						{
							if (ImGui::Button("Reset orientation"))
								selectedObject->ResetOrientation(0.5f);

							ImGui::EndPopup();
						}

						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Material"))
					{
						if (ImGui::BeginCombo("##mat", selectedObject->mat->GetKey().first.c_str()))
						{
							for (const auto& [key, mat] : Material::map)
							{
								if (ImGui::Selectable(key.first.c_str(), selectedObject->mat == mat))
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
					std::string nameLabel = "'" + selectedMaterial->GetKey().first + "'";
					nameLabel.append(" properties");
					ImGui::SeparatorText(nameLabel.c_str());

					if (ImGui::BeginCombo("Diffuse Map", selectedMaterial->diffuseMap->filename.c_str()))
					{
						for (const auto& [filename, tx] : Texture::map)
						{
							if (ImGui::Selectable(filename.c_str(), tx == selectedMaterial->diffuseMap))
								selectedMaterial->diffuseMap = tx;
						}

						ImGui::EndCombo();
					}
					if (ImGui::BeginCombo("Specular Map", selectedMaterial->specularMap->filename.c_str()))
					{
						for (const auto& [filename, tx] : Texture::map)
						{
							if (ImGui::Selectable(filename.c_str(), tx == selectedMaterial->specularMap))
								selectedMaterial->specularMap = tx;
						}

						ImGui::EndCombo();
					}
					ImGui::ColorEdit3("Tint", glm::value_ptr(selectedMaterial->tint));
					ImGui::SliderFloat("Shininess", &selectedMaterial->shininess, 1.0f, 512.0f);
				}
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Lighting"))
		{
			ImGui::SeparatorText("Light properties");
			if (ImGui::CollapsingHeader("Sun"))
			{
				ImGui::Indent(7.5f);

				ImGui::DragFloat3("Rotation", glm::value_ptr(currentScene->m_dirLight.eulerAngles));
				ImGui::ColorEdit3("Ambient", glm::value_ptr(currentScene->m_dirLight.ambient));
				ImGui::ColorEdit3("Diffuse", glm::value_ptr(currentScene->m_dirLight.diffuse));
				ImGui::ColorEdit3("Specular", glm::value_ptr(currentScene->m_dirLight.specular));

				ImGui::Unindent(7.5f);
			}
			if (ImGui::CollapsingHeader("Point Lights"))
			{
				ImGui::Indent(7.5f);

				for (int i = 0; i < currentScene->m_pointLights.size(); ++i)
				{
					std::string label = "Light " + std::to_string(i);
					if (ImGui::TreeNode(label.c_str()))
					{
						ImGui::DragFloat3("Position", glm::value_ptr(currentScene->m_pointLights[i].position));
						ImGui::DragFloat("Radius", &currentScene->m_pointLights[i].radius);
						ImGui::ColorEdit3("Ambient", glm::value_ptr(currentScene->m_pointLights[i].ambient));
						ImGui::ColorEdit3("Diffuse", glm::value_ptr(currentScene->m_pointLights[i].diffuse));
						ImGui::ColorEdit3("Specular", glm::value_ptr(currentScene->m_pointLights[i].specular));
						ImGui::TreePop();
					}
				}

				ImGui::Unindent(7.5f);
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
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

	ImGui::SliderFloat("Outline Distance", &outlineDist, 0.0f, 5.0f);
	ImGui::Separator();
	ImGui::DragFloat3("Directional Light Rotation", glm::value_ptr(currentScene->m_dirLight.eulerAngles));

	consoleHeight = ImGui::GetWindowHeight();
	ImGui::End();

	if (shouldDeleteMaterials)
	{
		if (selected == selectedMaterial)
			selected = nullptr;
		selectedMaterial = nullptr;

		multiSelectBeginID = -1;
		multiSelectEndID = -1;

		Material::BatchDelete(multiSelectedMaterials);
		shouldDeleteMaterials = false;
	}
		
	multiSelectedMaterials.clear();
	//Material::BatchDelete(matsToDelete);
	//matsToDelete.clear();
}

void Editor::genPickFramebuffer()
{
	glGenFramebuffers(1, &pickObjFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, pickObjFramebuffer);

	glGenTextures(1, &pickObjTx);
	glBindTexture(GL_TEXTURE_2D, pickObjTx);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, windowWidth, windowHeight, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, NULL);
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

		pickObjShader->use();
		obj->mesh->use();
		glDrawArrays(GL_TRIANGLES, 0, obj->mesh->numVertices);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void Editor::MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
	{
		unsigned int data[3];
		glBindFramebuffer(GL_FRAMEBUFFER, pickObjFramebuffer);
		glReadPixels(InputManager::mouseX, windowHeight - InputManager::mouseY - 1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, data);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (data[0] != 0)
		{
			selectedObject = currentScene->m_objects[data[0] - 1];
			selected = selectedObject;
		}
		else
		{
			selected = nullptr;
			selectedObject = nullptr;
		}
	}
}