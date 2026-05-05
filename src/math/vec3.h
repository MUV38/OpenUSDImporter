#pragma once

class vec3
{
public:
	vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	vec3(const float* v) : x(v[0]), y(v[1]), z(v[2]) {}
	vec3(float v) : x(v), y(v), z(v) {}

	float x, y, z;
};