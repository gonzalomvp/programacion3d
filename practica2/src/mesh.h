#pragma once

#include "buffer.h"
#include "shader.h"

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class Mesh {
public:
	static MeshPtr   createMesh() { return MeshPtr(new Mesh(), [](Mesh* p) { delete p; }); }

	void             addBuffer    (const BufferPtr& buffer, const ShaderPtr& shader = nullptr);
	size_t           getNumBuffers()             const { return m_buffers.size(); }
	const BufferPtr& getBuffer    (size_t index) const { return m_buffers[index].first ;}
	BufferPtr&       getBuffer    (size_t index)       { return m_buffers[index].first ;}
	void             draw         ();

private:
	Mesh()  {}
	~Mesh() {}

	std::vector<std::pair<BufferPtr, ShaderPtr>> m_buffers;
};