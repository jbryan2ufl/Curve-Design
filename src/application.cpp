#pragma once

#include "application.h"

void Application::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		totalTime += deltaTime;
		
		m_curve.m_position = static_cast<float>(fmod(totalTime * 0.1, 2.0));
		if (m_curve.m_position > 1.0f) {
			m_curve.m_position = 2.0f - m_curve.m_position;
		}

		process_input();
		draw();
		glfwPollEvents();

		lastTime = currentTime;
	}
}

void Application::draw()
{
	// glClearColor(8/255.0f, 10/255.0f, 23/255.0f, 1);
	// glClearColor(47/255.0f, 85/255.0f, 212/255.0f, 1);
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_modelViewProjectionMatrix = glm::mat4{1.0f};
	m_view = m_camera.getViewMatrix();
	// m_projection = glm::perspective(glm::radians(m_camera.m_fov), static_cast<float>(m_VIEW_WIDTH) / m_VIEW_HEIGHT, m_camera.m_nearPlane, m_camera.m_farPlane);
	m_projection = glm::ortho(-1.0f, +1.0f, -1.0f, +1.0f, 0.1f, 100.0f);

	m_model = m_translate * m_scale * m_rotate;
	for (auto& m : m_modelViewProjectionComponents)
	{
		m_modelViewProjectionMatrix*=*m;
	}

	m_gridShader.use();
	m_grid.draw();

	m_shader.use();
	m_shader.setMat4("mvpMatrix", m_modelViewProjectionMatrix);

	m_curve.populateMatrix();
	m_curve.draw();

	m_circleShader.use();
	m_circleShader.setMat4("mvpMatrix", m_modelViewProjectionMatrix);
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
		if (ImGui::Button("Reset All Matrices"))
		{
			m_translate = glm::mat4{1.0f};
			m_rotate = glm::mat4{1.0f};
			m_scale = glm::mat4{1.0f};
			m_zoom = 1;
		}

		ImGui::Checkbox("Scaffolding", &m_curve.m_showScaffolding);

		ImGui::Text("Resolution: %i",m_curve.m_resolution);

		ImGui::Text("Zoom: %f",m_zoom);

		ImGui::Text("Scaffold count: %i",m_curve.m_scaffolding_data.size());

		ImGui::Text("Rotate");
		if (ImGui::BeginTable("", 4))
		{
			for (int i{}; i < 4; i++)
			{
				ImGui::TableNextRow();
				for (int j{}; j < 4; j++)
				{
					ImGui::TableSetColumnIndex(j);
					ImGui::Text(std::to_string(m_modelViewProjectionMatrix[j][i]).c_str());
				}
			}
			ImGui::EndTable();
		}
		ImGui::NewLine();

	ImGui::Text("Control Points");
	for (int i{}; i<m_curve.m_control_points.size(); i++)
	{
		ImGui::DragFloat3(std::string("##"+std::to_string(i)).c_str(), &m_curve.m_control_points[i].x, 0.005f);
	}

		ImGui::Text("Center Point: %f, %f, %f, %f", m_curve.m_center_point.x, m_curve.m_center_point.y, m_curve.m_center_point.z);
		ImGui::NewLine();

		ImGui::Text("Mouse Pos 3D: %f, %f, %f", m_mousePos3D.x, m_mousePos3D.y, m_mousePos3D.z);
		ImGui::Text("Mouse Pos: %f, %f", m_mousePos.x, m_mousePos.y);
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

	glEnable(GL_DEPTH_TEST);
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
	m_gridShader = Shader("src/grid.vs", "src/source.fs");

	m_curve.init();
	m_curve.populateMatrix();
	m_curve.calculateCenterPoint();

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

	m_modelViewProjectionComponents.push_back(&m_projection);
	m_modelViewProjectionComponents.push_back(&m_view);
	m_modelViewProjectionComponents.push_back(&m_translate);
	m_modelViewProjectionComponents.push_back(&m_scale);
	m_modelViewProjectionComponents.push_back(&m_rotate);
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
		if (key == GLFW_KEY_DELETE)
		{
			m_curve.removePoint();
		}
	}
}

