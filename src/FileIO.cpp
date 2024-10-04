#include "FileIO.h"


void FileIO::LoadTexturesFromDirectory(std::filesystem::path dir)
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
void FileIO::LoadMaterialsFromDirectory(std::filesystem::path dir)
{
	namespace stdfs = std::filesystem;

	const stdfs::directory_iterator end{};
	for (stdfs::directory_iterator it{ dir }; it != end; it++)
	{
		std::string path = it->path().string();
		std::replace(path.begin(), path.end(), '\\', '/');

		std::string ext;
		std::string::const_iterator chr = path.end();
		while (chr != path.begin() && *chr != '.')
			ext.insert(ext.begin(), *chr);

		if (ext != "mat")
			continue;

		LoadMaterial(path);
	}
}

void FileIO::SerializeScene(Scene* scene, const std::string& outputDir)
{
	std::ofstream output(outputDir, std::ofstream::out | std::ofstream::trunc);
	if (!output)
	{
		std::cout << "Could not open file '" << outputDir << "'" << std::endl;
		return;
	}

	output << "[OBJECTS]\n";
	for (const auto& obj : scene->m_objects)
	{
		std::string line;
		WriteString(line, "NAME", obj->name);
		WriteVec3(line, "POS", obj->position);
		WriteVec3(line, "SCALE", obj->scale);

		output << line << '\n';
	}
	output << "[/OBJECTS]\n";

	output << "[CAMERA]\n";
	std::string line;
	WriteVec3(line, "POS", scene->m_mainCamera->Position);
	WriteFloat(line, "PITCH", scene->m_mainCamera->Pitch);
	WriteFloat(line, "YAW", scene->m_mainCamera->Yaw);
	output << line << "\n[/CAMERA]";

	output.close();
}
Scene* FileIO::LoadScene(const std::string& path)
{
	Scene* scene = new Scene();

	std::ifstream stream(path);
	std::string line;
	while (std::getline(stream, line))
	{
		if (line == "[OBJECTS]")
		{
			while (std::getline(stream, line))
			{
				if (line == "[/OBJECTS]")
					break;

				std::string objName = ReadString(line, "NAME");
				glm::vec3 objPos = ReadVec3(line, "POS");
				glm::vec3 objScale = ReadVec3(line, "SCALE");

				scene->RegisterObject(Object::defaultObject->Instantiate(objPos, objScale));
				//std::cout << "NAME: " << objName << " X: " << objPos.x << " Y: " << objPos.y << " Z: " << objPos.z << '\n' << "SCALEX: " << objScale.x << " SCALEY: " << objScale.y << " SCALEZ: " << objScale.z << std::endl;
			}
		}
		else if (line == "[CAMERA]")
		{
			std::getline(stream, line);
			if (line == "[/CAMERA]")
				continue;

			glm::vec3 cameraPos = ReadVec3(line, "POS");
			float pitch = ReadFloat(line, "PITCH");
			float yaw = ReadFloat(line, "YAW");

			scene->m_mainCamera->SetPosition(cameraPos);
			scene->m_mainCamera->SetRotation(pitch, yaw);
		}
	}

	return scene;
}

void FileIO::SerializeMaterial(Material* mat, const std::string& outputDir)
{
	std::ofstream output(outputDir, std::ofstream::out | std::ofstream::trunc);
	if (!output)
	{
		std::cout << "Could not write to file '" << outputDir << "'\n";
		return;
	}

	std::string line;
	WriteString(line, "DIFFUSE_MAP", mat->diffuseMap->filename);
	WriteString(line, "SPECULAR_MAP", mat->specularMap->filename);
	WriteFloat(line, "SHININESS", mat->shininess);
	WriteVec3(line, "TINT_COLOR", mat->tint);

	output << line;
	output.close();
}
Material* FileIO::LoadMaterial(const std::string& path)
{
	Texture* diff  = nullptr;
	Texture* spec  = nullptr;
	float shine	   = NULL;
	glm::vec3 tint = glm::vec3(NULL);

	std::ifstream stream(path);
	std::string line;
	while (std::getline(stream, line))
	{
		diff = Texture::map[ReadString(line, "DIFFUSE_MAP")];
		spec = Texture::map[ReadString(line, "SPECULAR_MAP")];
		shine = ReadFloat(line, "SHININESS");
		tint = ReadVec3(line, "TINT_COLOR");
	}
	
	Material* mat = new Material(diff, spec, shine, tint);
	mat->filepath = path;
	return mat;
}

glm::vec3 FileIO::ReadVec3(const std::string& line, const std::string& varName)
{
	glm::vec3 result = glm::vec3(NULL);

	int idx = line.find(varName) + varName.length() + 1;
	if (idx == std::string::npos)
		return result;

	std::vector<float> vals;
	std::string val;
	while (line[idx] != ')')
	{
		val += line[idx++];
		if (line[idx] == ' ' || line[idx] == ')')
		{
			vals.push_back(std::stof(val));
			val = "";
		}
		if (line[idx] == ' ')
			idx++;
	}

	for (int i = 0; i < vals.size() && i < 3; i++)
		result[i] = vals[i];

	return result;
}
float FileIO::ReadFloat(const std::string& line, const std::string& varName)
{
	float result = NULL;

	int idx = line.find(varName) + varName.length() + 1;
	if (idx == std::string::npos)
		return result;

	std::string val;
	while (line[idx] != ')')
		val += line[idx++];

	result = std::stof(val);
	return result;
}
std::string FileIO::ReadString(const std::string& line, const std::string& varName)
{
	std::string result;

	int idx = line.find(varName) + varName.length() + 1;
	if (idx == std::string::npos)
		return result;

	while (line[idx] != ')')
		result += line[idx++];

	return result;
}

void FileIO::WriteVec3(std::string& line, const std::string& varName, glm::vec3 value)
{
	line += varName + '(' + std::to_string(value.x) + ' ' + std::to_string(value.y) + ' ' + std::to_string(value.z) + ") ";
}
void FileIO::WriteFloat(std::string& line, const std::string& varName, float value)
{
	line += varName + '(' + std::to_string(value) + ") ";
}
void FileIO::WriteString(std::string& line, const std::string& varName, const std::string& value)
{
	line += varName + '(' + value + ") ";
}