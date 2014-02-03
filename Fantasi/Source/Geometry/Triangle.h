#ifndef Triangle_h__
#define Triangle_h__
#include "../OpenGL/GL.h"

struct Triangle
{
	vec3 v0;
	float g1;
	vec3 v1;
	float g2;
	vec3 v2;
	float g3;
	Triangle(vec3 a, vec3 b, vec3 c) : v0(a), v1(b), v2(c) {}
};

#endif // Triangle_h__
