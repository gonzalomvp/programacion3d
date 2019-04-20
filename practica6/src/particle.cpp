#include "particle.h"

void Particle::update(float deltaTime) {
	m_position += m_vel     * deltaTime;
	m_spin     += m_spinVel * deltaTime;
	if (m_spin > 360.f) {
		m_spin -= 360.f;
	}
	else if (m_spin < -360.f) {
		m_spin += 360.f;
	}

	m_remaingLifetime -= deltaTime;

	if (m_autofade) {
		glm::vec4 color = m_material->getColor();
		color.a = glm::max(0.0f, m_remaingLifetime / m_lifetime);
		m_material->setColor(color);
	}
}