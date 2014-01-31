#ifndef PointLight_h__
#define PointLight_h__
#include "../OpenGL/GL.h"

struct PointLight
{
	vec3 pos;
	vec4 color;
	PointLight(vec3 p, vec4 c) : pos(p), color(c) {}
};

#endif // PointLight_h__
