#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include "Scene.h"


class Scene;
class FileIO
{
public:
	static inline const std::string textureDirectory = "res/textures";
	static inline const std::string materialDirectory = "res/materials";

	static void LoadTexturesFromDirectory(std::filesystem::path dir = textureDirectory);
	static void LoadMaterialsFromDirectory(std::filesystem::path dir = materialDirectory);

	static void SerializeScene(Scene* scene, const std::string& outputDir);
	static Scene* LoadScene(const std::string& path);

	static void SerializeMaterial(Material* mat, const std::string& outputDir);
	static Material* LoadMaterial(const std::string& path);

private:
	static glm::vec3 ReadVec3(const std::string& line, const std::string& varName);
	static float ReadFloat(const std::string& line, const std::string& varName);
	static std::string ReadString(const std::string& line, const std::string& varName);

	static void WriteVec3(std::string& line, const std::string& varName, glm::vec3 value);
	static void WriteFloat(std::string& line, const std::string& varName, float value);
	static void WriteString(std::string& line, const std::string& varName, const std::string& value);
};