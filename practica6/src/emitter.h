#pragma once

#include "entity.h"
#include "material.h"
#include "particle.h"

class Emitter;
typedef std::shared_ptr<Emitter> EmitterPtr;

class Emitter : public Entity {
public:
	static EmitterPtr create(const MaterialPtr& mat , bool autofade = true) { return EmitterPtr(new Emitter(mat, autofade), [](Emitter* p) { delete p; }); }

	void			setRateRange(float min, float max)                           { m_rateRange[0]         = min; m_rateRange[1]         = max; }
	void			setVelocityRange(const glm::vec3& min, const glm::vec3& max) { m_velocityRange[0]     = min; m_velocityRange[1]     = max; }
	void			setSpinVelocityRange(float min, float max)                   { m_spinVelocityRange[0] = min; m_spinVelocityRange[1] = max; }
	void			setScaleRange(float min, float max)                          { m_scaleRange[0]        = min; m_scaleRange[1]        = max; }
	void			setLifetimeRange(float min, float max)                       { m_lifetimeRange[0]     = min; m_lifetimeRange[1]     = max; }
	void			setColorRange(const glm::vec4& min, const glm::vec4& max)    { m_colorRange[0]        = min; m_colorRange[1]        = max; }

	void			emit(bool enable);
	bool			isEmitting();
	virtual void	update(float deltaTime) override;
	virtual void	draw() override;

protected:
	Emitter(const MaterialPtr& mat, bool autofade) : 
		m_material(mat), m_autofade(autofade), m_rateRange{0.0f}, m_velocityRange{ glm::vec3(0.0f) }, m_spinVelocityRange{ 0.0f },
		m_scaleRange{ 1.0f }, m_lifetimeRange{ 0.0f }, m_colorRange{ glm::vec4(1.0f) }, m_pendingParticles(0.0f) {}
	~Emitter() {}

	MaterialPtr m_material;
	bool        m_autofade;
	float       m_rateRange[2];
	glm::vec3   m_velocityRange[2];
	float       m_spinVelocityRange[2];
	float       m_scaleRange[2];
	float       m_lifetimeRange[2];
	glm::vec4   m_colorRange[2];

	std::vector<ParticlePtr> m_particles;
	float                    m_pendingParticles;
};