#include "particle.h"

void Particle::update(float deltaTime) {
	m_position += m_vel     * deltaTime;
	m_spin     += m_spinVel * deltaTime;
	m_remaingLifetime -= deltaTime;
	glm::vec4 color = m_material->getColor();
	color.a = glm::max(0.0f, m_remaingLifetime / m_lifetime);
	m_material->setColor(color);
}