#ifndef BEZIER_H
#define BEZIER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <limits>

class Bezier
{
public:
	unsigned int m_VAO{};
	unsigned int m_VBO{};
	unsigned int m_colorVBO{};
	unsigned int m_scaffoldingVAO{};
	unsigned int m_scaffoldingVBO{};
	unsigned int m_scaffoldingColorVBO{};

	unsigned int m_circleVAO{};
	unsigned int m_circleVBO{};
	unsigned int m_circleColorVBO{};

	Bezier();
	void addPoint(glm::vec4 p);
	void removePoint();
	std::vector<glm::vec4> m_control_points{};
	glm::mat4 m_points{};

	bool m_showScaffolding{true};

	float m_position{0.5};

	unsigned int m_resolution{50};

	std::vector<glm::vec4> m_data{};
	std::vector<glm::vec4> m_color_data{};
	std::vector<glm::vec4> m_intermediate_data{};
	std::vector<glm::vec4> m_scaffolding_data{};
	std::vector<glm::vec4> m_scaffolding_color_data{};

	void populateMatrix();
	glm::vec4 evaluate(float t);
	void print(glm::mat4);
	void print(glm::vec4);
	void printDebug();

	void draw();
	void drawControls();

	void init();

	glm::vec4& closestPoint(double mouseX, double mouseY);

	glm::vec4* m_selected_point{};

private:
	std::vector<glm::vec4> m_colors{};
	std::vector<glm::vec4>::iterator m_current_color{};
};

#endif