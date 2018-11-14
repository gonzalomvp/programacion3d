#include "world.h"
#include "state.h"
#include <algorithm>

void World::addEntity(const EntityPtr& entity) {
	m_entities.push_back(entity);
	CameraPtr camera = std::dynamic_pointer_cast<Camera>(entity);
	if (camera) {
		m_cameras.push_back(camera);
	}
	else {
		LightPtr light = std::dynamic_pointer_cast<Light>(entity);
		if (light) {
			m_lights.push_back(light);
		}
	}
}

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
	else {
		LightPtr light = std::dynamic_pointer_cast<Light>(entity);
		if (light) {
			m_lights.erase(
				std::remove(m_lights.begin(), m_lights.end(), light),
				m_lights.end()
			);
		}
	}
}

void World::update(float deltaTime) {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		m_entities[i]->update(deltaTime);
	}
}

void World::draw() {
	State::ambient = m_ambient;
	State::lights.clear();
	for (size_t i = 0; i < m_lights.size(); ++i) {
		State::lights.push_back(m_lights[i]);
	}

	for (size_t i = 0; i < m_cameras.size(); ++i) {
		m_cameras[i]->prepare();
		for (size_t j = 0; j < m_entities.size(); ++j) {
			m_entities[j]->draw();
		}
	}
}