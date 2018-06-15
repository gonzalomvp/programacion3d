#pragma once

#include "entity.h"
#include "camera.h"

class World;
typedef std::shared_ptr<World> WorldPtr;

class World {
public:
	static WorldPtr  create() { return WorldPtr(new World(), [](World* p) { delete p; }); }

	void             addEntity     (const EntityPtr& entity);
	void             removeEntity  (const EntityPtr& entity);
	size_t           getNumEntities() const             { return m_entities.size(); }
	const EntityPtr& getEntity     (size_t index) const { return m_entities[index]; }
	EntityPtr&       getEntity     (size_t index)       { return m_entities[index]; }
	void             update        (float deltaTime);
	void             draw          ();

private:
	World()  {}
	~World() {}

	std::vector<EntityPtr> m_entities;
	std::vector<CameraPtr> m_cameras;
};