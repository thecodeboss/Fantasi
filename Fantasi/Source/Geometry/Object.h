#ifndef Object_h__
#define Object_h__

struct Object
{
	int type; // 0 = sphere, 1 = triangle
	int index;
	int materialtype; // 0 = basic material, 1 = physical material
	int materialindex;
	Object(int t, int i, int mt, int mi) : type(t), index(i), materialtype(mt), materialindex(mi) {}
};

#endif // Object_h__
