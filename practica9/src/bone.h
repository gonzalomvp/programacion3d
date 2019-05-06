#pragma once

#include "common.h"

class Bone;
typedef std::shared_ptr<Bone> BonePtr;

class Bone {
public:
	static BonePtr create(const char* name, int parentIndex) { return BonePtr(new Bone(name, parentIndex), [](Bone* p) { delete p; }); }
	~Bone() {}

	const std::string& getName()        const { return m_name;        }
	int                getParentIndex() const { return m_parentIndex; }

	const glm::mat4& getInvPoseMatrix() const             { return m_invPoseMatrix; }
	void             setInvPoseMatrix(const glm::mat4& m) { m_invPoseMatrix = m;    }

	void addPosition(uint16_t frame, const glm::vec3& position) { m_positions.push_back(std::make_pair(frame, position)); }
	void addRotation(uint16_t frame, const glm::quat& rotation) { m_rotations.push_back(std::make_pair(frame, rotation)); }
	void addScale   (uint16_t frame, const glm::vec3& scale)    { m_scales.push_back(std::make_pair(frame, scale));       }

	glm::mat4 calculateAnimMatrix(float frame) const;
	glm::vec3 calculatePosition  (float frame) const;
	glm::quat calculateRotation  (float frame) const;
	glm::vec3 calculateScale     (float frame) const;

private:
	Bone(const std::string& name, int parentIndex) : m_name(name), m_parentIndex(parentIndex) {}

	const std::string m_name;
	int               m_parentIndex;
	glm::mat4         m_invPoseMatrix;

	std::vector<std::pair<uint16_t, const glm::vec3>> m_positions;
	std::vector<std::pair<uint16_t, const glm::quat>> m_rotations;
	std::vector<std::pair<uint16_t, const glm::vec3>> m_scales;

};