#pragma once

#include "buffer.h"
#include "shader.h"
#include <vector>

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class Mesh {
public:
	void             addBuffer    (const BufferPtr& buffer, const ShaderPtr& shader = nullptr);
	size_t           getNumBuffers()             const { return m_buffers.size(); }
	const BufferPtr& getBuffer    (size_t index) const { return m_buffers[index].first ;}
	BufferPtr&       getBuffer    (size_t index)       { return m_buffers[index].first ;}
	void             draw         ();

private:
	std::vector<std::pair<BufferPtr, ShaderPtr>> m_buffers;
};