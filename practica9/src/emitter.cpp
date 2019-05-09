#include "emitter.h"

void Emitter::update(float deltaTime) {
	auto it = m_particles.begin();
	while (it != m_particles.end()) {
		(*it)->update(deltaTime);
		if ((*it)->getRemainingLifetime() <= 0.0f) {
			it = m_particles.erase(it);
		}
		else {
			it++;
		}
	}
	
	if (m_isEmitting) {
		m_pendingParticles += glm::linearRand(m_rateRange[0], m_rateRange[1]) * deltaTime;
		float particlesToEmit = 0.0f;
		m_pendingParticles = glm::modf(m_pendingParticles, particlesToEmit);

		for (size_t i = 0; i < particlesToEmit; ++i) {
			glm::vec3 velocity = glm::vec3(glm::linearRand(m_velocityRange[0].x, m_velocityRange[1].x), glm::linearRand(m_velocityRange[0].y, m_velocityRange[1].y), glm::linearRand(m_velocityRange[0].z, m_velocityRange[1].z));
			float spinVelocity = glm::linearRand(m_spinVelocityRange[0], m_spinVelocityRange[1]);
			float lifetime = glm::linearRand(m_lifetimeRange[0], m_lifetimeRange[1]);
			glm::vec3 scale = glm::vec3(glm::linearRand(m_scaleRange[0], m_scaleRange[1]));
			glm::vec4 color = glm::vec4(glm::linearRand(m_colorRange[0].r, m_colorRange[1].r), glm::linearRand(m_colorRange[0].g, m_colorRange[1].g), glm::linearRand(m_colorRange[0].b, m_colorRange[1].b), glm::linearRand(m_colorRange[0].a, m_colorRange[1].a));
			
			MaterialPtr particleMaterial = Material::create(m_material->getTexture(), m_material->getShader(), color, m_material->getShininess(), m_material->getBlendMode(), m_material->getLighting(), m_material->getCulling(), m_material->getDepthWrite());
			ParticlePtr particle = Particle::create(particleMaterial, velocity, spinVelocity, lifetime, m_autofade);
			particle->setPosition(m_position);
			particle->setScale(scale);
			m_particles.push_back(particle);
		}
	}
}

void Emitter::draw() {
	Entity::draw();

	for (size_t i = 0; i < m_particles.size(); ++i) {
		m_particles[i]->draw();
	}
}