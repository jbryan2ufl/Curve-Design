#ifndef GRID_H
#define GRID_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <limits>

class Grid
{
public:
	unsigned int m_VAO{};
	unsigned int m_VBO{};
	unsigned int m_colorVBO{};

	unsigned int m_width{};
	unsigned int m_height{};

	unsigned int m_xCount{};
	unsigned int m_yCount{};

	std::vector<glm::vec4> m_data{};
	std::vector<glm::vec4> m_color_data{};

	glm::vec4 m_line_color{1, 1, 1, 0.1};

	Grid(unsigned int width, unsigned int height, unsigned int xCount, unsigned int yCount);

	void populate();
	
	void draw();

	void init();

private:
};

#endif