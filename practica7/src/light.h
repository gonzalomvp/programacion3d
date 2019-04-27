#pragma once
#include "entity.h"
#include "shader.h"

class Light;
typedef std::shared_ptr<Light> LightPtr;

class Light : public Entity {
public:
	enum Type {
		DIRECTIONAL,
		POINT
	};

	static LightPtr create(Type type, glm::vec3 color = glm::vec3(1.0f), float linearAttenuation = 0.0f) { return LightPtr(new Light(type, color, linearAttenuation), [](Light* p) { delete p; }); }

	Type			 getType             () const                 { return m_type;              }
	void			 setType             (Type type)              { m_type = type;              }
	const glm::vec3& getColor            () const                 { return m_color;             }
	void			 setColor            (const glm::vec3& color) { m_color = color;            }
	float			 getLinearAttenuation() const                 { return m_linearAttenuation; }
	void			 setLinearAttenuation(float att)              { m_linearAttenuation = att;  }

	void			 prepare(int index, ShaderPtr& shader) const;

private:
	Light(Type type, glm::vec3 color, float linearAttenuation) : m_type(type), m_color(color), m_linearAttenuation(linearAttenuation) {}
	~Light() {}

	Type      m_type;
	glm::vec3 m_color;
	float     m_linearAttenuation;
};