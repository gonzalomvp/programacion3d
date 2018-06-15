#pragma once
#include "common.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex;

	Vertex(const glm::vec3& _pos, const glm::vec3& _color, const glm::vec2& _tex) : pos(_pos), color(_color), tex(_tex) {}
};