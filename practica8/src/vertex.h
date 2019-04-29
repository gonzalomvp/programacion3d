#pragma once
#include "common.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 normal;
	glm::vec3 tangent;

	Vertex(const glm::vec3& _pos, const glm::vec2& _tex, const glm::vec3& _normal, const glm::vec3& _tangent) : pos(_pos), tex(_tex), normal(_normal), tangent(_tangent) {}
};