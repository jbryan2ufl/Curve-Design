#include "shader.h"

// code from https://learnopengl.com/Getting-started/Shaders

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	ID=glCreateProgram();
	setupShader(vertexPath, vertex, GL_VERTEX_SHADER);
	setupShader(fragmentPath, fragment, GL_FRAGMENT_SHADER);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	ID=glCreateProgram();
	setupShader(vertexPath, vertex, GL_VERTEX_SHADER);
	setupShader(fragmentPath, fragment, GL_FRAGMENT_SHADER);
	setupShader(geometryPath, geometry, GL_GEOMETRY_SHADER);
}

void Shader::setupShader(const char* shaderPath, unsigned int& shaderID, GLenum shaderType)
{
	std::string shaderCodeString;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCodeString = shaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		return;
	}

	if (shaderID != 0) {
		glDeleteShader(shaderID);
	}

	const char* shaderCode{shaderCodeString.c_str()};

	glDetachShader(ID, shaderID);
	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);
	checkCompileErrors(shaderID, "SHADER");
	glAttachShader(ID, shaderID);
	glDeleteShader(shaderID);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
}

void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setFloat(const std::string &name, float f) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), f);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &vec) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}