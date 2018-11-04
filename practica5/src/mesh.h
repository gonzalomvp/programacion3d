#pragma once

#include "buffer.h"
#include "material.h"
#include "shader.h"

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class Mesh {
public:
	static MeshPtr      create() { return MeshPtr(new Mesh(), [](Mesh* p) { delete p; }); }
	static MeshPtr      load  (const char* filename, const ShaderPtr& shader = nullptr);

	void                addBuffer    (const BufferPtr& buffer, const ShaderPtr& shader = nullptr);
	void                addBuffer    (const BufferPtr& buffer, const MaterialPtr& material);
					    
	size_t              getNumBuffers()             const { return m_buffers.size();        }
	const BufferPtr&    getBuffer    (size_t index) const { return m_buffers[index].first ; }
	BufferPtr&          getBuffer    (size_t index)       { return m_buffers[index].first ; }
	const MaterialPtr&  getMaterial  (size_t index) const { return m_buffers[index].second; }
	MaterialPtr&        getMaterial  (size_t index)       { return m_buffers[index].second; }
	void                draw         ();

private:
	Mesh()  {}
	~Mesh() {}

	std::vector<std::pair<BufferPtr, MaterialPtr>> m_buffers;
};