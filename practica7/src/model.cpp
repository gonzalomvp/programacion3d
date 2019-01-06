#include "model.h"
#include "state.h"

void Model::draw() {
	glm::mat4 modelMatrix = glm::translate(glm::mat4(), m_position);
	modelMatrix = glm::rotate(modelMatrix, glm::angle(m_quat), glm::axis(m_quat));
	modelMatrix = glm::scale(modelMatrix, m_scale);
	State::modelMatrix = modelMatrix;
	m_mesh->draw();
}