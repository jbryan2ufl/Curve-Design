#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID{};
	unsigned int vertex{};
	unsigned int fragment{};
	unsigned int geometry{};

	Shader(){};
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
	void setupShader(const char* shaderPath, unsigned int& shaderID, GLenum shaderType);
	void use();
	void checkCompileErrors(unsigned int, std::string);
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setFloat(const std::string &name, float f) const;
	void setVec3(const std::string &name, const glm::vec3 &vec) const;
	void setVec4(const std::string &name, const glm::vec4 &vec) const;
};