#include "entity.h"

void Entity::update(float deltaTime) {
	if (m_callback) {
		m_callback(*this, deltaTime);
	}
}

void Entity::move(const glm::vec3& vec) {
	m_position += m_quat * vec;
}
