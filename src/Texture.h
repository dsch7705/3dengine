#pragma once
#include <glad/glad.h>
#include <stb_image.h>
#include <string>

class Texture
{
public:
	enum class FileFormat
	{
		JPG = GL_RGB, PNG = GL_RGBA
	};

public:
	Texture();
	Texture(const std::string& filepath, unsigned int filterMode);

	void SetTextureUnit(GLenum unit);

	unsigned int id;
	std::string path;
	FileFormat format;

	int width, height, nrChannels;
	unsigned char* data;
};