#pragma once

#include "entity.h"
#include "mesh.h"

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Model : public Entity {
public:
	static ModelPtr create(const MeshPtr& mesh) { return ModelPtr(new Model(mesh), [](Model* p) { delete p; }); }

	void  animate        (bool animate) { m_animate = animate;    }
	int   getFps         () const       { return m_fps;           }
	void  setFps         (int fps)      { m_fps = fps;            }
	float getCurrentFrame() const       { return m_currentFrame;  }
	void  setCurrentFrame(float frame)  { m_currentFrame = frame; }

	void calculateAnimMatrices(float frame, const std::vector<Bone>& bones);

	virtual void update(float deltaTime) override;
	virtual void draw() override;

private:
	Model(const MeshPtr& mesh) : m_mesh(mesh), m_animate(false), m_fps(0), m_currentFrame(0.0f) {}
	~Model() {}

	MeshPtr m_mesh;

	bool                   m_animate;
	int                    m_fps;
	float                  m_currentFrame;
	std::vector<glm::mat4> m_animMatrices;
};