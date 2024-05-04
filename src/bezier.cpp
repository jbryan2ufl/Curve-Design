#include "bezier.h"

Bezier::Bezier()
{
	// m_control_points.push_back(glm::vec3{0,0,0,1});
	// m_control_points.push_back(glm::vec3{0.25,0.25,0.1,1});
	// m_control_points.push_back(glm::vec3{0.5,0.25,0,1});
	// m_control_points.push_back(glm::vec3{1,0,0.2,1});

	// m_control_points.push_back(glm::vec3{-0.25, 0.7, 0});
	// m_control_points.push_back(glm::vec3{-0.25, 0.5, 0.6});
	// m_control_points.push_back(glm::vec3{1, 0.4, 0.7});
	// m_control_points.push_back(glm::vec3{1, 0, -0.4});
	// m_control_points.push_back(glm::vec3{0.25, -0.1, -1});
	// m_control_points.push_back(glm::vec3{-0.2, -0.3, 0.1});
	m_control_points.push_back(glm::vec3{-1, 1, 0});
	m_control_points.push_back(glm::vec3{0, 1, 0.25});
	m_control_points.push_back(glm::vec3{1, 1, 0});
	m_control_points.push_back(glm::vec3{1, 0, -0.25});
	m_control_points.push_back(glm::vec3{1, -1, 0});
	m_control_points.push_back(glm::vec3{0, -1, 0.25});
	m_control_points.push_back(glm::vec3{-1, -1, 0});
	m_control_points.push_back(glm::vec3{-1, 0, -0.25});
	m_control_points.push_back(glm::vec3{-1, 1, 0});
	m_control_points.push_back(glm::vec3{0, 0, 0});


	m_colors.push_back(glm::vec4{1,0,0,1});
	m_colors.push_back(glm::vec4{0,1,0,1});
	m_colors.push_back(glm::vec4{0,0,1,1});

	m_current_color = m_colors.begin();
}

void Bezier::init()
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_colorVBO);
	glGenBuffers(1, &m_scaffoldingVBO);
	glGenBuffers(1, &m_scaffoldingColorVBO);

	glGenVertexArrays(1, &m_VAO);
	glGenVertexArrays(1, &m_scaffoldingVAO);

	glGenBuffers(1, &m_circleVBO);
	glGenBuffers(1, &m_circleColorVBO);
	glGenVertexArrays(1, &m_circleVAO);
}

void Bezier::addPoint(glm::vec3 p)
{
	m_control_points.push_back(p);
}

void Bezier::removePoint()
{
	m_control_points.pop_back();
}

