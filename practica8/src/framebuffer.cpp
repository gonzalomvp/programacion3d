#include "framebuffer.h"

FramebufferPtr Framebuffer::create(const TexturePtr& colorTex, const TexturePtr& depthTex) {
	FramebufferPtr framebuffer(new Framebuffer(colorTex, depthTex), [](Framebuffer* p) { delete p; });

	glGenFramebuffers(1, &framebuffer->m_id);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_id);

	if (framebuffer->m_colorTex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebuffer->getColorTexture()->getId(), 0);
	}
	else {
		glDrawBuffer(GL_NONE);
	}

	if (framebuffer->m_depthTex) {
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, framebuffer->m_depthTex->getId(), 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return framebuffer;
}

void Framebuffer::bindScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &m_id);
}

void Framebuffer::bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}