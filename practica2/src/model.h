#pragma once

#include "entity.h"
#include "mesh.h"

class Model : public Entity {
public:
	Model(const MeshPtr& mesh) : m_mesh(mesh) {}
	virtual void draw() override;

private:
	MeshPtr m_mesh;
};