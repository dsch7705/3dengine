#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Object.h"
#include "InputManager.h"
#include "Texture.h"
#include "Editor.h"

#include <vector>


struct DirectionalLight
{
	glm::vec3 eulerAngles = glm::vec3(19.0f, 0.0f, 30.0f);

	glm::vec3 ambient = glm::vec3(141.0f / 255.0f, 141.0f / 255.0f, 141.0f / 255.0f);
	glm::vec3 diffuse = glm::vec3(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	glm::vec3 specular = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
};
struct PointLight
{
	glm::vec3 position = glm::vec3(0.0f);
	float radius = 10.0f;

	glm::vec3 ambient = glm::vec3(141.0f / 255.0f, 141.0f / 255.0f, 141.0f / 255.0f);
	glm::vec3 diffuse = glm::vec3(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	glm::vec3 specular = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
};

class Editor;
class Scene
{
public:
	Scene();
	~Scene();

	virtual void Setup() {};
	virtual void Update(float deltaTime) {};

	// Object methods
	void RegisterObject(Object* obj);
	void UpdateCamera(float deltaTime);
	void UpdateProjectionMatrix(float fov, float aspectRatio);

public:
	float deltaTime;

	Camera* m_mainCamera;
	Shader* m_lightingShader;
	Shader* m_lightSourceShader;
	Shader* m_outlineShader;

	std::vector<Object*> m_objects;

	DirectionalLight m_dirLight;
	std::vector<PointLight> m_pointLights;

	bool wireframe = false;
	bool drawLight = false;

	glm::mat4 viewMatrix;
	static inline glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 500.0f);

private:
	void DrawObjects();
};

class TestScene : public Scene
{
public:
	TestScene() : Scene() {};

	void Setup() override
	{
		/*glm::vec3 cubePositions[] = {
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
		};*/

		RegisterObject(Object::defaultObject->Instantiate(glm::vec3(0.0f)));
	}

	void Update(float deltaTime) override
	{
		UpdateCamera(deltaTime);
	}
};