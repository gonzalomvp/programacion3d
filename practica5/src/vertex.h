#pragma once
#include "common.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec2 tex;
	glm::vec3 normal;

	Vertex(const glm::vec3& _pos, const glm::vec2& _tex, const glm::vec3& _normal) : pos(_pos), tex(_tex), normal(_normal) {}
};