#pragma once
#include <glad/glad.h>
#include <imgui/imgui.h>
#include "FileIO.h"
#include "Scene.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"


class Scene;
class Editor
{
public:
	static void Setup();
	static void Draw(float deltaTime);
	static void setCurrentScene(Scene* scene);
	static void genPickFramebuffer();
	static void pickObjectPass();

	static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);

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

	static inline float outlineDist = 0.05f;

private:
	static inline float menuBarHeight;
	static inline ImVec2 inspectorSize;
	static inline ImVec2 editorSize;
	static inline float consoleHeight;

	static const inline float pollingRate = 100.0f;	// every n ms
	static inline float timeSinceLastPoll = 0.0f;
	static inline unsigned int nFrametimeSamples = 0;
	static inline float frametimeSampleSum = 0.0f;

	static inline float frameTime = 0.0f;

	static inline std::vector<Object*> objsToDelete;
	static inline std::vector<Material*> matsToDelete;
	static inline bool shouldDeleteObjects = false;
	static inline bool shouldDeleteMaterials = false;

	static inline std::vector<Material*> multiSelectedMaterials;
	static inline int multiSelectBeginID = -1;
	static inline int multiSelectEndID = -1;
};