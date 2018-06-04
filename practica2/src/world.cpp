#include "world.h"
#include <algorithm>

void World::addEntity(const EntityPtr& entity) {
	m_entities.push_back(entity);
	CameraPtr camera = std::dynamic_pointer_cast<Camera>(entity);
	if (camera) {
		m_cameras.push_back(camera);
	}
}

// Revisar
void World::removeEntity(const EntityPtr& entity) {
	m_entities.erase(
		std::remove(m_entities.begin(), m_entities.end(), entity),
		m_entities.end()
	);
	CameraPtr camera = std::dynamic_pointer_cast<Camera>(entity);
	if (camera) {
		m_cameras.erase(
			std::remove(m_cameras.begin(), m_cameras.end(), camera),
			m_cameras.end()
		);
	}
}

void World::update(float deltaTime) {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		m_entities[i]->update(deltaTime);
	}
}

void World::draw() {
	for (size_t i = 0; i < m_cameras.size(); ++i) {
		m_cameras[i]->prepare();
		for (size_t j = 0; j < m_entities.size(); ++j) {
			m_entities[j]->draw();
		}
	}
}