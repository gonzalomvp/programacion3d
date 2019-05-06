#include "bone.h"

glm::mat4 Bone::calculateAnimMatrix(float frame) const {
	glm::mat4 animMatrix = glm::translate(glm::mat4(), calculatePosition(frame));
	glm::quat rotation = calculateRotation(frame);
	animMatrix = glm::rotate(animMatrix, glm::angle(rotation), glm::axis(rotation));
	animMatrix = glm::scale(animMatrix, calculateScale(frame));
	return animMatrix;
}

glm::vec3 Bone::calculatePosition(float frame) const {
	glm::vec3 position(0.0f);

	for (size_t i = 0; i < m_positions.size(); ++i) {
		float nextFrame = static_cast<float>(m_positions[i].first);
		if (nextFrame == frame) {
			position = m_positions[i].second;
			break;
		}
		if (nextFrame > frame) {
			uint16_t previousFrame = m_positions[i - 1].first;
			uint16_t nextFrame = m_positions[i].first;
			position = glm::mix(m_positions[i - 1].second, m_positions[i].second, (frame - previousFrame) / (nextFrame - previousFrame));
			break;
		}
		position = m_positions[i].second;
	}

	return position;
}

glm::quat Bone::calculateRotation(float frame) const {
	glm::quat rotation;

	for (size_t i = 0; i < m_rotations.size(); ++i) {
		float nextFrame = static_cast<float>(m_rotations[i].first);
		if (nextFrame == frame) {
			rotation = m_rotations[i].second;
			break;
		}
		if (nextFrame > frame) {
			uint16_t previousFrame = m_rotations[i - 1].first;
			uint16_t nextFrame = m_rotations[i].first;
			rotation = glm::slerp(m_rotations[i - 1].second, m_rotations[i].second, (frame - previousFrame) / (nextFrame - previousFrame));
			break;
		}
		rotation = m_rotations[i].second;
	}

	return rotation;
}

glm::vec3 Bone::calculateScale(float frame) const {
	glm::vec3 scale(1.0f);

	for (size_t i = 0; i < m_scales.size(); ++i) {
		float nextFrame = static_cast<float>(m_scales[i].first);
		if (nextFrame == frame) {
			scale = m_scales[i].second;
			break;
		}
		if (nextFrame > frame) {
			uint16_t previousFrame = m_scales[i - 1].first;
			uint16_t nextFrame = m_scales[i].first;
			scale = glm::mix(m_scales[i - 1].second, m_scales[i].second, (frame - previousFrame) / (nextFrame - previousFrame));
			break;
		}
		scale = m_scales[i].second;
	}
	
	return scale;
}