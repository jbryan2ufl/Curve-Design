#pragma once

#include "application.h"

void Application::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		totalTime += deltaTime;
		
		// m_curve.m_position = static_cast<float>(fmod(totalTime, 2.0));
		// if (m_curve.m_position > 1.0f) {
		// 	m_curve.m_position = 2.0f - m_curve.m_position;
		// }

		process_input();
		draw();
		glfwPollEvents();

		lastTime = currentTime;
	}
}

void Application::draw()
{
	glClearColor(8/255.0f, 10/255.0f, 23/255.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	m_shader.use();

	m_curve.populateMatrix();
	m_grid.draw();
	m_curve.draw();

	m_circleShader.use();
	m_circleShader.setVec4("color", glm::vec4{1.0, 0.8, 0, 0.5});

	m_curve.drawControls();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowBgAlpha(1.0f);
	glfwGetWindowSize(m_window, &m_SCR_WIDTH, &m_SCR_HEIGHT);
	ImGui::SetNextWindowPos(ImVec2{m_SCR_WIDTH*m_viewport_ratio,0});
	ImGui::SetNextWindowSize(ImVec2{m_SCR_WIDTH*(1-m_viewport_ratio),m_SCR_HEIGHT});
	if (ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration))
	{
		ImGui::Checkbox("Scaffolding", &m_curve.m_showScaffolding);
		if (ImGui::Button("Print"))
		{
			m_curve.printDebug();
		}
		ImGui::Text("Resolution: %i",m_curve.m_resolution);
		ImGui::Text("Scaffold count: %i",m_curve.m_scaffolding_data.size());
	}
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_window);
}

Application::Application()
{}

void Application::init()
{
	// glfw initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	// glfw window creation
	m_window = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "Curve Design", NULL, NULL);

	// check if window is created
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}

	// glfw callback configuration
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	// glfw mouse capture
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(vsync);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(10.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// imgui configuration
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	m_ioptr=&io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();

	// start glew
	glewInit();

	m_shader = Shader("src/source.vs", "src/source.fs");
	m_circleShader = Shader("src/source.vs", "src/circle.fs");

	m_curve.init();
	m_curve.populateMatrix();

	m_grid.init();
	m_grid.populate();

	glfwSetWindowUserPointer(m_window, this);

	glViewport(0, 0, m_SCR_WIDTH*m_viewport_ratio, m_SCR_HEIGHT);

	// #ifdef _WIN32
	// 	HWND hwnd = (HWND)glfwGetWin32Window(m_window);
	// 	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
	// 	style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	// 	SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_EX_LAYERED);

	// 	style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	// 	SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	// 	SetLayeredWindowAttributes(hwnd, RGB(255, 0, 0), 128, LWA_COLORKEY);
	// #endif
}


void Application::process_framebuffer_size(int width, int height)
{
	glViewport(0, 0, width*m_viewport_ratio, height);
}

// FOR SINGLE PRESSES
void Application::process_key(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(m_window, true);
			close();
		}
		// if (key == GLFW_KEY_DOWN)
		// {
		// 	m_curve.m_resolution = (m_curve.m_resolution > 1) ? (m_curve.m_resolution - 1) : 1;
		// }
		// if (key == GLFW_KEY_UP)
		// {
		// 	m_curve.m_resolution += 1;
		// }
		if (key == GLFW_KEY_BACKSPACE)
		{
			m_curve.removePoint();
		}
	}
}

void Application::process_mouse_button(int button, int action, int mods)
{
	if (!m_ioptr->WantCaptureMouse)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseDragging = true;
			glfwGetCursorPos(m_window, &lastX, &lastY);
			m_curve.m_selected_point=&m_curve.closestPoint(((lastX/m_VIEW_WIDTH)-0.5)*2, -((lastY/m_VIEW_HEIGHT)-0.5)*2);

			double distance = glm::length(glm::vec2{m_curve.m_selected_point->x - ((lastX/m_VIEW_WIDTH)-0.5)*2, m_curve.m_selected_point->y - -((lastY/m_VIEW_HEIGHT)-0.5)*2});
			if (distance < 0.1)
			{
				*m_curve.m_selected_point = glm::vec4{((lastX/m_VIEW_WIDTH)-0.5)*2, -((lastY/m_VIEW_HEIGHT)-0.5)*2, 0, 1};
			}
			else
			{
				m_curve.addPoint(glm::vec4{((lastX/m_VIEW_WIDTH)-0.5)*2, -((lastY/m_VIEW_HEIGHT)-0.5)*2, 0, 1});
				m_curve.m_selected_point = &m_curve.m_control_points.back();
			}
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			mouseDragging = false;
		}
	}
}

void Application::process_cursor_position(double xposIn, double yposIn)
{
	if (!m_ioptr->WantCaptureMouse)
	{
		float xpos=static_cast<float>(xposIn);
		float ypos=static_cast<float>(yposIn);

		float xoffset = xpos-lastX;
		float yoffset = ypos-lastY;

		lastX=xpos;
		lastY=ypos;

		if (mouseDragging && m_curve.m_selected_point != nullptr)
		{
			*m_curve.m_selected_point = glm::vec4{((lastX/m_VIEW_WIDTH)-0.5)*2, -((lastY/m_VIEW_HEIGHT)-0.5)*2, 0, 1};
		}
	}
}

void Application::process_scroll(double xoffset, double yoffset)
{
}

void Application::process_input()
{
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// firstMouse=true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_curve.m_position+=0.01;
		if (m_curve.m_position>1)
		{
			m_curve.m_position=1;
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_curve.m_position-=0.01;
		if (m_curve.m_position<0)
		{
			m_curve.m_position=0;
		}
	}
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_curve.m_resolution += 1;
	}
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_curve.m_resolution = (m_curve.m_resolution > 1) ? (m_curve.m_resolution - 1) : 1;
	}
}

void Application::close()
{
	glfwTerminate();
	exit(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_framebuffer_size(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_key(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_mouse_button(button, action, mods);
}

void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_cursor_position(xposIn, yposIn);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_scroll(xoffset, yoffset);
}
