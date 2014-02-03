#ifndef PointLight_h__
#define PointLight_h__
#include "../OpenGL/GL.h"

struct PointLight
{
	vec3 pos;
	float garbage; // OpenGL likes multiples of 4
	vec4 color;
	PointLight(vec3 p, vec4 c) : pos(p), garbage(1.0f), color(c) {}
};

#endif // PointLight_h__
