#include "billboard.h"

Billboard::Billboard(const MaterialPtr& mat, const glm::vec2& size, float spin) : m_material(mat), m_size(size), m_spin(spin), m_buffer(nullptr) {
	std::vector<Vertex> vertices;
	vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)));
	vertices.push_back(Vertex(glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)));
	vertices.push_back(Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)));
	vertices.push_back(Vertex(glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)));
	std::vector<uint16_t> indexes = {0, 1, 2, 1, 2, 3};
	m_buffer = Buffer::create(vertices, indexes);
	
	if (m_material && m_material->getTexture()) {
		m_size = m_material->getTexture()->getSize();
	}
}

void Billboard::draw() {
	// Matrix column-major
	glm::mat4 modelMatrix = glm::mat4(glm::vec4(State::viewMatrix[0].x, State::viewMatrix[1].x, State::viewMatrix[2].x, 0.0f),
                                      glm::vec4(State::viewMatrix[0].y, State::viewMatrix[1].y, State::viewMatrix[2].y, 0.0f),
                                      glm::vec4(State::viewMatrix[0].z, State::viewMatrix[1].z, State::viewMatrix[2].z, 0.0f),
                                      glm::vec4(m_position.x, m_position.y, m_position.z, 1.0f));

	modelMatrix = glm::scale(modelMatrix, glm::vec3(m_size.x, m_size.y, 1.0f) * m_scale);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(m_spin) , glm::vec3(0.0f, 0.0f, 1.0f));
	State::modelMatrix = modelMatrix;
	m_material->prepare();
	m_buffer->draw(*(m_material->getShader()));
}