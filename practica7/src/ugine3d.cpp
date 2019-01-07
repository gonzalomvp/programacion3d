#include "ugine3d.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/stb_image.h"

bool init() {
	bool ret = false;

	// initialize opengl extensions and opengl states
	if (glewInit() == GLEW_OK) {
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		// create default shader
		ShaderPtr shader = Shader::create(readString("data/vertex.glsl"), readString("data/fragment.glsl"));
		if (shader) {
			State::defaultShader = shader;
			ret = true;
		}
	}

	return ret;
}