#include "Editor.h"

Scene* Editor::currentScene = nullptr;

void Editor::setCurrentScene(Scene* scene)
{
	currentScene = scene;
}

void Editor::Draw()
{
	if (currentScene == nullptr)
	{
		std::cout << "No scene selected in editor!" << std::endl;
		return;
	}

	ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
	float winW = minWidthLeft;
	if (objWinSize.x > winW)
		winW = objWinSize.x;
	if (lightWinSize.x > winW)
		winW = lightWinSize.x;
	if (miscWinSize.x > winW)
		winW = miscWinSize.x;

	// Object window
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(winW, minHeightLeft), ImVec2(viewportSize.x / 3, viewportSize.y/2));

	ImGui::Begin("Objects", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);
	
	for (const auto& obj : currentScene->m_objects)
	{
		std::string name = "Object ";
		name.append(std::to_string(obj->id));
		if (ImGui::TreeNode(name.c_str()))
		{
			if (ImGui::TreeNode("Transform"))
			{
				ImGui::DragFloat3("Position", glm::value_ptr(obj->position), 0.1f);
				ImGui::DragFloat3("Scale", glm::value_ptr(obj->scale), 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Material"))
			{
				ImGui::ColorEdit3("Ambient", glm::value_ptr(obj->materialAmbient));
				ImGui::ColorEdit3("Diffuse", glm::value_ptr(obj->materialDiffuse));
				ImGui::ColorEdit3("Specular", glm::value_ptr(obj->materialSpecular));
				ImGui::SliderFloat("Shininess", &obj->materialShininess, 1.5f, 2048.0f);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
	objWinSize = ImGui::GetWindowSize();
	ImGui::End();

	// Light window
	ImGui::SetNextWindowPos(ImVec2(0, objWinSize.y));
	ImGui::SetNextWindowSizeConstraints(ImVec2(winW, minHeightLeft), ImVec2(viewportSize.x / 3, viewportSize.y / 2));

	ImGui::Begin("Light", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::ColorEdit3("Ambient", glm::value_ptr(currentScene->m_lightAmbient));
	ImGui::ColorEdit3("Diffuse", glm::value_ptr(currentScene->m_lightDiffuse));
	ImGui::ColorEdit3("Specular", glm::value_ptr(currentScene->m_lightSpecular));
	ImGui::DragFloat3("Position", glm::value_ptr(currentScene->m_lightPosition));

	lightWinSize = ImGui::GetWindowSize();
	ImGui::End();

	// Misc.
	ImGui::SetNextWindowPos(ImVec2(0, objWinSize.y + lightWinSize.y));
	ImGui::SetNextWindowSizeConstraints(ImVec2(winW, minHeightLeft), ImVec2(viewportSize.x / 3, viewportSize.y / 2));

	ImGui::Begin("Misc.", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Checkbox("Wireframe", &currentScene->wireframe);

	miscWinSize = ImGui::GetWindowSize();
	ImGui::End();
}