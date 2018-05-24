#pragma once

struct Vertex {
	float x, y, z;
	float r, g, b;

	Vertex(float x, float y, float z, float r, float g, float b)
	:
		x(x), y(y), z(z),
		r(r), g(g), b(b)
	{}
};