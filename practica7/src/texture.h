#pragma once

#include "common.h"
#include "../lib/stb/stb_image.h"

class Texture;

typedef std::shared_ptr<Texture> TexturePtr;

class Texture {
public:
	static TexturePtr load   (const char* filename);
	static TexturePtr load   (const char* left, const char* right, const char* front, const char* back, const char* top, const char* bottom);
	uint32_t          getId  () const { return m_id;                        }
	const glm::ivec2& getSize() const { return m_size;                      }
	void              bind   (size_t layer = 0) const;
	bool              isCube () const { return m_isCube;                    }

private:
	Texture() : m_id(0), m_size(0), m_isCube(false) {}
	~Texture();

	uint32_t   m_id;
	glm::ivec2 m_size;
	bool       m_isCube;
};