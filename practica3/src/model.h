#pragma once

#include "entity.h"
#include "mesh.h"

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Model : public Entity {
public:
	static ModelPtr create(const MeshPtr& mesh) { return ModelPtr(new Model(mesh), [](Model* p) { delete p; }); }

	virtual void draw() override;

private:
	Model(const MeshPtr& mesh) : m_mesh(mesh) {}
	~Model() {}

	MeshPtr m_mesh;
};