#pragma once

#include "common.h"
#include "shader.h"
#include "light.h"

class State {
public:
	static ShaderPtr               defaultShader;
	static ShaderPtr               overrideShader;
	static glm::mat4               projectionMatrix;
	static glm::mat4               viewMatrix;
	static glm::mat4               modelMatrix;
	static glm::mat4               depthBiasMatrix;
	static std::vector<LightPtr>   lights;
	static glm::vec3               ambient;
	static glm::vec3               eyePos;
	static bool                    shadows;
	static bool                    animation;
	static std::vector<glm::mat4>* animMatrices;

private:
	State() {}
	~State() {}
};
