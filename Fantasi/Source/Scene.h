#ifndef Scene_h__
#define Scene_h__
#include <vector>
#include "OpenGL/GL.h"
#include "Geometry/Sphere.h"
#include "Lighting/PointLight.h"

class Scene
{

	std::vector<Sphere*> Spheres;
	std::vector<PointLight*> PointLights;

public:

	Scene();
	virtual ~Scene();
	void Tick(int frame);

	void AddSphere(vec3 pos, float r);
	size_t GetNumSpheres();
	Sphere GetSphereAt(unsigned i);

	void AddPointLight(vec3 pos, vec4 color);
	size_t GetNumPointLights();
	PointLight GetPointLightAt(unsigned i);
};

#endif // Scene_h__
