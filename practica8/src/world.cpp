#include "world.h"

#include "framebuffer.h"
#include "state.h"
#include "utils.h"
#include <algorithm>

#define MAX_LIGHTS 8
#define SHADOWS_TEX_W 1024
#define SHADOWS_TEX_H 1024

WorldPtr World::create() {
	WorldPtr world = WorldPtr(new World(), [](World* p) { delete p; });

	TexturePtr depthTex = Texture::create(SHADOWS_TEX_W, SHADOWS_TEX_H, true);
	FramebufferPtr framebuffer = Framebuffer::create(nullptr, depthTex);

	world->m_shadowsCamera = Camera::create();
	world->m_shadowsCamera->setViewport(glm::ivec4(0, 0, SHADOWS_TEX_W, SHADOWS_TEX_H));
	world->m_shadowsCamera->setFramebuffer(framebuffer);

	world->m_shadowsShader = Shader::create(readString("data/shadows_vertex.glsl"), readString("data/shadows_fragment.glsl"));

	return world;
}

void World::addEntity(const EntityPtr& entity) {
	m_entities.push_back(entity);
	CameraPtr camera = std::dynamic_pointer_cast<Camera>(entity);
	if (camera) {
		m_cameras.push_back(camera);
	}
	else {
		LightPtr light = std::dynamic_pointer_cast<Light>(entity);
		if (light && m_lights.size() < MAX_LIGHTS) {
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

void World::setDepthOrtho(float left, float right, float bottom, float top, float near, float far) {
	m_shadowsCamera->setProjection(glm::ortho(left, right, bottom, top, near, far));
	m_shadowsCameraFar = far;
}

void World::update(float deltaTime) {
	for (size_t i = 0; i < m_entities.size(); ++i) {
		m_entities[i]->update(deltaTime);
	}
}

void World::draw() {
	State::ambient = m_ambient;
	State::lights = m_lights;

	// Depth pass
	if (State::shadows) {
		LightPtr directionalLight = nullptr;
		for (size_t i = 0; i < m_lights.size(); ++i) {
			directionalLight = m_lights[i];
			if (directionalLight->getType() == Light::DIRECTIONAL) {
				break;
			}
		}

		if (directionalLight) {
			State::overrideShader = m_shadowsShader;

			glm::vec3 lightPos = glm::normalize(directionalLight->getPosition());
			m_shadowsCamera->setPosition(lightPos * m_shadowsCameraFar * 0.5f);
			glm::vec3 lightDir = lightPos * -1.0f;
			m_shadowsCamera->setEuler(glm::vec3(glm::degrees(asinf(lightDir.y)), glm::degrees(atan2f(-lightDir.x, -lightDir.z)), 0.0f));

			m_shadowsCamera->prepare();

			for (size_t i = 0; i < m_entities.size(); ++i) {
				m_entities[i]->draw();
			}

			glm::mat4 biasMatrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.5f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
			State::depthBiasMatrix = biasMatrix * State::projectionMatrix * State::viewMatrix;

			m_shadowsCamera->getFramebuffer()->getDepthTexture()->bind(SHDW_TEX_LAYER);

			State::overrideShader = nullptr;
		}
	}

	// Render pass
	for (size_t i = 0; i < m_cameras.size(); ++i) {
		m_cameras[i]->prepare();
		for (size_t j = 0; j < m_entities.size(); ++j) {
			m_entities[j]->draw();
		}
	}
}