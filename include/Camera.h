#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Camera
{
public:
	Camera();

	void ProcessKBMInput(float deltaTime);
	void ProcessGamepadInput(float deltaTime);
	void SetPosition(glm::vec3 position);
	void SetRotation(float pitch, float yaw);
	glm::mat4 GetViewMatrix();

public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	float Pitch;
	float Yaw;

	float MovementSpeed = 25.0f;
	float ControllerLookSpeed = 200.0f;

private:
	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	void updateCameraVectors();
};