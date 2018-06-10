#include "material.h"

void Material::prepare() {
	ShaderPtr shader = getShader();
	shader->use();

	glm::mat4 MVP = State::projectionMatrix * State::viewMatrix * State::modelMatrix;
	shader->setMatrix(shader->getLocation("MVP"), MVP);
	shader->setInt(shader->getLocation("texSampler"), 0);

	if (m_texture) {
		m_texture->bind();
		shader->setInt(shader->getLocation("useTexture"), 1);
	}
	else {
		shader->setInt(shader->getLocation("useTexture"), 0);
	}
}