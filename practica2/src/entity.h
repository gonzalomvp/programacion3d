#pragma once

#include "common.h"

class Entity;
typedef std::shared_ptr<Entity> EntityPtr;

class Entity {
public:
	Entity() : m_position(glm::vec3()), m_rotation(glm::vec3()), m_scale(glm::vec3(1.0f)) {}
	virtual ~Entity() {}

	const glm::vec3& getPosition() const                 { return m_position; }
	void             setPosition(const glm::vec3& pos)   { m_position = pos;  }
	const glm::vec3& getRotation() const                 { return m_rotation; }
	const void       setRotation(const glm::vec3& rot)   { m_rotation = rot;  }
	const glm::vec3& getScale   () const                 { return m_scale;    }
	void             setScale   (const glm::vec3& scale) { m_scale = scale;   }
	void             move       (const glm::vec3& vec);
	virtual void     update     (float deltaTime) {}
	virtual void     draw       ()                {}

protected:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};