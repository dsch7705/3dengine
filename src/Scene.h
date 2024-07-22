#pragma once
#include "Camera.h"
#include "Shader.h"
#include "Object.h"
#include "InputManager.h"

#include <vector>


class Scene
{
public:
	Scene();
	~Scene();

	void Draw();

	virtual void Setup() {};
	virtual void Update(float deltaTime) {};

	// Object methods
	void RegisterObject(Object* obj);
	void UpdateCamera(float deltaTime);

public:
	Camera* m_mainCamera;
	Shader* m_lightingShader;
	Shader* m_lightSourceShader;

	std::vector<Object*> m_objects;

	glm::vec3 m_lightPosition = glm::vec3(10.0f, 10.0f, 5.0f);
	glm::vec3 m_lightAmbient = glm::vec3(141.0f / 255.0f, 141.0f / 255.0f, 141.0f / 255.0f);
	glm::vec3 m_lightDiffuse = glm::vec3(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
	glm::vec3 m_lightSpecular = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);

	bool wireframe = false;

private:
	void DrawObjects();

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 500.0f);
};

class TestScene : public Scene
{
public:
	TestScene() : Scene() {};

	void Setup() override
	{
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
		VertexData boxData(boxVertices, { 3, 3 });

		Object* box = new Object();
		box->LoadVertexData(boxData);
		RegisterObject(box);

		for (int i = 1; i <= 2; i++)
		{
			RegisterObject(Object::Instantiate(box, glm::vec3(i * 2.0f, 0.0f, 0.0f)));
		}

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				
			}
		}
	}

	void Update(float deltaTime) override
	{
		UpdateCamera(deltaTime);

		Draw();
	}
};