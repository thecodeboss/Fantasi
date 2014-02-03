#include "Scene.h"
#include "IO/Keyboard.h"

Scene::Scene() : NumReflections(1)
{
	for (int it = ChildType::S_FIRST; it < ChildType::S_COUNT; it++)
	{
		Children[(ChildType)it] = ChildVector_t();
	}
}

Scene::~Scene()
{
	// Cleanup children in the scene
	for (int it = ChildType::S_FIRST; it < ChildType::S_COUNT; it++)
	{
		for (auto it2 = Children[(ChildType)it].begin(); it2 != Children[(ChildType)it].end(); it2++ )
		{
			delete *it2;
		}
	}
}

void Scene::Tick(int frame)
{
	//for (auto it = Children[Scene::S_POINTLIGHT].begin(); it != Children[Scene::S_POINTLIGHT].end(); it++)
	//{
	//	vec3 ref = vec3(0.0f, 0.0f, 10.0f);
	//	mat4 RotateMatrix = mat4(1.0f);
	//	RotateMatrix = glm::rotate(RotateMatrix, 2.0f, vec3(0.0f, 1.0f, 0.0f));
	//	PointLight* p = (PointLight*)(*it);
	//	p->pos = ref + vec3(RotateMatrix * vec4(p->pos - ref, 1.0f));
	//}

	vec3 ref = vec3(0.0f, 0.0f, 50.0f);
	mat4 RotateMatrix = mat4(1.0f);
	RotateMatrix = glm::rotate(RotateMatrix, 0.5f, vec3(0.0f, 1.0f, 0.0f));
	Sphere* s = (Sphere*)Children[Scene::S_SPHERE][1];
	s->pos = ref + vec3(RotateMatrix * vec4(s->pos - ref, 1.0f));

	for (auto it = KeyDownStatus.begin(); it != KeyDownStatus.end(); it++)
	{
		if (it->second)
		{
			switch (it->first)
			{
			case KEY_W: case KEY_UP:
				if (KeyDownStatus.find(KEY_SHIFT) != KeyDownStatus.end() && KeyDownStatus[KEY_SHIFT])
				{
					ViewMatrix = glm::translate(ViewMatrix, vec3(0.0f, 1.0f, 0.0f));
				}
				else
				{
					ViewMatrix = glm::translate(ViewMatrix, vec3(0.0f, 0.0f, 1.0f));
				}
				break;
			case KEY_A: case KEY_LEFT:
				ViewMatrix = glm::translate(ViewMatrix, vec3(-1.0f, 0.0f, 0.0f));
				break;
			case KEY_S: case KEY_DOWN:
				if (KeyDownStatus.find(KEY_SHIFT) != KeyDownStatus.end() && KeyDownStatus[KEY_SHIFT])
				{
					ViewMatrix = glm::translate(ViewMatrix, vec3(0.0f, -1.0f, 0.0f));
				}
				else
				{
					ViewMatrix = glm::translate(ViewMatrix, vec3(0.0f, 0.0f, -1.0f));
				}
				break;
			case KEY_D: case KEY_RIGHT:
				ViewMatrix = glm::translate(ViewMatrix, vec3(1.0f, 0.0f, 0.0f));
				break;
			default:
				break;
			}
		}
	}
}

void Scene::HandleOnKeyDown(WPARAM w)
{
	KeyDownStatus[w] = true;
}

void Scene::HandleOnKeyUp(WPARAM w)
{
	KeyDownStatus[w] = false;
}

void Scene::AddChild( ChildType ct, void* child )
{
	Children[ct].push_back(child);
}

size_t Scene::GetNumChildrenByType( ChildType ct )
{
	return Children[ct].size();
}

void* Scene::GetChildAt( ChildType ct, int index )
{
	return Children[ct].at(index);
}
