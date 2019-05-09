#pragma once

#include "common.h"
#include <functional>

class Entity;
typedef std::shared_ptr<Entity> EntityPtr;
typedef std::function<void(Entity&, float)> EntityCallback;
typedef std::shared_ptr<void> EntityDataPtr;

class Entity {
public:
	static EntityPtr      create() { return EntityPtr(new Entity(), [](Entity* p) { delete p; }); }

	const glm::vec3&      getPosition   () const                         { return m_position;                                       }
	void                  setPosition   (const glm::vec3& pos)           { m_position = pos;                                        }
	const glm::vec3&      getEuler      () const                         { return m_euler;                                          }
	const void            setEuler      (const glm::vec3& rot)           { m_euler = rot; m_quat = glm::quat(glm::radians(rot));    }
	const glm::quat&      getQuaternion () const                         { return m_quat;                                           }
	const void            setQuaternion (const glm::quat& q)             { m_quat = q; m_euler = glm::degrees(glm::eulerAngles(q)); }
	const glm::vec3&      getScale      () const                         { return m_scale;                                          }
	void                  setScale      (const glm::vec3& scale)         { m_scale = scale;                                         }
	const EntityCallback& getCallback   () const                         { return m_callback;                                       }
	void                  setCallback   (const EntityCallback& callback) { m_callback = callback;                                   }
	EntityDataPtr&	      getUserData   ()                               { return m_data;                                           }
	void                  setUserData   (const EntityDataPtr& data)      { m_data = data;                                           }

	void         move   (const glm::vec3& vec);
	virtual void update (float deltaTime);
	virtual void draw   ();

protected:
	Entity() : m_position(), m_euler(), m_quat(), m_scale(1.0f), m_callback(nullptr) {}
	virtual ~Entity() {}

	glm::vec3      m_position;
	glm::vec3      m_euler;
	glm::quat      m_quat;
	glm::vec3      m_scale;
	EntityCallback m_callback;
	EntityDataPtr  m_data;
};