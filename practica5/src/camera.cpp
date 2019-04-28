#include "camera.h"

#include "state.h"

void Camera::prepare() {
	State::projectionMatrix = m_projection;

	//Calculate view matrix
	glm::mat4 viewMatrix = glm::rotate(glm::mat4(), -glm::angle(m_quat), glm::axis(m_quat));
	viewMatrix = glm::translate(viewMatrix, -m_position);
	State::viewMatrix = viewMatrix;

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glScissor(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}