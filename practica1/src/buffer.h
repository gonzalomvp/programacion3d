#pragma once

#include "common.h"
#include <vector>

class Buffer;
class Shader;

typedef std::shared_ptr<Buffer> BufferPtr;

class Buffer {
public:
	static BufferPtr createBuffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indexes);
	void draw(const Shader& shader) const;

private:
	Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indexes);
	~Buffer();
	uint32_t m_vertexBuffer;
	uint32_t m_indexBuffer;

	uint32_t m_vertexSize;
};