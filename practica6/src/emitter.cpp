#include "emitter.h"

void Emitter::update(float deltaTime) {
	for (size_t i = 0; i < m_particles.size(); ++i) {
		m_particles[i]->update(deltaTime);
	}
	
	m_pendingParticles += glm::linearRand(m_rateRange[0], m_rateRange[1]) * deltaTime;
	float particlesToEmit = 0.0f;
	m_pendingParticles = glm::modf(m_pendingParticles, particlesToEmit);

	for (size_t i = 0; i < particlesToEmit; ++i) {
		glm::vec3 velocity = glm::vec3(glm::linearRand(m_velocityRange[0].x, m_velocityRange[1].x), glm::linearRand(m_velocityRange[0].y, m_velocityRange[1].y), glm::linearRand(m_velocityRange[0].z, m_velocityRange[1].z));
		float spinVelocity = glm::linearRand(m_spinVelocityRange[0], m_spinVelocityRange[1]);
		float lifetime = glm::linearRand(m_lifetimeRange[0], m_lifetimeRange[1]);
		ParticlePtr particle = Particle::create(m_material, velocity, spinVelocity, lifetime, m_autofade);
		particle->setPosition(m_position);
		particle->setScale(glm::vec3(glm::linearRand(m_scaleRange[0], m_scaleRange[1])));
		m_particles.push_back(particle);
	}
}

void Emitter::draw() {
	for (size_t i = 0; i < m_particles.size(); ++i) {
		m_particles[i]->draw();
	}
}