void Bezier::populateMatrix()
{
	m_data.clear();
	m_color_data.clear();
	for (int i{}; i <= m_resolution; i++)
	{
		m_data.push_back(evaluate((float)i/m_resolution));
		m_color_data.push_back(glm::vec4{(float)i/(float)m_resolution, 1 - (float)i/(float)m_resolution, 1, 1});
	}

	m_intermediate_data.clear();
	std::vector<glm::vec3> points{m_control_points};
	std::vector<glm::vec3> midpoints{};
	glm::vec3 a{};
	glm::vec3 b{};
	while (points.size() > 1)
	{
		int count = points.size() - 1;
		for (int i{}; i < count; ++i)
		{
			a = points[i];
			b = points[i+1];
			midpoints.push_back(glm::vec3{a.x+((b.x-a.x)*m_position), a.y+((b.y-a.y)*m_position), a.z+((b.z-a.z)*m_position)});
		}
		points = midpoints;
		m_intermediate_data.insert(m_intermediate_data.end(), points.begin(), points.end());
		midpoints.clear();
	}

	if (m_showScaffolding)
	{
		m_scaffolding_data.clear();
		m_scaffolding_color_data.clear();
		m_current_color = m_colors.begin();
		for (int i{}; i < m_control_points.size()-1; i++)
		{
			m_scaffolding_data.push_back(m_control_points[i]);
			m_scaffolding_data.push_back(m_control_points[i+1]);
			m_scaffolding_color_data.push_back(glm::vec4{1, 1, 1, 1});
			m_scaffolding_color_data.push_back(glm::vec4{1, 1, 1, 1});
		}
		int count1{m_control_points.size()-1};
		int count2{count1};
		for (size_t i = 0; i < m_intermediate_data.size()-2; i++)
		{
			count2--;
			if (count2 == 0)
			{
				count2 = --count1; 
				m_current_color++;
				if (m_current_color == m_colors.end())
				{
					m_current_color = m_colors.begin();
				}
			}
			else
			{
				m_scaffolding_data.push_back(m_intermediate_data[i]);
				m_scaffolding_data.push_back(m_intermediate_data[i + 1]);

				m_scaffolding_color_data.push_back(*m_current_color);
				m_scaffolding_color_data.push_back(*m_current_color);
			}
		}
	}

	// curve
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_data.size()*sizeof(glm::vec3), m_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
	glBufferData(GL_ARRAY_BUFFER, m_color_data.size()*sizeof(glm::vec4), m_color_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(1);

	// scaffolding
	glBindVertexArray(m_scaffoldingVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_scaffoldingVBO);
	glBufferData(GL_ARRAY_BUFFER, m_scaffolding_data.size()*sizeof(glm::vec3), m_scaffolding_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_scaffoldingColorVBO);
	glBufferData(GL_ARRAY_BUFFER, m_scaffolding_color_data.size()*sizeof(glm::vec4), m_scaffolding_color_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(1);
	
	// circles
	glBindVertexArray(m_circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_circleVBO);
	glBufferData(GL_ARRAY_BUFFER, m_control_points.size()*sizeof(glm::vec3), m_control_points.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Bezier::draw()
{
	glLineWidth(4.0f);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_LINE_STRIP, 0, m_data.size());

	glLineWidth(1.0f);
	if (m_showScaffolding)
	{
		glBindVertexArray(m_scaffoldingVAO);
		glDrawArrays(GL_LINES, 0, m_scaffolding_data.size());
	}

	glBindVertexArray(0);
}

glm::vec3 Bezier::evaluate(float t)
{
	std::vector<glm::vec3> points{m_control_points};

	while (points.size() > 1)
	{
		std::vector<glm::vec3> new_points;
		for (size_t i = 0; i < points.size() - 1; ++i)
		{
			glm::vec3 interpolated_point = (1 - t) * points[i] + t * points[i + 1];
			new_points.push_back(interpolated_point);
		}
		points = new_points;
	}

	return points[0];
}

void Bezier::print(glm::mat4 m)
{
	for (int i{}; i < 4; i++)
	{
		for (int j{}; j < 4; j++)
		{
			std::cout << m[i][j] << ' ';
		}
		std::cout << '\n';
	}
}
void Bezier::print(glm::vec3 v)
{
	std::cout << '(';
	for (int i{}; i < 4; i++)
	{
		std::cout << v[i];
		if (i != 3)
		{
			std::cout << ", ";
		}
	}
	std::cout << ")\n";
}

glm::vec3& Bezier::closestPoint(glm::vec3 mousePos)
{
	double minDistance{std::numeric_limits<double>::max()};
	int closestIndex = -1;

	for (size_t i = 0; i < m_control_points.size(); ++i)
	{
		// std::cout << "COMPARE CONTROL POINT "<<i<<'\n';
		// print(mousePos);
		// print(m_control_points[i]);
		double distance = glm::distance(mousePos, m_control_points[i]);
		// std::cout << "DISTANCE: "<<distance<<"\n\n";
		if (distance < minDistance) {
			minDistance = distance;
			closestIndex = static_cast<int>(i);
		}
	}
	return m_control_points[closestIndex];
}

void Bezier::printDebug()
{
	std::cout << "M_DATA\n";
	for (auto data : m_scaffolding_data)
	{
		print(data);
	}
	std::cout << '\n';
}

void Bezier::drawControls()
{
	glBindVertexArray(m_circleVAO);
	glDrawArrays(GL_POINTS, 0, m_control_points.size());
	glBindVertexArray(0);
}

void Bezier::movePoint(glm::vec3 newPos)
{
	*m_selected_point = newPos;
	calculateCenterPoint();
}

void Bezier::calculateCenterPoint()
{
	m_center_point = glm::vec3{0};

	for (auto& p : m_data)
	{
		m_center_point += p;
	}
	m_center_point /= static_cast<float>(m_data.size());
}
