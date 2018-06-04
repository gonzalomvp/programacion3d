#include "entity.h"

void Entity::update(float deltaTime) {
	if (m_callback) {
		m_callback(*this, deltaTime);
	}
}

void Entity::move(const glm::vec3& vec) {
	// with quaternions
	m_position += m_rotation * vec;
	// with euler angles
	//m_position += glm::quat(m_rotation) * vec;
}
