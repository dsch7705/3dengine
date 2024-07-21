#pragma once
#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#define INPUT_NONE -1

enum ButtonState {
	PRESSED,
	HELD,
	RELEASED,
	OFF
};

struct ButtonData;

struct AxisData
{
	float value;
	int axisOffset;

	int positiveButtonId;
	int negativeButtonId;

	bool invert;
	float deadzone = 0.05f;

	AxisData()
	{
		value = 0.0f;
		axisOffset = INPUT_NONE;

		positiveButtonId = INPUT_NONE;
		negativeButtonId = INPUT_NONE;

		invert = false;
	}
	AxisData(int id, bool inv = false)
	{
		value = 0.0f;
		axisOffset = id;

		positiveButtonId = INPUT_NONE;
		negativeButtonId = INPUT_NONE;

		invert = inv;
	}
	AxisData(int pId, int nId, bool inv = false)
	{
		value = 0.0f;
		axisOffset = -1;

		positiveButtonId = pId;
		negativeButtonId = nId;

		invert = inv;
	}
	AxisData(int axOffset, int pId, int nId, bool inv = false)
	{
		value = 0.0f;
		axisOffset = axOffset;

		positiveButtonId = pId;
		negativeButtonId = nId;

		invert = inv;
	}
};
struct ButtonData
{
	ButtonState state;
	int buttonId;

	ButtonData()
	{
		state = ButtonState::OFF;
		buttonId = INPUT_NONE;
	}
	ButtonData(int id)
	{
		state = ButtonState::OFF;
		buttonId = id;
	}
};

class InputManager
{
public:
	static void Init(GLFWwindow* window);

	static void RegisterAxis(const std::string& axisName, int axisId, bool inv = false);
	static void RegisterAxis(const std::string& axisName, int pId, int nId);
	static float GetAxis(const std::string& axisName);
	static void SetAxisDeadzone(const std::string& axisName, float deadzone);

	static void RegisterButton(const std::string& buttonName, int buttonId);
	static ButtonState GetButton(const std::string& buttonName);

	// UseJoystick(int id);

	static void Poll();

private:
	static GLFWwindow* m_window;

	static std::map<std::string, AxisData> m_axisMap;
	static std::map<std::string, ButtonData> m_buttonMap;

	static int m_joystickId;
};