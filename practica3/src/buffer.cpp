#include "buffer.h"
#include "shader.h"

BufferPtr Buffer::create(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indexes) {
	BufferPtr buffer(new Buffer(vertices, indexes), [](Buffer* p) { delete p; });
	return buffer;
}

Buffer::Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indexes) : m_vertexBuffer(0), m_indexBuffer(0), m_indexesCount(0) {
	m_indexesCount = static_cast<uint32_t>(indexes.size());

	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indexes.size(), indexes.data(), GL_STATIC_DRAW);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}

void Buffer::draw(const Shader& shader) const {
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	shader.setupAttribs();

	glDrawElements(GL_TRIANGLES, m_indexesCount, GL_UNSIGNED_SHORT, nullptr);
}