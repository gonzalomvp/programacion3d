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
	shader->setVec3(shader->getLocation("color"), glm::vec3(m_color.r, m_color.g, m_color.b));
	shader->setInt(shader->getLocation("shininess"), m_shininess);

	// Prepare Lights
	shader->setVec3(shader->getLocation("ambient"), State::ambient);
	shader->setInt(shader->getLocation("numLights"), static_cast<int>(State::lights.size()));
	for (size_t i = 0; i < State::lights.size(); ++i) {
		State::lights[i]->prepare(static_cast<int>(i), shader);
	}
}