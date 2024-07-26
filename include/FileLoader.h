#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include "Texture.h"


class Texture;
class FileLoader
{
public:
	static void LoadTexturesFromDirectory(std::filesystem::path dir);
};