#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <glm/glm.hpp>
#include <random>
#include <ctime>

class Object
{
	glm::vec3 random_color();

public:
	Object();

	std::vector<glm::vec3> vertex_data{};
	std::vector<glm::vec3> vertex_data_copy{};
	std::vector<glm::vec3> color_data{};
	std::vector<glm::vec3> normal_data{};
	std::vector<glm::vec3> full_vertex_data{};
	std::vector<glm::vec3> full_color_data{};
	std::vector<unsigned int> index_data{};
	std::vector<unsigned int> full_index_data{};
	std::vector<unsigned int> normal_index_data{};

	glm::vec3 normal_to_color(glm::vec3);
	void print_vec(glm::vec3);

	void load_file(std::string);
	std::string filename{};

};

#endif