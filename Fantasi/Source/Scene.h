#ifndef Scene_h__
#define Scene_h__
#include "Debugging/Macros.h"
#include <windows.h>
#include <vector>
#include <map>
#include "OpenGL/GL.h"
#include "Geometry/Object.h"
#include "Geometry/Sphere.h"
#include "Geometry/Triangle.h"
#include "Geometry/Metaball.h"
#include "Lighting/PointLight.h"
#include "Materials/BasicMaterial.h"

class Scene
{

public:

	enum ChildType
	{
		S_OBJECT,
		S_SPHERE,
		S_TRIANGLE,
		S_POINTLIGHT,
		S_BASICMATERIAL,
		S_METABALL,
		S_TORUS,

		// Special Elements
		S_COUNT,
		S_FIRST = S_OBJECT,
		S_LAST = S_TORUS
	};

private:

	typedef std::vector<void*> ChildVector_t;
	typedef std::map<ChildType, ChildVector_t> ChildMap_t;

	ChildMap_t Children;

	std::map<WPARAM, bool> KeyDownStatus;

public:

	int NumReflections;
	float AntiAlias;
	mat4 ViewMatrix;
	bool bExitSignalled;

	Scene();
	virtual ~Scene();
	void Tick(int frame);
	void HandleOnKeyDown(WPARAM w);
	void HandleOnKeyUp(WPARAM w);

	void AddChild(ChildType ct, void* child);
	size_t GetNumChildrenByType(ChildType ct);
	void* GetChildAt(ChildType ct, int index);
};

#endif // Scene_h__
