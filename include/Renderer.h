#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Editor.h"
#include "Scene.h"


class Renderer
{
public:

	static void Setup(unsigned int viewW, unsigned int viewH);
	static void SetCurrentScene(Scene* scene);

	static void RenderScene(Scene* scene = m_currentScene);
	static void RenderPickObject(Scene* scene = m_currentScene);
	static void RenderDebug(Scene* scene = m_currentScene);

	static void SetShadersViewMatrix(glm::mat4 view);
	static void SetShadersModelMatrix(glm::mat4 model);
	static void SetShadersProjectionMatrix(glm::mat4 projection);

	static void FramebufferCallback(GLFWwindow* window, int w, int h);

	static inline unsigned int m_pickObjectFBO;
	static inline unsigned int m_pickObjectColorTex;
	static inline unsigned int m_pickObjectDepthTex;

	static inline unsigned int m_viewportWidth;
	static inline unsigned int m_viewportHeight;

	static inline Shader* m_lightingShader;
	static inline Shader* m_lightSourceShader;
	static inline Shader* m_outlineShader;
	static inline Shader* m_pickObjectShader;

	static inline Scene* m_currentScene;
};