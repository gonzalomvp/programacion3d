#include "state.h"

std::shared_ptr<Shader> State::defaultShader;
glm::mat4               State::projectionMatrix;
glm::mat4               State::viewMatrix;
glm::mat4               State::modelMatrix;
std::vector<LightPtr>   State::lights;
glm::vec3               State::ambient;
glm::vec3               State::eyePos;