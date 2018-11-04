#include "material.h"

void Material::prepare() {
	ShaderPtr shader = getShader();
	shader->use();

	glm::mat4 MVP = State::projectionMatrix * State::viewMatrix * State::modelMatrix;
	shader->setMatrix(shader->getLocation("MVP"), MVP);
	shader->setInt(shader->getLocation("texSampler"), 0);

	int useTexture = 0;
	if (m_texture) {
		m_texture->bind();
		useTexture = 1;
	}

	shader->setInt(shader->getLocation("useTexture"), useTexture);
}