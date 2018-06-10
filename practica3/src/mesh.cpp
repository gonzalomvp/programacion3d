#include "mesh.h"
#include "state.h"

void Mesh::addBuffer(const BufferPtr& buffer, const ShaderPtr& shader) {
	addBuffer(buffer, Material::create(nullptr, shader));
}

void Mesh::addBuffer(const BufferPtr& buffer, const MaterialPtr& material) {
	m_buffers.push_back(std::make_pair(buffer, material));
}

void Mesh::draw() {
	for (size_t i = 0; i < m_buffers.size(); ++i) {
		BufferPtr buffer = m_buffers[i].first;
		MaterialPtr material = m_buffers[i].second;

		material->prepare();
		
		buffer->draw(*(material->getShader()));
	}
}