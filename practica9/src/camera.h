#pragma once

#include "entity.h"
#include "framebuffer.h"

class Camera;
typedef std::shared_ptr<Camera> CameraPtr;

class Camera : public Entity {
public:
	static CameraPtr  create() { return CameraPtr(new Camera(), [](Camera* p) { delete p; }); }

	const glm::mat4&      getProjection() const                             { return m_projection;         }
	void                  setProjection(const glm::mat4& proj)              { m_projection = proj;         }
	const glm::ivec4&     getViewport  () const                             { return m_viewport;           }
	void                  setViewport  (const glm::ivec4& vp)               { m_viewport = vp;             }
	const glm::vec3&      getClearColor() const                             { return m_clearColor;         }
	void                  setClearColor(const glm::vec3& color)             { m_clearColor = color;        }
	const FramebufferPtr& getFramebuffer() const                            { return m_framebuffer;        }
	void                  setFramebuffer(const FramebufferPtr& framebuffer) { m_framebuffer = framebuffer; }

	void                  prepare();

private:
	Camera() : m_projection(), m_viewport(), m_clearColor() {}
	~Camera() {}

	glm::mat4      m_projection;
	glm::ivec4     m_viewport;
	glm::vec3      m_clearColor;
	FramebufferPtr m_framebuffer;
};