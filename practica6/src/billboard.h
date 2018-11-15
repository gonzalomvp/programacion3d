#pragma once

#include "entity.h"
#include "buffer.h"
#include "material.h"

class Billboard;
typedef std::shared_ptr<Billboard> BillboardPtr;

class Billboard : public Entity {
public:
	static BillboardPtr create(const MaterialPtr& mat) { return BillboardPtr(new Billboard(mat), [](Billboard* p) { delete p; }); }

	const MaterialPtr&  getMaterial() const            { return m_material; }
	MaterialPtr&        getMaterial()                  { return m_material; }
	const glm::vec2&	getSize() const                { return m_size;     }
	void			    setSize(const glm::vec2& size) { m_size = size;     }
	float			    getSpin() const                { return m_spin;     }
	void			    setSpin(float spin)            { m_spin = spin;     }

	virtual void draw() override;

private:
	Billboard(const MaterialPtr& mat);
	~Billboard() {}

	MaterialPtr m_material;
	glm::vec2   m_size;
	float       m_spin;
	BufferPtr   m_buffer;
};