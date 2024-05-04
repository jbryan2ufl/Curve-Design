#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum KeyboardMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	glm::vec3 m_position{0.0f, 0.0f, 5.0f};
	glm::vec3 m_front{0.0f, 0.0f, -1.0f};
	glm::vec3 m_up{0.0f, 1.0f, 0.0f};
	glm::vec3 m_right{};
	glm::vec3 m_worldUp{m_up};

	float m_yaw{-90.0f};
	float m_pitch{0.0f};
	float m_movementSpeed{2.5f};
	float m_mouseSensitivity{0.1f};
	float m_fov{60.0f};

	float m_nearPlane{0.1};
	float m_farPlane{100};

	Camera();

	glm::mat4 getViewMatrix();

	void processKeyboardMovement(KeyboardMovement direction, float deltaTime);

	void processMouseMovement(float xoffset, float yoffset);

private:
	void updateCameraVectors();
};

#endif