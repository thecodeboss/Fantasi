#ifndef Metaball_h__
#define Metaball_h__
#include "../OpenGL/GL.h"

struct Metaball
{
	vec3 pos;
	float strength;
	Metaball(vec3 p, float s) : pos(p), strength(s) {}
};

#endif // Metaball_h__
