#pragma once

#include "common.h"
#include "../lib/stb/stb_image.h"

class Texture;

typedef std::shared_ptr<Texture> TexturePtr;

class Texture {
public:
	static TexturePtr load   (const char* filename);
	uint32_t          getId  () const { return m_id;                        }
	const glm::ivec2& getSize() const { return m_size;                      }
	void              bind   () const { glBindTexture(GL_TEXTURE_2D, m_id); }

private:
	Texture() : m_id(0) {}
	~Texture();

	uint32_t   m_id;
	glm::ivec2 m_size;
};