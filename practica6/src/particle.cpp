#include "particle.h"

float Particle::getRemainingLifetime() const {
	return 0;
}

void Particle::update(float deltaTime) {
	m_position += m_vel     * deltaTime;
	m_spin     += m_spinVel * deltaTime;
}