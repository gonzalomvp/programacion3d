#include "model.h"
#include "state.h"

void::Model::draw() {
	glm::mat4 modelMatrix = glm::translate(glm::mat4(), m_position);
	// with quaternions
	modelMatrix = glm::rotate(modelMatrix, glm::angle(m_rotation), glm::axis(m_rotation));
	// with euler angles
	//modelMatrix = glm::rotate(modelMatrix, glm::angle(glm::quat(glm::radians(m_rotation))), glm::axis(glm::quat(glm::radians(m_rotation))));
	modelMatrix = glm::scale(modelMatrix, m_scale);
	State::modelMatrix = modelMatrix;
	m_mesh->draw();
}