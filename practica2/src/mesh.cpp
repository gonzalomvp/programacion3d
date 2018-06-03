#include "mesh.h"
#include "state.h"

void Mesh::addBuffer(const BufferPtr& buffer, const ShaderPtr& shader) {
	m_buffers.push_back(std::make_pair(buffer, shader));
}

void Mesh::draw() {
	for (size_t i = 0; i < m_buffers.size(); ++i) {
		BufferPtr buffer = m_buffers[i].first;
		ShaderPtr shader = m_buffers[i].second;
		if (!shader) {
			shader = State::defaultShader;
		}

		glm::mat4 MVP = State::projectionMatrix * State::viewMatrix * State::modelMatrix;

		shader->use();
		shader->setMatrix(shader->getLocation("MVP"), MVP);
		buffer->draw(*shader);
	}
}