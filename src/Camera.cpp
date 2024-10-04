#include "Camera.h"
#include "InputManager.h"

Camera::Camera()
{
	Position = glm::vec3(0.0f, 0.0f, 3.0f);
	Front = glm::vec3(0.0f, 0.0f, -1.0f);

	Yaw = -90.0f;
	Pitch = 0.0f;

	InputManager::RegisterAxis("CamMoveX", GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_KEY_D, GLFW_KEY_A);
	InputManager::SetAxisDeadzone("CamMoveX", 0.15f);
	InputManager::RegisterAxis("CamMoveZ", GLFW_GAMEPAD_AXIS_LEFT_Y, GLFW_KEY_S, GLFW_KEY_W, true);
	InputManager::SetAxisDeadzone("CamMoveZ", 0.15f);

	InputManager::RegisterAxis("CamLookX", GLFW_GAMEPAD_AXIS_RIGHT_X, GLFW_KEY_RIGHT, GLFW_KEY_LEFT);
	InputManager::SetAxisDeadzone("CamLookX", 0.1f);
	InputManager::RegisterAxis("CamLookY", GLFW_GAMEPAD_AXIS_RIGHT_Y, GLFW_KEY_DOWN, GLFW_KEY_UP, true);

	InputManager::RegisterButton("CamUp", GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
	InputManager::RegisterButton("CamDown", GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);

	InputManager::RegisterButton("CamUpKey", GLFW_KEY_E);
	InputManager::RegisterButton("CamDownKey", GLFW_KEY_Q);

	updateCameraVectors();
}

void Camera::ProcessKBMInput(float deltaTime)
{/*
	float velocity = MovementSpeed * deltaTime;
	switch (direction)
	{
	case Camera_Movement::FORWARD:
		Position += Front * velocity;
		break;
	case Camera_Movement::BACKWARD:
		Position -= Front * velocity;
		break;
	case Camera_Movement::LEFT:
		Position -= Right * velocity;
		break;
	case Camera_Movement::RIGHT:
		Position += Right * velocity;
		break;

	case Camera_Movement::LOOK_UP:
		Pitch += 0.1f;
		break;
	case Camera_Movement::LOOK_DOWN:
		Pitch -= 0.1f;
		break;
	case Camera_Movement::LOOK_LEFT:
		Yaw -= 0.1f;
		break;
	case Camera_Movement::LOOK_RIGHT:
		Yaw += 0.1f;
		break;
	}

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	updateCameraVectors();*/
}

void Camera::ProcessGamepadInput(float deltaTime)
{
	// Move
	float velocity = MovementSpeed * deltaTime;

	Position += Front * InputManager::GetAxis("CamMoveZ") * velocity;
	Position += Right * InputManager::GetAxis("CamMoveX") * velocity;
	
	if (InputManager::GetButton("CamUp") == ButtonState::HELD || InputManager::GetButton("CamUpKey") == ButtonState::HELD)
		Position += worldUp * velocity;
	if (InputManager::GetButton("CamDown") == ButtonState::HELD || InputManager::GetButton("CamDownKey") == ButtonState::HELD)
		Position -= worldUp * velocity;

	// Look
	Pitch += InputManager::GetAxis("CamLookY") * ControllerLookSpeed * deltaTime;
	Yaw += InputManager::GetAxis("CamLookX") * ControllerLookSpeed * deltaTime;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	updateCameraVectors();
}
void Camera::SetPosition(glm::vec3 position)
{
	Position = position;

	updateCameraVectors();
}
void Camera::SetRotation(float pitch, float yaw)
{
	Pitch = pitch;
	Yaw = yaw;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, worldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}