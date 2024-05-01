#pragma once

// stl includes
#include <iostream>
#include <vector>
#include <string>
#include <limits>

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
#include "bezier.h"
#include "grid.h"


#ifdef _WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h>
	#include <windows.h>
#endif

class Application
{
public:
	int m_SCR_WIDTH{1440};
	int m_SCR_HEIGHT{1080};
	const float m_viewport_ratio{m_SCR_HEIGHT/static_cast<float>(m_SCR_WIDTH)};
	int m_VIEW_WIDTH{m_SCR_WIDTH*m_viewport_ratio};
	int m_VIEW_HEIGHT{m_SCR_HEIGHT};

	ImGuiIO* m_ioptr{};

	GLFWwindow* m_window{};

	Shader m_shader{};
	Shader m_circleShader{};

	Bezier m_curve{};
	Grid m_grid{m_VIEW_WIDTH, m_VIEW_HEIGHT, 50, 50};

	bool mouseDragging{false};
	double lastX{};
	double lastY{};

	bool vsync{true};

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

	double lastTime{};
	double currentTime{};
	double deltaTime{};
	double totalTime{};
};

static void framebuffer_size_callback(GLFWwindow*, int, int);

static void key_callback(GLFWwindow*, int, int, int, int);

static void mouse_button_callback(GLFWwindow*, int, int, int);

static void cursor_position_callback(GLFWwindow* , double, double);

static void scroll_callback(GLFWwindow*, double, double);
