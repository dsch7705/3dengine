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
#include "FileIO.h"
#include "Material.h"
#include "Renderer.h"

const bool fullscreen = false;
const unsigned int NUM_VAOS = 2;
const unsigned int NUM_VBOS = 1;

int windowWidth = 1280;
int windowHeight = 720;
const char* windowTitle = "3D Engine";

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
std::vector<float> crateVerticies = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

double mouseX, mouseY;
// misc
float deltaTime = 0.0f;
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
glm::vec4 skyColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	Editor::Draw(deltaTime);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	//mainScene->Update(deltaTime);
	Editor::currentScene->deltaTime = deltaTime;
	Renderer::RenderScene(Editor::currentScene);
	Renderer::RenderPickObject(Editor::currentScene);
	Renderer::RenderDebug(Editor::currentScene);
	//Editor::pickObjectPass();
	imguiDraw();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

#pragma region Callbacks

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
	{
		unsigned int col[3];
		glBindFramebuffer(GL_FRAMEBUFFER, Renderer::m_pickObjectFBO);
		glReadPixels(mouseX, windowHeight - mouseY - 1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, col);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		std::cout << "R: " << col[0] << " G: " << col[1] << " B: " << col[2] << std::endl;
		if (col[0] != 0)
		{
			Editor::selectedObject = Editor::currentScene->m_objects[col[0]-1];
			Editor::selected = Editor::selectedObject;
		}
		else
		{
			Editor::selected = nullptr;
		}
	}
}

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	mouseX = x;
	mouseY = y;
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "Scroll: " << xoffset << " | " << yoffset << "\n";
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	Renderer::FramebufferCallback(window, w, h);

	glViewport(0, 0, w, h);
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

	glfwSetMouseButtonCallback(window, Editor::MouseClickCallback);
	glfwSetCursorPosCallback(window, InputManager::MousePosCallback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glViewport(0, 0, windowWidth, windowHeight);

	// Misc
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	
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

void setupDefaults()
{
	Texture::GenerateDefaultTexture();
	Material::GenerateDefaultMaterial();

	Mesh::defaultMesh = new Mesh(crateVerticies, { 3, 3, 2 });
	Object::defaultObject = new Object(Mesh::defaultMesh, Material::defaultMaterial, glm::vec3(0.0f));

	Editor::pickObjShader = new Shader("res/shaders/pick.shader");
	
	FileIO::LoadTexturesFromDirectory("res/textures");
}

int main(int argc, char* args[])
{
	glSetup();
	imguiSetup();

	setupDefaults();

	Renderer::Setup(windowWidth, windowHeight);
	Editor::Setup();

	mainScene = new TestScene;
	mainScene->Setup();
	Renderer::SetCurrentScene(mainScene);

	mainScene->m_pointLights.push_back(PointLight());

	Editor::setCurrentScene(mainScene);

	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;


		draw();
		//Editor::pickObjectPass();
		Editor::currentScene->m_mainCamera->ProcessGamepadInput(deltaTime);
		InputManager::Poll();
	}

	imguiClean();
	glfwTerminate();
	glfwDestroyWindow(window);
	return 0;
}