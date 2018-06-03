#include "camera.h"

#include "state.h"

void Camera::prepare() {
	State::projectionMatrix = m_projection;

	//Calculate view matix
	// Roll
	glm::mat4 viewMatrix = glm::rotate(glm::mat4(), glm::radians(-m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// Pitch
	viewMatrix = glm::rotate(viewMatrix, glm::radians(-m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	// Yaw
	viewMatrix = glm::rotate(viewMatrix, glm::radians(-m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::translate(viewMatrix, -m_position);
	State::viewMatrix = viewMatrix;

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glScissor(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}