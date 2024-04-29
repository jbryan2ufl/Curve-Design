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
	uint32_t ID{};

	Shader(){};
	Shader(const char*, const char*);
	Shader(const char*, const char*, const char*);
	void use();
	void setBool(const std::string&, bool) const;
	void setInt(const std::string&, int32_t);
	void setFloat(const std::string&, float);
	void checkCompileErrors(unsigned int, std::string);
	void setVec2(const std::string &name, const glm::vec2 &value) const;
void setVec2(const std::string &name, float x, float y)const;
// ------------------------------------------------------------------------
void setVec3(const std::string &name, const glm::vec3 &value)const;
void setVec3(const std::string &name, float x, float y, float z)const;
// ------------------------------------------------------------------------
void setVec4(const std::string &name, const glm::vec4 &value)const;
void setVec4(const std::string &name, float x, float y, float z, float w) const;
// ------------------------------------------------------------------------
void setMat2(const std::string &name, const glm::mat2 &mat)const;
// ------------------------------------------------------------------------
void setMat3(const std::string &name, const glm::mat3 &mat)const;
// ------------------------------------------------------------------------
void setMat4(const std::string &name, const glm::mat4 &mat)const;

};