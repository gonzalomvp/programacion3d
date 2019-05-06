#pragma once
#include "common.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec4 boneIndices;
	glm::vec4 boneWeights;

	Vertex(const glm::vec3& _pos, const glm::vec2& _tex = glm::vec2(0.0f), const glm::vec3& _normal = glm::vec3(0.0f), const glm::vec3& _tangent = glm::vec3(0.0f), const glm::vec4& _boneIndices = glm::vec4(-1.0f), const glm::vec4& _boneWeights = glm::vec4(0.0f))
	: pos(_pos)
	, tex(_tex)
	, normal(_normal)
	, tangent(_tangent)
	, boneIndices(_boneIndices)
	, boneWeights(_boneWeights)
	{}
};