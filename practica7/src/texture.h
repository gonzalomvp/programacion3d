#pragma once

#include "common.h"
#include "../lib/stb/stb_image.h"

#define BASE_TEX_LAYER 0
#define CUBE_TEX_LAYER 1
#define NORM_TEX_LAYER 2
#define REFL_TEX_LAYER 3
#define REFR_TEX_LAYER 4

class Texture;

typedef std::shared_ptr<Texture> TexturePtr;

class Texture {
public:
	static TexturePtr load   (const char* filename);
	static TexturePtr load   (const char* left, const char* right, const char* front, const char* back, const char* top, const char* bottom);
	uint32_t          getId  () const { return m_id;                        }
	const glm::ivec2& getSize() const { return m_size;                      }
	bool              isCube () const { return m_isCube;                    }
	
	void              bind(size_t layer = 0) const;

private:
	Texture() : m_id(0), m_size(0), m_isCube(false) {}
	~Texture();

	uint32_t   m_id;
	glm::ivec2 m_size;
	bool       m_isCube;
};