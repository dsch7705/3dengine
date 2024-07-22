#include "InputManager.h"

GLFWwindow* InputManager::m_window;
int InputManager::m_joystickId = -1;

void InputManager::Init(GLFWwindow* window)
{
	m_window = window;
	for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_16; i++)
	{
		if (glfwJoystickPresent(i))
		{
			m_joystickId = i;
			std::cout << "Using joystick [" << i << "] - " << glfwGetGamepadName(i) << std::endl;
			break;
		}

		std::cout << "No joystick present" << std::endl;
	}
}

std::map<std::string, AxisData> InputManager::m_axisMap;
std::map<std::string, ButtonData> InputManager::m_buttonMap;

void InputManager::RegisterAxis(const std::string& axisName, int axisOffset, bool inv)
{
	m_axisMap[axisName] = AxisData(axisOffset, inv);
}
void InputManager::RegisterAxis(const std::string& axisName, int pId, int nId, bool inv)
{
	m_axisMap[axisName] = AxisData(pId, nId, inv);
}
void InputManager::RegisterAxis(const std::string& axisName, int axisId, int pId, int nId, bool inv)
{
	m_axisMap[axisName] = AxisData(axisId, pId, nId, inv);
}

float InputManager::GetAxis(const std::string& axisName)
{
	AxisData& axis = m_axisMap[axisName];
	if (abs(axis.value) < axis.deadzone)
		return 0.0f;

	return axis.invert ? -axis.value : axis.value;
}

void InputManager::SetAxisDeadzone(const std::string& axisName, float deadzone)
{
	AxisData& axis = m_axisMap[axisName];
	axis.deadzone = deadzone;
}

void InputManager::RegisterButton(const std::string& buttonName, int buttonId)
{
	m_buttonMap[buttonName] = ButtonData(buttonId);
}

ButtonState InputManager::GetButton(const std::string& buttonName)
{
	return m_buttonMap[buttonName].state;
}

void InputManager::Poll()
{
	// Axes
	for (auto& [axisKey, axisData] : m_axisMap)
	{
		if (axisData.axisOffset == -1 && axisData.positiveButtonId == -1 && axisData.negativeButtonId == -1)
			continue;

		if (axisData.axisOffset == -1)
		{
			axisData.value = (float)glfwGetKey(m_window, axisData.positiveButtonId) - (float)glfwGetKey(m_window, axisData.negativeButtonId);
			continue;
		}
		
		if (axisData.axisOffset != -1 && m_joystickId != -1)
		{
			int n;
			const float* axes = glfwGetJoystickAxes(m_joystickId, &n);
			axisData.value = axes[axisData.axisOffset];
		}

		if (axisData.axisOffset != -1 && axisData.positiveButtonId != -1 && axisData.negativeButtonId != -1)
		{
			if (m_joystickId == -1)
				axisData.value = (float)glfwGetKey(m_window, axisData.positiveButtonId) - (float)glfwGetKey(m_window, axisData.negativeButtonId);

			else
			{
				int n;
				const float* axes = glfwGetJoystickAxes(m_joystickId, &n);
				axisData.value = axes[axisData.axisOffset];
			}
		}
	}

	// Buttons
	for (auto& [buttonKey, buttonData] : m_buttonMap)
	{
		int keyState;
		if (m_joystickId != -1)
		{
			int n;
			keyState = glfwGetJoystickButtons(m_joystickId, &n)[buttonData.buttonId];
		}
		else
			keyState = glfwGetKey(m_window, buttonData.buttonId);

		switch (keyState)
		{
		case GLFW_PRESS:
			switch (buttonData.state) // Button state
			{
			case ButtonState::OFF:
				buttonData.state = ButtonState::PRESSED;
				break;
			case ButtonState::PRESSED:
				buttonData.state = ButtonState::HELD;
				break;
			default:
				break;
			}
			break;
		case GLFW_RELEASE:
			switch (buttonData.state)
			{
			case ButtonState::PRESSED || ButtonState::HELD:
				buttonData.state = ButtonState::RELEASED;
				break;
			default:
				buttonData.state = ButtonState::OFF;
				break;
			}
		}
	}
}