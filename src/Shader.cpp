#include "Shader.h"

Shader::Shader() 
{}
Shader::Shader(const std::string& filepath)
{
	Shader::ShaderProgramSource source = ParseShader(filepath);
	id = CreateShader(source.VertexSource, source.FragmentSource);
}

void Shader::setFloat(const std::string& uniformName, float v0)
{
	use();
	glUniform1f(glGetUniformLocation(id, uniformName.c_str()), v0);
}
void Shader::setInt(const std::string& uniformName, int v0)
{
	use();
	glUniform1i(glGetUniformLocation(id, uniformName.c_str()), v0);
}
void Shader::setBool(const std::string& uniformName, bool v0)
{
	use();
	glUniform1i(glGetUniformLocation(id, uniformName.c_str()), (int)v0);
}

void Shader::setVec3(const std::string& uniformName, glm::vec3 vec)
{
	use();
	glUniform3fv(glGetUniformLocation(id, uniformName.c_str()), 1, &vec[0]);
}
void Shader::setVec4(const std::string& uniformName, glm::vec4 vec)
{
	use();
	glUniform4fv(glGetUniformLocation(id, uniformName.c_str()), 1, &vec[0]);
}

void Shader::setMat4(const std::string& uniformName, glm::mat4& mat, bool transpose)
{
	use();
	glUniformMatrix4fv(glGetUniformLocation(id, uniformName.c_str()), 1, transpose, glm::value_ptr(mat));
}

void Shader::use()
{
	glUseProgram(id);
}

Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);
		std::cout << "Failed to compile shader program!" << std::endl;
		std::cout << message << std::endl;
		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}