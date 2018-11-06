#include "light.h"
#include "state.h"

void Light::prepare(int index, ShaderPtr& shader) const {
	glm::vec4 lightVectorTemp(0.0f, 1.0f, 0.0f, 0.0f);
	lightVectorTemp = State::viewMatrix * lightVectorTemp;
	shader->setVec4(shader->getLocation("L[0]"), lightVectorTemp);
}