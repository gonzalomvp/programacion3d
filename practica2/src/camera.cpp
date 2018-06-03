#include "camera.h"

#include "state.h"

void Camera::prepare() {
	State::projectionMatrix = m_projection;

	//Calculate view
	/*State::viewMatrix = ;*/

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glScissor(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
}