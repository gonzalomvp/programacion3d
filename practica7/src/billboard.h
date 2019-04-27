#pragma once

#include "entity.h"
#include "buffer.h"
#include "material.h"

class Billboard;
typedef std::shared_ptr<Billboard> BillboardPtr;

class Billboard : public Entity {
public:
	static BillboardPtr create(const MaterialPtr& mat, const glm::vec2& size = glm::vec2(1.0f), float spin = 0.0f) { return BillboardPtr(new Billboard(mat, size, spin), [](Billboard* p) { delete p; }); }

	const MaterialPtr&  getMaterial() const            { return m_material; }
	MaterialPtr&        getMaterial()                  { return m_material; }
	const glm::vec2&	getSize() const                { return m_size;     }
	void			    setSize(const glm::vec2& size) { m_size = size;     }
	float			    getSpin() const                { return m_spin;     }
	void			    setSpin(float spin)            { m_spin = spin;     }

	virtual void draw() override;

protected:
	Billboard(const MaterialPtr& mat, const glm::vec2& size = glm::vec2(1.0f), float spin = 0.0f);
	~Billboard() {}

	MaterialPtr m_material;
	glm::vec2   m_size;
	float       m_spin;
	BufferPtr   m_buffer;
};