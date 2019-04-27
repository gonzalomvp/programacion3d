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
	texture->bind(BASE_TEX_LAYER);

	// Set params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Load image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free STB buffer
	stbi_image_free(buffer);

	return texture;
}

TexturePtr Texture::load(const char* left, const char* right, const char* front, const char* back, const char* top, const char* bottom) {
	std::vector<const char*> faces = { right, left, top, bottom, front, back };

	TexturePtr texture(new Texture(), [](Texture* p) { delete p; });
	texture->m_isCube = true;

	// Generate openGL texture
	glGenTextures(1, &(texture->m_id));

	// Bind texture
	texture->bind(CUBE_TEX_LAYER);

	// Set params
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(false);

	for (unsigned int i = 0; i < faces.size(); ++i) {
		int w, h;
		uint8_t* buffer = stbi_load(faces[i], &w, &h, nullptr, 4);
		if (buffer) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			stbi_image_free(buffer);
		}
	}

	//Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	return texture;
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}

void Texture::bind(size_t layer) const {
	glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(layer));

	if (m_isCube) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}