#include "light.h"
#include "state.h"

void Light::prepare(int index, ShaderPtr& shader) const {
	glm::vec4 light(0.0f);

	switch (m_type) {
		case Light::DIRECTIONAL:
			light = glm::vec4(m_position, 0.0f);
			break;
		case Light::POINT:
			light = glm::vec4(m_position, 1.0f);
			break;
		default:
			break;
	}

	// Convert light to view space
	light = State::viewMatrix * light;

	std::string indexStr = "[" + std::to_string(index) + "]";
	shader->setVec4(shader->getLocation((std::string("lights") + indexStr).c_str()), light);
	shader->setVec3(shader->getLocation((std::string("lightsColors") + indexStr).c_str()), m_color);
	shader->setFloat(shader->getLocation((std::string("lightsAttenuations") + indexStr).c_str()), m_linearAttenuation);
}