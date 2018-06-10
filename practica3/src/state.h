#pragma once

#include "common.h"
#include "shader.h"

class State {
public:
	static ShaderPtr defaultShader;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 modelMatrix;

private:
	State();
	~State();
};
