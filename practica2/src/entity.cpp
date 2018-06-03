#include "entity.h"

void Entity::move(const glm::vec3& vec) {
	m_position += glm::quat(m_rotation) * vec;
}
