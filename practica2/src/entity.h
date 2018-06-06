#pragma once

#include "common.h"

class Entity;
typedef std::shared_ptr<Entity> EntityPtr;
typedef void(*CallbackFunc) (Entity&, float);

class Entity {
public:
	//static EntityPtr createEntity() { return EntityPtr(new Entity(), [](Entity* p) { delete p; }); }

	const glm::vec3& getPosition() const                 { return m_position;     }
	void             setPosition(const glm::vec3& pos)   { m_position = pos;      }
	// with quaternions
	const glm::quat& getRotation() const                 { return m_rotation;     }
	void		     setRotation(const glm::quat& rot)   { m_rotation = rot;      }
	// with euler angles
	//const glm::vec3& getRotation() const               { return m_rotation;     }
	//void           setRotation(const glm::vec3& rot)   { m_rotation = rot;      }
	const glm::vec3& getScale   () const                 { return m_scale;        }
	void             setScale   (const glm::vec3& scale) { m_scale = scale;       }
	CallbackFunc     getCallback() const                 { return m_callback;     }
	void setCallback(const CallbackFunc callback)        { m_callback = callback; }

	void             move       (const glm::vec3& vec);
	virtual void     update     (float deltaTime);
	virtual void     draw       () {}

protected:
	Entity() : m_position(), m_rotation(), m_scale(1.0f) {}
	virtual ~Entity() {}

	glm::vec3    m_position;
	// with quaternions
	glm::quat    m_rotation;
	// with euler angles
	//glm::vec3  m_rotation;
	glm::vec3    m_scale;
	CallbackFunc m_callback;
};