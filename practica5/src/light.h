#pragma once

#include "entity.h"

class Light;
class ShaderPtr;
typedef std::shared_ptr<Light> LightPtr;

class Light : public Entity {
public:
	enum Type {
		DIRECTIONAL,
		POINT
	};

	static LightPtr create() { return LightPtr(new Light(), [](Light* p) { delete p; }); }

	Type			 getType             () const                 { return m_type;              }
	void			 setType             (Type type)              { m_type = type;              }
	const glm::vec3& getColor            () const                 { return m_color;             }
	void			 setColor            (const glm::vec3& color) { m_color = color;            }
	float			 getLinearAttenuation() const                 { return m_linearAttenuation; }
	void			 setLinearAttenuation(float att)              { m_linearAttenuation = att;  }

	void			 prepare(int index, ShaderPtr& shader) const;

private:
	Light(): m_type(DIRECTIONAL), m_color(), m_linearAttenuation(1.0f) {}
	~Light() {}

	Type      m_type;
	glm::vec3 m_color;
	float     m_linearAttenuation;
};