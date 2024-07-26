#pragma once
#include <glad/glad.h>
#include <imgui/imgui.h>
#include "FileLoader.h"
#include "Scene.h"


class Scene;
class Editor
{
public:
	static void Draw(float deltaTime);
	static void setCurrentScene(Scene* scene);
	static void genPickFramebuffer();
	static void pickObjectPass();

	static Scene* currentScene;
	static inline Object* selectedObject = nullptr;
	static inline Material* selectedMaterial = nullptr;
	static inline void* selected = nullptr;

	static inline Shader* pickObjShader;

	static inline int windowWidth = 1280;
	static inline int windowHeight = 720;

	static inline unsigned int pickObjFramebuffer;
	static inline unsigned int pickObjTx;
	static inline unsigned int pickObjDTx;

private:
	static inline ImVec2 inspectorSize;
	static inline ImVec2 editorSize;
	static inline float consoleHeight;

	static const inline float pollingRate = 100.0f;	// every n ms
	static inline float timeSinceLastPoll = 0.0f;
	static inline unsigned int nFrametimeSamples = 0;
	static inline float frametimeSampleSum = 0.0f;

	static inline float frameTime = 0.0f;
};