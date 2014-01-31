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

void Scene::Tick(int frame)
{
	for (auto it = PointLights.begin(); it != PointLights.end(); it++)
	{
		vec3 ref = vec3(0.0f, 0.0f, 10.0f);
		mat4 RotateMatrix = mat4(1.0f);
		RotateMatrix = glm::rotate(RotateMatrix, 2.0f, vec3(0.0f, 1.0f, 0.0f));
		(*it)->pos = ref + vec3(RotateMatrix * vec4((*it)->pos - ref, 1.0f));
	}

	for (auto it = Spheres.begin(); it != Spheres.end(); it++)
	{
		mat4 RotateMatrix = mat4(1.0f);
		RotateMatrix = glm::rotate(RotateMatrix, 0.5f, vec3(0.0f, 0.0f, 1.0f));
		(*it)->pos = vec3(RotateMatrix * vec4((*it)->pos, 1.0f));
	}
}

void Scene::AddSphere(vec3 pos, float r)
{
	Sphere* s = new Sphere(pos, r);
	Spheres.push_back(s);
}

size_t Scene::GetNumSpheres()
{
	return Spheres.size();
}

Sphere Scene::GetSphereAt(unsigned i)
{
	return *Spheres[i];
}

void Scene::AddPointLight(vec3 pos, vec4 color)
{
	PointLight* s = new PointLight(pos, color);
	PointLights.push_back(s);
}

size_t Scene::GetNumPointLights()
{
	return PointLights.size();
}

PointLight Scene::GetPointLightAt(unsigned i)
{
	return *PointLights[i];
}
