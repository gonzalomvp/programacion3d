#pragma once

struct Vertex {
	float x, y;
	float r, g, b;

	Vertex(float x, float y, float r, float g, float b)
	:
		x(x), y(y),
		r(r), g(g), b(b)
	{}
};