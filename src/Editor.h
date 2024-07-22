#pragma once
#include <imgui/imgui.h>
#include "Scene.h"


class Editor
{
public:
	static void Draw();
	static void setCurrentScene(Scene* scene);

private:
	static Scene* currentScene;

	static inline ImVec2 objWinSize;
	static inline ImVec2 lightWinSize;
	static inline ImVec2 miscWinSize;

	static inline float minWidthLeft = 200.0f;
	static inline float minHeightLeft = 100.0f;
};