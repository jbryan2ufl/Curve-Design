#include "camera.h"

Camera::Camera()
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::processKeyboardMovement(KeyboardMovement direction, float deltaTime)
{
	float velocity{m_movementSpeed * deltaTime};

	switch (direction)
	{
		case FORWARD:
			m_position += m_front * velocity;
			break;
		case BACKWARD:
			m_position -= m_front * velocity;
			break;

		case LEFT:
			m_position -= m_right * velocity;
			break;
		case RIGHT:
			m_position += m_right * velocity;
			break;

		case UP:
			m_position += m_worldUp * velocity;
			break;
		case DOWN:
			m_position -= m_worldUp * velocity;
			break;
		default:
			break;
	}
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
	xoffset *= m_mouseSensitivity;
	yoffset *= m_mouseSensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	m_front = glm::normalize(glm::vec3{
		cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)),
		sin(glm::radians(m_pitch)),
		sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
	});

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
