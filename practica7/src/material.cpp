#include "material.h"

void Material::prepare() {
	ShaderPtr shader = getShader();
	shader->use();

	shader->setMatrix(shader->getLocation("Model"), State::modelMatrix);

	glm::mat4 ModelView = State::viewMatrix * State::modelMatrix;
	shader->setMatrix(shader->getLocation("ModelView"), ModelView);
	
	glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));
	shader->setMatrix(shader->getLocation("Normal"), Normal);

	glm::mat4 MVP = State::projectionMatrix * ModelView;
	shader->setMatrix(shader->getLocation("MVP"), MVP);

	shader->setVec3(shader->getLocation("eyePos"), State::eyePos);

	shader->setInt(shader->getLocation("normalTexSampler"), 1);

	int useTexture = 0;

	if (m_texture) {
		if (m_texture->isCube()) {
			shader->setInt(shader->getLocation("cubeSampler"), 2);
			m_texture->bind(GL_TEXTURE2);
		}
		else {
			shader->setInt(shader->getLocation("texSampler"), 0);
			m_texture->bind(GL_TEXTURE0);
			useTexture = 1;
		}
	}
	if (m_normalTexture) {
		m_normalTexture->bind(GL_TEXTURE1);
	}

	if (m_reflectionTexture) {
		m_reflectionTexture->bind(GL_TEXTURE2);
		shader->setInt(shader->getLocation("cubeSampler"), 2);
		useTexture = 1;
	}

	if (m_refractionTexture) {
		m_refractionTexture->bind(GL_TEXTURE2);
		shader->setInt(shader->getLocation("cubeSampler"), 2);
		useTexture = 1;
	}

	shader->setInt(shader->getLocation("useTexture"), useTexture);
	shader->setVec4(shader->getLocation("color"), m_color);
	shader->setInt(shader->getLocation("shininess"), m_shininess);

	switch (m_blendMode) {
		case Material::ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case Material::ADD:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case Material::MUL:
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			break;
	}

	if (m_culling) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (m_depthWrite) {
		glDepthMask(GL_TRUE);
	}
	else {
		glDepthMask(GL_FALSE);
	}

	// Prepare Lights
	int numLights = 0;
	if (m_lighting) {
		shader->setVec3(shader->getLocation("ambient"), State::ambient);
		numLights = static_cast<int>(State::lights.size());
		for (size_t i = 0; i < State::lights.size(); ++i) {
			State::lights[i]->prepare(static_cast<int>(i), shader);
		}
	}
	shader->setInt(shader->getLocation("numLights"), numLights);
}