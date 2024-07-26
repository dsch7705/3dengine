#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <string>
#include "FileLoader.h"

class FileLoader;
class Texture
{
public:
	enum class FileFormat
	{
		JPG = GL_RGB, PNG = GL_RGBA, OTHER = NULL
	};

public:
	Texture(const std::string& filepath);
	~Texture();

	void SetTextureUnit(GLenum unit);
	static void GenerateDefaultTexture();

	unsigned int id;
	std::string path;
	std::string filename;
	FileFormat format;

	int width, height, nrChannels;
	unsigned char* data;

	static inline Texture* defaultTexture;
	static inline std::vector<Texture*> list;

private:
	Texture();
};