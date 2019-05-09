#include "bone.h"

#include <algorithm>

glm::mat4 Bone::calculateAnimMatrix(float frame) const {
	glm::mat4 animMatrix = glm::translate(glm::mat4(), calculatePosition(frame));
	glm::quat rotation = calculateRotation(frame);
	animMatrix = glm::rotate(animMatrix, glm::angle(rotation), glm::axis(rotation));
	animMatrix = glm::scale(animMatrix, calculateScale(frame));
	return animMatrix;
}

glm::vec3 Bone::calculatePosition(float frame) const {
	glm::vec3 position(0.0f);

	if (m_positions.size() > 0) {
		position = m_positions[0].second;
		for (size_t i = 1; i < m_positions.size(); ++i) {
			float leftFrame = static_cast<float>(m_positions[i - 1].first);
			float rightFrame = static_cast<float>(m_positions[i].first);
			glm::vec3 leftFramePos = m_positions[i - 1].second;
			glm::vec3 rightFramePos = m_positions[i].second;

			if (frame <= rightFrame) {
				position = glm::mix(leftFramePos, rightFramePos, (std::max(frame, leftFrame) - leftFrame) / (rightFrame - leftFrame));
				break;
			}
			else {
				position = rightFramePos;
			}
		}
	}

	return position;
}

glm::quat Bone::calculateRotation(float frame) const {
	glm::quat rotation;

	if (m_rotations.size() > 0) {
		rotation = m_rotations[0].second;
		for (size_t i = 1; i < m_rotations.size(); ++i) {
			float leftFrame = static_cast<float>(m_rotations[i - 1].first);
			float rightFrame = static_cast<float>(m_rotations[i].first);
			glm::quat leftFrameRot = m_rotations[i - 1].second;
			glm::quat rightFrameRot = m_rotations[i].second;

			if (frame <= rightFrame) {
				rotation = glm::slerp(leftFrameRot, rightFrameRot, (std::max(frame, leftFrame) - leftFrame) / (rightFrame - leftFrame));
				break;
			}
			else {
				rotation = rightFrameRot;
			}
		}
	}

	return rotation;
}

glm::vec3 Bone::calculateScale(float frame) const {
	glm::vec3 scale(1.0f);

	if (m_scales.size() > 0) {
		scale = m_scales[0].second;
		for (size_t i = 1; i < m_scales.size(); ++i) {
			float leftFrame = static_cast<float>(m_scales[i - 1].first);
			float rightFrame = static_cast<float>(m_scales[i].first);
			glm::vec3 leftFrameScl = m_scales[i - 1].second;
			glm::vec3 rightFrameScl = m_scales[i].second;

			if (frame <= rightFrame) {
				scale = glm::mix(leftFrameScl, rightFrameScl, (std::max(frame, leftFrame) - leftFrame) / (rightFrame - leftFrame));
				break;
			}
			else {
				scale = rightFrameScl;
			}
		}
	}
	
	return scale;
}