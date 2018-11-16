#include "material.h"

void Material::prepare() {
	ShaderPtr shader = getShader();
	shader->use();

	glm::mat4 ModelView = State::viewMatrix * State::modelMatrix;
	shader->setMatrix(shader->getLocation("ModelView"), ModelView);
	
	glm::mat4 Normal = glm::transpose(glm::inverse(ModelView));
	shader->setMatrix(shader->getLocation("Normal"), Normal);

	glm::mat4 MVP = State::projectionMatrix * ModelView;
	shader->setMatrix(shader->getLocation("MVP"), MVP);

	shader->setInt(shader->getLocation("texSampler"), 0);

	int useTexture = 0;
	if (m_texture) {
		m_texture->bind();
		useTexture = 1;
	}

	shader->setInt(shader->getLocation("useTexture"), useTexture);
	shader->setVec4(shader->getLocation("color"), glm::vec4(m_color.r, m_color.g, m_color.b, m_color.a));
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
	shader->setVec3(shader->getLocation("ambient"), State::ambient);
	if (m_lighting) {
		numLights = static_cast<int>(State::lights.size());
		for (size_t i = 0; i < State::lights.size(); ++i) {
			State::lights[i]->prepare(static_cast<int>(i), shader);
		}
	}
	shader->setInt(shader->getLocation("numLights"), numLights);
}