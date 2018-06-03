#include "model.h"
#include "state.h"

void::Model::draw() {
	glm::mat4 modelMatrix = glm::translate(glm::mat4(), m_position);
	// Roll
	glm::rotate(modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// Pitch
	glm::rotate(modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	// Yaw
	glm::rotate(modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::scale(modelMatrix, m_scale);
	State::modelMatrix = modelMatrix;
	m_mesh->draw();
}