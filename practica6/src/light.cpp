#include "light.h"
#include "state.h"

LightPtr Light::create() {
	if (State::lights.size() < MAX_LIGHTS) {
		return LightPtr(new Light(), [](Light* p) { delete p; });
	}
	else {
		return nullptr;
	}
	
}

void Light::prepare(int index, ShaderPtr& shader) const {
	glm::vec4 lightVector(0.0f);

	switch (m_type) {
	case Light::DIRECTIONAL:
		lightVector = glm::vec4(m_position, 0.0f);
		break;
	case Light::POINT:
		lightVector = glm::vec4(m_position, 1.0f);
		break;
	default:
		break;
	}

	lightVector = State::viewMatrix * lightVector;
	std::string indexStr = "[" + std::to_string(index) + "]";
	shader->setVec4(shader->getLocation((std::string("lightVectors") + indexStr).c_str()), lightVector);
	shader->setVec3(shader->getLocation((std::string("lightColors") + indexStr).c_str()), m_color);
	shader->setFloat(shader->getLocation((std::string("lightAttenuations") + indexStr).c_str()), m_linearAttenuation);
}