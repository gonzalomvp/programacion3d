#include "texture.h"

TexturePtr Texture::load(const char* filename) {
	// Load image with STB
	int w, h;
	
	stbi_set_flip_vertically_on_load(true);
	uint8_t* buffer = stbi_load(filename, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;

	TexturePtr texture(new Texture(), [](Texture* p) { delete p; });
	texture->m_size = glm::ivec2(w, h);
	texture->m_isCube = false;

	// Generate openGL texture
	glGenTextures(1, &(texture->m_id));

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture->m_id);

	// Set params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free STB buffer
	stbi_image_free(buffer);

	return texture;
}

TexturePtr Texture::load(const char* left, const char* right, const char* front, const char* back, const char* top, const char* bottom) {
	
	const char* filenames[6] = { left, right, front, back, top, bottom };
	GLenum glParams[6] = { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X
						, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Y
						, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_Z };

	TexturePtr texture(new Texture(), [](Texture* p) { delete p; });
	texture->m_isCube = true;

	// Generate openGL texture
	glGenTextures(1, &(texture->m_id));

	// Bind texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_id);

	// Set params
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int w, h;
	stbi_set_flip_vertically_on_load(false);
	uint8_t* buffer;

	buffer = stbi_load(left, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	//stbi_image_free(buffer);

	buffer = stbi_load(right, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	//stbi_image_free(buffer);

	buffer = stbi_load(bottom, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	////stbi_image_free(buffer);

	buffer = stbi_load(top, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	////stbi_image_free(buffer);

	buffer = stbi_load(back, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	//stbi_image_free(buffer);

	buffer = stbi_load(front, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	//stbi_image_free(buffer);

	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	//Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//stbi_image_free(buffer);

	return texture;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}

void Texture::bind(size_t layer) const {
	glActiveTexture(static_cast<GLenum>(layer));

	if (m_isCube) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}