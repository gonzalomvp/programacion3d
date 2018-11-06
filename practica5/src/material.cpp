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

	//Lights

	shader->setInt(shader->getLocation("numLights"), State::lights.size());
	glm::vec4 lightVectorTemp(0.0f, 1.0f, 0.0f, 0.0f);
	lightVectorTemp = ModelView * lightVectorTemp;
	shader->setVec4(shader->getLocation("L[0]"), lightVectorTemp);
	lightVectorTemp = glm::vec4(0.0f, 0.0f, -5.0f, 1.0f);
	lightVectorTemp = ModelView * lightVectorTemp;
	shader->setVec4(shader->getLocation("L[1]"), lightVectorTemp);


	for (size_t i = 0; i < State::lights.size(); ++i) {
		State::lights[i]->prepare(static_cast<int>(i), shader);
	}
}