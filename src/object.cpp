#include "object.h"

void Object::load_file(std::string fn)
{
	filename=fn;
	vertex_data.clear();
	vertex_data_copy.clear();
	color_data.clear();
	normal_data.clear();
	full_vertex_data.clear();
	full_color_data.clear();
	index_data.clear();
	full_index_data.clear();
	normal_index_data.clear();

	std::ifstream file{"data/"+filename};

	if (!file)
	{
		std::cerr << "file could not be loaded\n";
		exit(1);
	}

	std::string line{};
	while (std::getline(file, line))
	{
		std::stringstream ss{line};
		std::string term{};
		ss >> term;
		if (term == "v")
		{
			glm::vec3 vertex{};
			for (int i{}; i < 3; i++)
			{
				ss >> term;
				vertex[i]=std::stof(term);
			}
			vertex_data.push_back(vertex);
			vertex_data_copy.push_back(vertex);
		}
		
		if (term == "vn")
		{
			glm::vec3 vertex{};
			for (int i{}; i < 3; i++)
			{
				ss >> term;
				vertex[i]=std::stof(term);
			}
			normal_data.push_back(vertex);
		}

		if (term == "f")
		{

			std::vector<unsigned int> face_vertices{};
			unsigned int temp{};
			while(ss >> term)
			{
				temp=std::stoi(term.substr(0,term.find('/')))-1;
				face_vertices.push_back(temp);
				index_data.push_back(temp);
				normal_index_data.push_back(std::stoi(term.substr(term.rfind('/')+1, -1))-1);
			}
			while (face_vertices.size() > 2)
			{
				full_vertex_data.push_back(vertex_data[face_vertices[0]]);

				full_vertex_data.push_back(vertex_data[face_vertices.end()[-2]]);

				full_vertex_data.push_back(vertex_data[face_vertices.end()[-1]]);

				face_vertices.pop_back();
			}
			while (index_data.size() > 2)
			{
				if (normal_data.size() != 0)
				{
					full_color_data.push_back(normal_to_color(normal_data[normal_index_data[0]]));
					full_color_data.push_back(normal_to_color(normal_data[normal_index_data.end()[-2]]));
					full_color_data.push_back(normal_to_color(normal_data[normal_index_data.end()[-1]]));
				}
				else
				{
					full_color_data.push_back(normal_to_color(glm::normalize(vertex_data[index_data[0]])));
					full_color_data.push_back(normal_to_color(glm::normalize(vertex_data[index_data.end()[-2]])));
					full_color_data.push_back(normal_to_color(glm::normalize(vertex_data[index_data.end()[-1]])));

					// full_color_data.push_back(random_color());
					// full_color_data.push_back(random_color());
					// full_color_data.push_back(random_color());
				}

				full_index_data.push_back(index_data[0]);
				full_index_data.push_back(index_data.end()[-2]);
				full_index_data.push_back(index_data.end()[-1]);

				index_data.pop_back();
				normal_index_data.pop_back();
			}
			index_data.clear();
			normal_index_data.clear();
		}
	}

	for (int i{}; i < vertex_data.size(); i++)
	{
		color_data.push_back(normal_to_color(glm::normalize(vertex_data[i])));
	}
}

Object::Object()
{
	std::srand(std::time(nullptr));

	load_file("cow.obj");
}

glm::vec3 Object::normal_to_color(glm::vec3 normal)
{
	return (normal+glm::vec3{1.0f})/2.0f;
}

glm::vec3 Object::random_color()
{
	return glm::vec3{static_cast<float>(std::rand())/RAND_MAX, static_cast<float>(std::rand())/RAND_MAX, static_cast<float>(std::rand())/RAND_MAX};
}

void Object::print_vec(glm::vec3 vec)
{
	std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << '\n';
}