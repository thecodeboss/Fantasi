#ifndef Sphere_h__
#define Sphere_h__
#include "../OpenGL/GL.h"

struct Sphere
{
	vec3 pos;
	float r;
	Sphere(vec3 p, float R) : pos(p), r(R) {}
};

#endif // Sphere_h__
