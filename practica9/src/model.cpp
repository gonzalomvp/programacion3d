#include "model.h"
#include "state.h"

void Model::calculateAnimMatrices(float frame, const std::vector<Bone>& bones) {
	m_animMatrices.clear();
	for (size_t i = 0; i < bones.size(); ++i) {
		int parentBoneIndex = bones[i].getParentIndex();
		if (parentBoneIndex > -1) {
			m_animMatrices.push_back(m_animMatrices[parentBoneIndex] * bones[i].calculateAnimMatrix(frame));
		}
		else {
			m_animMatrices.push_back(bones[i].calculateAnimMatrix(frame));
		}
	}
	for (size_t i = 0; i < m_animMatrices.size(); ++i) {
		m_animMatrices[i] = m_animMatrices[i] * bones[i].getInvPoseMatrix();
	}
}

void Model::update(float deltaTime) {
	Entity::update(deltaTime);

	if (m_animate) {
		m_currentFrame += m_fps * deltaTime;
		if (m_currentFrame > m_mesh->getLastFrame()) {
			m_currentFrame = 0.0f;
		}
		else if (m_currentFrame < 0.0f) {
			m_currentFrame = m_mesh->getLastFrame();
		}

		calculateAnimMatrices(m_currentFrame, m_mesh->getBones());
	}
}

void Model::draw() {
	Entity::draw();

	glm::mat4 modelMatrix = glm::translate(glm::mat4(), m_position);
	modelMatrix = glm::rotate(modelMatrix, glm::angle(m_quat), glm::axis(m_quat));
	modelMatrix = glm::scale(modelMatrix, m_scale);
	State::modelMatrix = modelMatrix;

	State::animation = m_animate;
	if (m_animate) {
		State::animMatrices = &m_animMatrices;
	}
	m_mesh->draw();

	State::animMatrices = nullptr;
}