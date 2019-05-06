#include "state.h"

ShaderPtr              State::defaultShader;
ShaderPtr              State::overrideShader;
glm::mat4              State::projectionMatrix;
glm::mat4              State::viewMatrix;
glm::mat4              State::modelMatrix;
glm::mat4              State::depthBiasMatrix;
std::vector<LightPtr>  State::lights;
glm::vec3              State::ambient;
glm::vec3              State::eyePos;
bool                   State::shadows   = false;
bool                   State::animation = false;
std::vector<glm::mat4> State::animMatrices;