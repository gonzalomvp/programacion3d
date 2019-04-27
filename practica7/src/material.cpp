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

	// To render the skybox we remove the skybox cube transformations (Model Matrix) and the camara translation (last column and last row of the View Matrix, keeping only the upper-left 3x3)
	// This way the skybox will be always centered at the camera posisition
	shader->setMatrix(shader->getLocation("SkyboxMVP"), State::projectionMatrix * glm::mat4(glm::mat3(State::viewMatrix)));

	int useTexture = 0;
	int isCubeTexture = 0;
	if (m_texture) {
		useTexture = 1;
		if (m_texture->isCube()) {
			shader->setInt(shader->getLocation("texCubeSampler"), CUBE_TEX_LAYER);
			m_texture->bind(CUBE_TEX_LAYER);
			isCubeTexture = 1;
		}
		else {
			shader->setInt(shader->getLocation("texSampler"), BASE_TEX_LAYER);
			m_texture->bind(BASE_TEX_LAYER);
		}
	}

	if (m_normalTexture) {
		shader->setInt(shader->getLocation("texNormalsSampler"), NORM_TEX_LAYER);
		m_normalTexture->bind(NORM_TEX_LAYER);
	}

	shader->setInt(shader->getLocation("useTexture"), useTexture);
	shader->setInt(shader->getLocation("isCubeTexture"), isCubeTexture);
	shader->setVec4(shader->getLocation("materialColor"), m_color);
	shader->setInt(shader->getLocation("materialShininess"), m_shininess);

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
		shader->setVec3(shader->getLocation("ambientLight"), State::ambient);
		numLights = static_cast<int>(State::lights.size());
		for (size_t i = 0; i < State::lights.size(); ++i) {
			State::lights[i]->prepare(static_cast<int>(i), shader);
		}
	}
	shader->setInt(shader->getLocation("numLights"), numLights);
}