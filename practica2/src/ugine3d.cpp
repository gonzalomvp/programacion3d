#include "ugine3d.h"

bool init() {
	bool ret = false;

	// initialize opengl extensions and opengl states
	if (glewInit() == GLEW_OK) {
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);

		// create default shader
		std::shared_ptr<Shader> shader = Shader::createShader(readString("data/vertex.glsl"), readString("data/fragment.glsl"));
		if (shader) {
			State::defaultShader = shader;
			ret = true;
		}
	}

	return ret;
}