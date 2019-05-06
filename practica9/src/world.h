#pragma once

#include "entity.h"
#include "camera.h"
#include "light.h"
#include "state.h"

class World;
typedef std::shared_ptr<World> WorldPtr;

class World {
public:
	static WorldPtr  create();

	void             addEntity     (const EntityPtr& entity);
	void             removeEntity  (const EntityPtr& entity);
	size_t           getNumEntities() const             { return m_entities.size(); }
	const EntityPtr& getEntity     (size_t index) const { return m_entities[index]; }
	EntityPtr&       getEntity     (size_t index)       { return m_entities[index]; }

	const glm::vec3& getAmbient    () const                   { return m_ambient;    }
	void			 setAmbient    (const glm::vec3& ambient) { m_ambient = ambient; }

	void             setShadows    (bool enable) { State::shadows = enable; }
	void             setDepthOrtho (float left, float right, float bottom, float top, float near, float far);

	void             update        (float deltaTime);
	void             draw          ();

private:
	World() : m_ambient(0.2f), m_shadowsShader(nullptr), m_shadowsCamera(nullptr), m_shadowsCameraFar(0.0f) {}
	~World() {}

	std::vector<EntityPtr> m_entities;
	std::vector<CameraPtr> m_cameras;
	std::vector<LightPtr>  m_lights;
	glm::vec3              m_ambient;
	ShaderPtr              m_shadowsShader;
	CameraPtr              m_shadowsCamera;
	float                  m_shadowsCameraFar;
};