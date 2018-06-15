#include "texture.h"

TexturePtr Texture::load(const char* filename) {
	// Load image with STB
	int w, h;
	
	stbi_set_flip_vertically_on_load(true);
	uint8_t* buffer = stbi_load(filename, &w, &h, nullptr, 4);
	if (!buffer) return nullptr;

	TexturePtr texture(new Texture(), [](Texture* p) { delete p; });
	texture->m_size = glm::ivec2(w, h);

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

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}