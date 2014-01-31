#ifndef Scene_h__
#define Scene_h__
#include <vector>
#include "Geometry/Sphere.h"

class Scene
{

	std::vector<Sphere*> Spheres;

public:

	Scene();
	virtual ~Scene();
	void AddSphere(float x, float y, float z, float r);
	void Tick(int frame);
	size_t GetNumSpheres();
	Sphere GetSphereAt( unsigned i );
};

#endif // Scene_h__
