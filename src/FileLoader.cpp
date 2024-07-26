#include "FileLoader.h"


void FileLoader::LoadTexturesFromDirectory(std::filesystem::path dir)
{
	namespace stdfs = std::filesystem;

	const stdfs::directory_iterator end{};
	for (stdfs::directory_iterator it{ dir }; it != end; it++)
	{
		std::string path = it->path().string();
		std::replace(path.begin(), path.end(), '\\', '/');
		
		Texture* tex = new Texture(path);
		if (tex->format == Texture::FileFormat::OTHER)
			delete tex;
	}
}