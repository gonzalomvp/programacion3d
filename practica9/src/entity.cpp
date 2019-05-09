#include "entity.h"

#include "state.h"

void Entity::move(const glm::vec3& vec) {
	m_position += m_quat * vec;
}

void Entity::update(float deltaTime) {
	if (m_callback) {
		m_callback(*this, deltaTime);
	}
}

void Entity::draw() {
	State::animation = false;
}