void Application::updateMousePos3D()
{
	m_mousePos = normalizePoint(lastX, lastY);

	m_mousePos3D = glm::inverse(m_model) * glm::vec4{m_mousePos, 0, 1};
}

glm::vec2 Application::normalizePoint(double x, double y)
{
	x=x/m_VIEW_WIDTH*2-1;
	y=y/m_VIEW_HEIGHT*2-1;

	return glm::vec2{x, -y};
}

void Application::process_mouse_button(int button, int action, int mods)
{
	if (!m_ioptr->WantCaptureMouse)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseDragging = true;
			glfwGetCursorPos(m_window, &lastX, &lastY);
			m_curve.m_selected_point=&m_curve.closestPoint(m_mousePos3D);
			
			double distance = glm::distance(m_mousePos3D, *m_curve.m_selected_point);
			// std::cout << "DISTANCE: " << distance << '\n';
			// std::cout << mousePos3D.x << ' ' << mousePos3D.y << ' ' << mousePos3D.z << '\n';
			// std::cout << m_curve.m_selected_point->x << ' ' << m_curve.m_selected_point->y << ' ' << m_curve.m_selected_point->z << '\n';
			if (distance < 0.1)
			{
				m_curve.movePoint(m_mousePos3D);
			}
			else
			{
				m_curve.addPoint(m_mousePos3D);
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
		updateMousePos3D();

		if (mouseDragging && m_curve.m_selected_point != nullptr)
		{
			*m_curve.m_selected_point = m_mousePos3D;
		}
	}
}

void Application::process_scroll(double xoffset, double yoffset)
{
	yoffset > 0 ? m_zoom *= 1.1f : m_zoom /= 1.1f;

	// updateMousePos3D();
	glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_mousePos, 0.0f});

	glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3{m_zoom});

	glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), -glm::vec3{m_mousePos, 0.0f});

	m_scale = translateToCenter * scale * translateBack;
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
	if (glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS)
	{
		m_translate = glm::translate(m_translate, glm::vec3{0.0f, 0.01f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_J) == GLFW_PRESS)
	{
		m_translate = glm::translate(m_translate, glm::vec3{-0.01f, 0.0f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS)
	{
		m_translate = glm::translate(m_translate, glm::vec3{0.0f, -0.01f, 0.0f});
	}
	if (glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS)
	{
		m_translate = glm::translate(m_translate, glm::vec3{0.01f, 0.0f, 0.0f});
	}

	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_curve.m_center_point});

		glm::mat4 rotate = glm::rotate(glm::mat4{1.0}, 0.01f, glm::vec3{glm::row(m_rotate, 2)});

		glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3{-m_curve.m_center_point});

		m_rotate *= translateToCenter * rotate * translateBack;
	}
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_curve.m_center_point});

		glm::mat4 rotate = glm::rotate(glm::mat4{1.0}, -0.01f, glm::vec3{glm::row(m_rotate, 2)});

		glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3{-m_curve.m_center_point});

		m_rotate *= translateToCenter * rotate * translateBack;
	}
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_curve.m_center_point});

		glm::mat4 rotate = glm::rotate(glm::mat4{1.0}, 0.01f, glm::vec3{glm::row(m_rotate, 0)});

		glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3{-m_curve.m_center_point});

		m_rotate *= translateToCenter * rotate * translateBack;
	}
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_curve.m_center_point});

		glm::mat4 rotate = glm::rotate(glm::mat4{1.0}, -0.01f, glm::vec3{glm::row(m_rotate, 0)});

		glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3{-m_curve.m_center_point});

		m_rotate *= translateToCenter * rotate * translateBack;
	}
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_curve.m_center_point});

		glm::mat4 rotate = glm::rotate(glm::mat4{1.0}, 0.01f, glm::vec3{glm::row(m_rotate, 1)});

		glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3{-m_curve.m_center_point});

		m_rotate *= translateToCenter * rotate * translateBack;
	}
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_curve.m_center_point});

		glm::mat4 rotate = glm::rotate(glm::mat4{1.0}, -0.01f, glm::vec3{glm::row(m_rotate, 1)});

		glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), glm::vec3{-m_curve.m_center_point});

		m_rotate *= translateToCenter * rotate * translateBack;
	}
	updateMousePos3D();
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
