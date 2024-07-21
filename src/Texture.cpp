#include "Texture.h"

Texture::Texture()
{}
Texture::Texture(const std::string& filepath, unsigned int filterMode)
{
	path = filepath;
	if (path.find(".jpg") != std::string::npos)
		format = FileFormat::JPG;
	else if (path.find(".png") != std::string::npos)
		format = FileFormat::PNG;

	data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void Texture::SetTextureUnit(GLenum unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, id);
}