#ifndef BasicMaterial_h__
#define BasicMaterial_h__
#include "../OpenGL/GL.h"

struct BasicMaterial
{
	vec4 EmissiveColor;
	vec4 AmbientColor;
	vec4 DiffuseColor;
	vec4 SpecularColor;
	float Shininess;
	float RefractiveIndex;
	float AbsorptionCoefficient;
	float Dielectric;

	BasicMaterial(vec4 e, vec4 a, vec4 d, vec4 s, float S, float R, float K, float D)
		: EmissiveColor(e), AmbientColor(a), DiffuseColor(d), SpecularColor(s), Shininess(S), RefractiveIndex(R), AbsorptionCoefficient(K), Dielectric(D) {}
};

#endif // BasicMaterial_h__
