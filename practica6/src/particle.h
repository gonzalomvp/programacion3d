#pragma once

#include "billboard.h"

class Particle;
typedef std::shared_ptr<Particle> ParticlePtr;

class Particle : public Billboard {
public:
	static ParticlePtr create(const MaterialPtr& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade) { 
		return ParticlePtr(new Particle(mat, vel, spinVel, lifetime, autofade), [](Particle* p) { delete p; });
	}

	float        getRemainingLifetime() const      { return m_remaingLifetime; }
	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	Particle(const MaterialPtr& mat, const glm::vec3& vel, float spinVel, float lifetime, bool autofade) : 
		Billboard(mat), m_vel(vel), m_spinVel(spinVel), m_lifetime(lifetime), m_remaingLifetime(lifetime), m_autofade(autofade) {}
	~Particle() {}

	glm::vec3 m_vel;
	float     m_spinVel;
	float     m_lifetime;
	float     m_remaingLifetime;
	bool      m_autofade;
};