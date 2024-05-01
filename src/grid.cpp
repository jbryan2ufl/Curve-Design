#include "grid.h"

Grid::Grid(unsigned int width, unsigned int height, unsigned int xCount, unsigned int yCount)
	: m_width{width}
	, m_height{height}
	, m_xCount{xCount}
	, m_yCount{yCount}
{
}

void Grid::populate()
{
	float xStep{2/static_cast<float>(m_xCount)};
	for (int i{}; i < m_xCount; i++)
	{
		m_data.push_back(glm::vec4{-1+i*xStep, -1, 0, 1});
		m_data.push_back(glm::vec4{-1+i*xStep, 1, 0, 1});
		m_color_data.push_back(m_line_color);
		m_color_data.push_back(m_line_color);
	}

	float yStep{2/static_cast<float>(m_yCount)};
	for (int i{}; i < m_xCount; i++)
	{
		m_data.push_back(glm::vec4{-1, -1+i*yStep, 0, 1});
		m_data.push_back(glm::vec4{1, -1+i*yStep, 0, 1});
		m_color_data.push_back(m_line_color);
		m_color_data.push_back(m_line_color);
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_data.size()*sizeof(glm::vec4), m_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, m_color_data.size()*sizeof(glm::vec4), m_color_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}
	
void Grid::draw()
{
	glLineWidth(2.0f);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_LINES, 0, m_data.size());
	glBindVertexArray(0);
}

void Grid::init()
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_colorVBO);
	glGenVertexArrays(1, &m_VAO);
}