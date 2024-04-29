#pragma once

// stl includes
#include <iostream>
#include <vector>
#include <string>

// render includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <filesystem>

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.h"
#include "object.h"

struct Transformation
{
	std::string name{};
	glm::mat4 t{};
};

class Application
{
public:
	// settings


	int m_SCR_WIDTH{1440};
	int m_SCR_HEIGHT{1080};
	const float m_viewport_ratio{m_SCR_HEIGHT/static_cast<float>(m_SCR_WIDTH)};
	int m_VIEW_WIDTH{m_SCR_WIDTH*m_viewport_ratio};
	int m_VIEW_HEIGHT{m_SCR_HEIGHT};

	ImGuiIO* m_ioptr{};

	GLFWwindow* m_window{};

	Shader m_shader{};

	glm::mat4 modelTransformation{1.0f};

	Transformation scale{"Scale", glm::mat4{1.0f}};
	Transformation rotate{"Rotate", glm::mat4{1.0f}};
	Transformation translate{"Translate", glm::mat4{1.0f}};
	std::vector<Transformation*> modelTransformationComponents{};

	std::vector<std::string> obj_names{};

	unsigned int m_VAO;
	unsigned int m_ColorVBO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	bool firstMouse{};
	float lastX{};
	float lastY{};

	bool vsync{true};
	bool useGPU{true};
	bool wireframe{false};
	bool useEBO{true};

	Object* obj;

	void draw();

	void process_input();

	public:

	Application();

	void init();

	void process_framebuffer_size(int, int);

	void process_key(int, int, int, int);

	void process_mouse_button(int, int, int);

	void process_cursor_position(double, double);

	void process_scroll(double, double);

	void run();

	void close();

	void reload_data();

	void print_debug();
};

static void framebuffer_size_callback(GLFWwindow*, int, int);

static void key_callback(GLFWwindow*, int, int, int, int);

static void mouse_button_callback(GLFWwindow*, int, int, int);

static void cursor_position_callback(GLFWwindow* , double, double);

static void scroll_callback(GLFWwindow*, double, double);
