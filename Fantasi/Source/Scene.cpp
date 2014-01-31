#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	// Cleanup spheres in scene
	for (auto it = Spheres.begin(); it != Spheres.end(); it++)
	{
		delete (*it);
	}
}

void Scene::AddSphere( float x, float y, float z, float r )
{
	Sphere* s = new Sphere(x, y, z, r);
	Spheres.push_back(s);
}

void Scene::Tick(int frame)
{
	// Called every frame to update the scene
}

size_t Scene::GetNumSpheres()
{
	return Spheres.size();
}

Sphere Scene::GetSphereAt( unsigned i )
{
	return *Spheres[i];
}
