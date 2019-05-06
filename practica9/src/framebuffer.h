#pragma once

#include "common.h"
#include "texture.h"

class Framebuffer;

typedef std::shared_ptr<Framebuffer> FramebufferPtr;

class Framebuffer {
public:
	static FramebufferPtr create(const TexturePtr& colorTex, const TexturePtr& depthTex);
	static void bindScreen();

	const TexturePtr& getColorTexture() const { return m_colorTex; }
	const TexturePtr& getDepthTexture() const { return m_depthTex; }

	void bind() const;
private:
	Framebuffer(const TexturePtr& colorTex, const TexturePtr& depthTex) : m_colorTex(colorTex), m_depthTex(depthTex) {}
	~Framebuffer();

	uint32_t m_id;
	const TexturePtr m_colorTex;
	const TexturePtr m_depthTex;
};