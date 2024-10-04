#include "Texture.h"

Texture::Texture(const std::string& filepath)
{
	path = filepath;
	if (path.find(".jpg") != std::string::npos)
		format = FileFormat::JPG;
	else if (path.find(".png") != std::string::npos)
		format = FileFormat::PNG;
	else
		format = FileFormat::OTHER;
	if (format == FileFormat::OTHER)
		return;

	std::string name;
	std::string::const_iterator it = filepath.end() - 1;
	while (it > filepath.begin() && *it != '/')
	{
		name.insert(name.begin(), *it);
		it--;
	}

	filename = name;
	key = filename;
	data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);
	
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	map[key] = this;
}
Texture::~Texture()
{
	glDeleteTextures(1, &id);
	map.erase(key);
}

void Texture::SetTextureUnit(GLenum unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::GenerateDefaultTexture()
{
	if (defaultTexture != nullptr)
	{
		std::cout << "Default texture already initialized!\n";
		return;
	}

	defaultTexture = new Texture();
}

Texture::Texture()
{
	filename = "default";
	key = filename;
	format = FileFormat::PNG;

	data = new unsigned char[4];
	data[0] = 255;	// R
	data[1] = 255;	// G
	data[2] = 255;	// B
	data[3] = 255;	// A

	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, (GLenum)format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	free(data);

	map[key] = this;
}