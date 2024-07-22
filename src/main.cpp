#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Object.h"
#include "InputManager.h"
#include "Editor.h"

const bool fullscreen = false;
const unsigned int NUM_VAOS = 2;
const unsigned int NUM_VBOS = 1;

int windowWidth = 1280;
int windowHeight = 720;
const char* windowTitle = "Minecraft Clone";

GLFWwindow* window;
std::vector<float> boxVertices = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

Scene* mainScene;

// misc
float deltaTime = 0.0f;
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
glm::vec4 skyColor(0.0f, 0.85f, 1.0f, 1.0f);

glm::vec3 lightAmbient(141.0f / 255.0f, 141.0f / 255.0f, 141.0f / 255.0f);
glm::vec3 lightDiffuse(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
glm::vec3 lightSpecular(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
float lightY = 10.0f;
float lightOrbitRadius = 10.0f;
float lightOrbitVelocity = 3.141592f * 2.0f;	// Radians/second

bool wireframe = false;
void imguiDraw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	// draw
	ImGui::NewFrame();

	Editor::Draw();
	/*
	ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
	float imWindowWidth = 400.0f;
	float imWindowHeight = 200.0f;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(imWindowWidth, imWindowHeight), ImGuiCond_Appearing);
	ImGui::Begin("Test");
	ImGui::ColorEdit4("Sky Color ", glm::value_ptr(skyColor));
	ImGui::NewLine();
	//ImGui::SliderFloat3("Box Scale ", glm::value_ptr(object.scale), 0.0f, 100.0f);
	ImGui::NewLine();
	ImGui::Checkbox("Wireframe ", &wireframe);
	glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(viewportSize.x - imWindowWidth, 0.0f), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(imWindowWidth, imWindowHeight), ImGuiCond_Appearing);
	ImGui::Begin("Material Properties");
	
	//ImGui::ColorEdit3("Material Ambient ", glm::value_ptr(materialAmbient));
	//ImGui::ColorEdit3("Material Diffuse ", glm::value_ptr(materialDiffuse));
	//ImGui::ColorEdit3("Material Specular ", glm::value_ptr(materialSpecular));
	//ImGui::SliderFloat("Material Shininess ", &materialShininess, 1.0f, 1024.0f);
	if (ImGui::CollapsingHeader("Objects"))
	{
		ImGui::Indent(5.0f);
		int i = 0;
		for (auto& obj : objManager.objects)
		{
			ImGui::PushID(obj);
			std::string objName = "Object ";
			objName.append(std::to_string(obj->id));
			if (ImGui::TreeNode(objName.c_str()))
			{
				std::string mName = "Properties ";
				mName.append(std::to_string(obj->id));
				if (ImGui::TreeNode(mName.c_str()))
				{
					ImGui::ColorEdit3("Material Ambient ", glm::value_ptr(obj->materialAmbient));
					ImGui::ColorEdit3("Material Diffuse ", glm::value_ptr(obj->materialDiffuse));
					ImGui::ColorEdit3("Material Specular ", glm::value_ptr(obj->materialSpecular));
					ImGui::SliderFloat("Material Shininess ", &obj->materialShininess, 1.0f, 1024.0f);

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			ImGui::PopID();
			i++;
		}*
		ImGui::Unindent();
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(viewportSize.x - imWindowWidth, imWindowHeight), ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(ImVec2(imWindowWidth, imWindowHeight), ImGuiCond_Appearing);
	ImGui::Begin("Light Properties");

	ImGui::ColorEdit3("Light Ambient ", glm::value_ptr(lightAmbient));
	ImGui::ColorEdit3("Light Diffuse ", glm::value_ptr(lightDiffuse));
	ImGui::ColorEdit3("Light Specular ", glm::value_ptr(lightSpecular));
	ImGui::SliderFloat("Light Y ", &lightY, 2.0f, 50.0f);
	ImGui::SliderFloat("Light Orbit Radius ", &lightOrbitRadius, 0.0f, 100.0f);
	ImGui::SliderFloat("Light Orbit Velocity ", &lightOrbitVelocity, 0.0f, 50.0f);
	ImGui::End();
	*/

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void imguiClean()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void draw()
{
	// Clear screen
	glClearColor(skyColor.r, skyColor.g, skyColor.b, skyColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	// Light
	glm::vec3 lightPos(cos(glfwGetTime() * lightOrbitVelocity) * lightOrbitRadius, lightY, sin(glfwGetTime() * lightOrbitVelocity) * lightOrbitRadius);

	shaderLightCube.use();
	object.UseVertexArray();
	//glBindVertexArray(VAOs[1]);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	glm::mat4 view = cam.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 500.0f);

	shaderLightCube.setMat4("model", model, false);
	shaderLightCube.setMat4("view", view, false);
	shaderLightCube.setMat4("projection", projection, false);

	shaderLightCube.setVec3("lightColor", lightColor);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Box
	//shaderLighting.setMat4("view", view, false);
	//shaderLighting.setMat4("projection", projection, false);
	//shaderLighting.setVec3("objectColor", objectColor);
	//shaderLighting.setVec3("lightColor", lightColor);
	shaderLighting.setVec3("lightPos", lightPos);
	shaderLighting.setVec3("viewPos", cam.Position);

	//shaderLighting.setVec3("material.ambient", materialAmbient);
	//shaderLighting.setVec3("material.diffuse", materialDiffuse);
	//shaderLighting.setVec3("material.specular", materialSpecular);
	//shaderLighting.setFloat("material.shininess", materialShininess);

	shaderLighting.setVec3("light.ambient", lightAmbient);
	shaderLighting.setVec3("light.diffuse", lightDiffuse);
	shaderLighting.setVec3("light.specular", lightSpecular);

	objManager.lightingShader = &shaderLighting;
	objManager.Draw(view, projection);
	/*
	model = glm::mat4(1.0f);
	model = glm::translate(model, object.position);
	shaderLighting.setMat4("model", model, false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	Object obj2 = Object::Instantiate(object, glm::vec3(2.0f, 0.0f, 0.0f));
	model = glm::mat4(1.0f);
	model = glm::translate(model, obj2.position);
	shaderLighting.setMat4("model", model, false);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	/*
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(150.0f + i*5), glm::vec3(0.5f, 1.0f, 0.0f));

		shaderLighting.setMat4("model", model, false);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	*/
	
	mainScene->Update(deltaTime);
	imguiDraw();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

#pragma region Callbacks

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "Scroll: " << xoffset << " | " << yoffset << "\n";
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	draw();
}

#pragma endregion

int glSetup()
{
	// Window
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW.\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	if (fullscreen)
	{
		windowWidth = mode->width;
		windowHeight = mode->height;
	}

	window = glfwCreateWindow(
		windowWidth,
		windowHeight,
		windowTitle,
		fullscreen ? glfwGetPrimaryMonitor() : nullptr,	// Monitor, use glfwPrimaryMonitor() for full screen
		nullptr											// Share, use this for resource sharing
	);
	if (window == nullptr)
	{
		std::cout << "Failed to create GFLW window.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialized GLAD.\n";
		return -1;
	}

	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, windowWidth, windowHeight);

	// OpenGL

	// Shader loading
	//shader1 = Shader("res/shaders/basic.shader");
	//shader2 = Shader("res/shaders/basic2.shader");
	//shaderCube = Shader("res/shaders/Cube.shader");
	//shaderLighting = Shader("res/shaders/Lighting.shader");
	//shaderLightCube = Shader("res/shaders/LightCube.shader");

	// Texture loading
	//texture1 = Texture("res/textures/container.jpg", GL_LINEAR);
	//texture2 = Texture("res/textures/cobble.png", GL_NEAREST);
	
	//texture1.SetTextureUnit(GL_TEXTURE0);
	//texture2.SetTextureUnit(GL_TEXTURE1);

	//shader2.setInt("texture1", 0);
	//shader2.setInt("texture2", 1);

	//shaderCube.setInt("texture1", 0);
	//shaderCube.setInt("texture2", 1);

	// Vertex array objects
	//glGenVertexArrays(NUM_VAOS, VAOs);

	// Vertex buffer objects
	//glGenBuffers(NUM_VBOS, VBOs);

	// Load vertex data and configure vertex attributes

	// Box
	//glBindVertexArray(VAOs[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * boxVertices.size(), &boxVertices.front(), GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// Light
	//glBindVertexArray(VAOs[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // using the same vertex data as cube

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	// Objects
	//VertexData boxData(boxVertices, { 3, 3 });
	//object.LoadVertexData(boxData);
	//object.scale = glm::vec3(20.0f, 0.1f, 20.0f);

	// Misc
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	InputManager::Init(window);
}

void imguiSetup()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

int main(int argc, char* args[])
{
	//VertexData(boxVertices, { 3, 3 });
	//std::cout << boxVertices << std::endl;
	glSetup();
	imguiSetup();
	mainScene = new TestScene;
	mainScene->Setup();

	Editor::setCurrentScene(mainScene);

	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		draw();
		InputManager::Poll();
	}

	imguiClean();
	glfwTerminate();
	return 0;
}