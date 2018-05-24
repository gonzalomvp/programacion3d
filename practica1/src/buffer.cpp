#include "buffer.h"
#include "shader.h"

BufferPtr Buffer::createBuffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indexes) {
	std::shared_ptr<Buffer> buffer(new Buffer(vertices, indexes), [](Buffer* p) { delete p; });

	//todo
	/*if (shader->m_Id == 0)
	{
		shader = nullptr;
	}*/
	return buffer;
}


Buffer::Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indexes) {
	m_vertexSize = vertices.size();

	glGenBuffers(1, &m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indexes.size(), indexes.data(), GL_STATIC_DRAW);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}



void Buffer::draw(const Shader& shader) const {
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

	shader.setupAttribs();

	glDrawArrays(GL_TRIANGLES, 0, m_vertexSize);
}