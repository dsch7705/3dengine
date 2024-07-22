#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


class Shader
{
public:
	Shader();
	Shader(const std::string& filepath);

	unsigned int id;
	void use();

	void setFloat(const std::string& uniformName, float v0);
	void setInt(const std::string& uniformName, int v0);
	void setBool(const std::string& uniformName, bool v0);

	void setVec3(const std::string& uniformName, glm::vec3 vec);
	void setVec4(const std::string& uniformName, glm::vec4 vec);

	void setMat4(const std::string& uniformName, glm::mat4& mat, bool transpose);

private:

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	static ShaderProgramSource ParseShader(const std::string& filepath);
	static unsigned int CompileShader(unsigned int type, const std::string& source);
	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};