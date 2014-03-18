#version 430
#define FAR_CLIP 100000.0f
#define EPSILON 0.0001f
#define SCREEN_X 1024.0f
#define SCREEN_Y 768.0f
#define INV_3 0.333333f
#define SQRT_3 1.732051f
#define FLOAT_MAX 100000000.0f
#define PI 3.1415927f
#define PI_2 1.5707963f

// Enums
#define OBJ_SPHERE 0
#define OBJ_TRIANGLE 1
#define OBJ_METABALL 2

#define MAT_BASIC 0
#define MAT_PHYSICAL 1

layout (local_size_x = 16, local_size_y = 16) in;

uniform uint NumObjects;
uniform uint NumSpheres;
uniform uint NumTriangles;
uniform uint NumPointLights;
uniform uint NumBasicMaterials;
uniform uint NumMetaballs;

uniform mat4 ViewMatrix;
uniform uint NumReflections;
uniform writeonly image2D destTex;

struct Object
{
	int type; // 0 = sphere, 1 = triangle, 2 = metaball, 3 = torus
	int index;
	int materialtype; // 0 = basic material, 1 = physical material
	int materialindex;
};

struct Sphere
{
	vec3 pos;
	float r;
};

struct Metaball
{
	vec3 pos;
	float strength;
};

struct Triangle
{
	vec3 v0;
	vec3 v1;
	vec3 v2;
	vec3 n;
};

struct Ray
{
	vec3 origin;
	vec3 dir;
};

struct PointLight
{
	vec3 pos;
	vec4 color;
};

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
};

layout(std430, binding = 1) buffer ObjectsBuffer { Object objects[]; };
layout(std430, binding = 2) buffer SpheresBuffer { Sphere spheres[]; };
layout(std430, binding = 3) buffer TrianglesBuffer { Triangle triangles[]; };
layout(std430, binding = 4) buffer PointLightsBuffer { PointLight pointlights[]; };
layout(std430, binding = 5) buffer BasicMaterialsBuffer { BasicMaterial basicmaterials[]; };
layout(std430, binding = 6) buffer MetaballsBuffer { Metaball metaballs[]; };

uniform float AntiAlias;

float FresnelDieletricConductorApprox(float Eta, float Etak, float CosTheta)
{
	// Modified from Sebastion Lagarde

	float  CosTheta2 = CosTheta * CosTheta;
	float TwoEtaCosTheta = 2.0f * Eta * CosTheta;

	float t0 = Eta * Eta + Etak * Etak;
	float t1 = t0 * CosTheta2;
	float Rs = (t0 - TwoEtaCosTheta + CosTheta2) / (t0 + TwoEtaCosTheta + CosTheta2);
	float Rp = (t1 - TwoEtaCosTheta + 1.0f) / (t1 + TwoEtaCosTheta + 1.0f);

	return 0.5f* (Rp + Rs);
}

float FresnelDielectricDielectric(float Eta, float CosTheta)
{
	float SinTheta2 = 1 - CosTheta * CosTheta;

	float t0 = sqrt(1 - (SinTheta2 / (Eta * Eta)));
	float t1 = Eta * t0;
	float t2 = Eta * CosTheta;

	float rs = (CosTheta - t1) / (CosTheta + t1);
	float rp = (t0 - t2) / (t0 + t2);

	return 0.5 * (rs * rs + rp * rp);
}

vec3 normal(int index, vec3 Intersection)
{
	int k = objects[index].index;
	vec3 n;
	switch (objects[index].type)
	{
		case OBJ_SPHERE:
			n = normalize(Intersection - spheres[k].pos);
			break;
		case OBJ_TRIANGLE:
			// Normal = (v1 - v0) x (v2 - v0)
			// Assuming Triangle order is clockwise
			n = triangles[k].n;
			break;
		case OBJ_METABALL:
			n = vec3(0.0f, 0.0f, 0.0f);
			{
				float p = 0.0f;
				for (int i = 0; i < NumMetaballs; i++)
				{
					vec3 v = metaballs[i].pos - Intersection;
					float R = length(v)/metaballs[k].strength;
					if (R >= 0.71f)
					{
						continue;
					}
					R *= R;
					n += 2.0f*(2.0f*R-1.0f)*v;
				}
			}
			n = normalize(n);
			break;
		default:
			n = vec3(1.0f, 0.0f, 0.0f);
			break;
	}
	return n;
}

float FindRaySphereIntersection(Ray r, Sphere s)
{
	vec3 oc = r.origin - s.pos;
	float p = -dot(r.dir, oc);
	float d = p*p - dot(oc,oc) + s.r*s.r;
	if (d < 0) return FAR_CLIP;
	else if (d == 0) return (p < 0) ? FAR_CLIP : p;
	else
	{
		float t1 = sqrt(d);
		float t2 = p + t1;
		t1 = p - t1;

		// If the ray started inside the sphere, we don't render it
		if ( t1 < -EPSILON || t2 < -EPSILON ) return FAR_CLIP;

		if ( min(t1, t2) < EPSILON) return FAR_CLIP;
		else return min(t1, t2);
	}
}

float FindRayTriangleIntersection(Ray r, Triangle p)
{
	// Based on Moller-Trumbore paper
	vec3 E1 = p.v1 - p.v0;
	vec3 E2 = p.v2 - p.v0;
	vec3 T = r.origin - p.v0;
	vec3 D = r.dir;
	vec3 P = cross(D, E2);
	float f = dot(P, E1);

	float u = dot(P, T) / f;
	if (u < -EPSILON || u > 1.0f) return FAR_CLIP;

	vec3 Q = cross(T, E1);
	float v = dot(Q, D) / f;
	if (v < -EPSILON || u+v > 1.0f) return FAR_CLIP;

	return dot(Q, E2) / f;
}

float FindRayMetaballIntersection(Ray r, Metaball m)
{
	float Threshold = 0.1f;

	Sphere s;
	s.pos = m.pos;
	s.r = m.strength*0.707f;
	float t = FindRaySphereIntersection(r, s);
	if (t > EPSILON && t < FAR_CLIP)
	{
		// Start ray-marching
		vec3 Intersection = r.origin + t*r.dir;
		vec3 Increment = r.dir*m.strength*0.71f/100.0f;
		bool bFoundIntersection = false;
		for (int i = 0; i < 100; i++)
		{
			float p = 0.0f;
			for (int j = 0; j < NumMetaballs; j++)
			{
				float R = length(metaballs[j].pos - Intersection)/m.strength;
				if (R >= 0.707f)
				{
					continue;
				}
				R *= R;
				p += R*R - R + 0.25f;
			}
			if (p > Threshold)
			{
				bFoundIntersection = true;
				t += i*m.strength*0.707f/100.0f;
				break;
			}
			Intersection += Increment;
		}
		if (bFoundIntersection)
		{
			return t;
		}
	}
	return FAR_CLIP;
}

vec4 ComputeBasicMaterialColor(int ObjectIndex, vec3 Viewer, vec3 Intersection)
{
	BasicMaterial b = basicmaterials[objects[ObjectIndex].materialindex];
	vec3 n = normal(ObjectIndex, Intersection);
	vec4 OverallColor = b.AmbientColor + b.EmissiveColor;
	for (int i=0; i<NumPointLights; i++)
	{
		vec3 L = pointlights[i].pos - Intersection;
		Ray r = Ray(Intersection, normalize(L));
		bool IsVisible = true;
		for (int j=0; j<NumObjects; j++)
		{
			float t;
			switch (objects[j].type)
			{
				case OBJ_SPHERE:
					t = FindRaySphereIntersection(r, spheres[objects[j].index]);
					break;
				case OBJ_TRIANGLE:
					t = FindRayTriangleIntersection(r, triangles[objects[j].index]);
					break;
				case OBJ_METABALL:
					t = FindRayMetaballIntersection(r, metaballs[objects[j].index]);
					break;
			}

			if (t > EPSILON && t < FAR_CLIP && length(L) >= t)
			{
				IsVisible = false;
				break;
			}
		}
		if (IsVisible)
		{
			// Compute Diffuse
			float NL = dot(n, normalize(L));
			OverallColor += max(NL, 0.0f) * b.DiffuseColor * pointlights[i].color;

			// Compute Specular
			vec3 V = Intersection - Viewer;
			vec3 H = normalize(L + V / length(L + V));
			float NH = dot(n, H);
			float facing = (NL > EPSILON) ? 1.0f : 0.0f;

			OverallColor += facing * pow(max(NH, 0.0f), b.Shininess) * b.SpecularColor * pointlights[i].color;
		}
	}
	return OverallColor;
}

void main()
{
	ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	imageStore(destTex, storePos, vec4(0.0, 0.0, 0.0, 1.0));

	//float AntiAlias = 1.0f;

	vec4 AliasedColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for (float u=0.0f; u<1.0f; u+=1.0f/AntiAlias) for (float v=0.0f; v<1.0f; v+=1.0f/AntiAlias)
	{
		vec3 RayDir = vec3(storePos.x + u - SCREEN_X/2.0f, storePos.y + v - SCREEN_Y/2.0f, SCREEN_X);

		Ray r = Ray(vec3(0.0, 0.0, 0.0), normalize(RayDir));
		r.origin = (ViewMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f)).xyz;
		vec4 OverallColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		float reflectance = 1.0f;
		for (int i=0; i < NumReflections+1; i++)
		{
			float nearest_t = FAR_CLIP;
			float nearest_t2 = FAR_CLIP;
			int nearest_index = 0;
			int nearest_index2 = -1;
			for (int j=0; j<NumObjects; j++)
			{
				float t = FAR_CLIP;
				switch (objects[j].type)
				{
					case OBJ_SPHERE:
						t = FindRaySphereIntersection(r, spheres[objects[j].index]);
						break;
					case OBJ_TRIANGLE:
						t = FindRayTriangleIntersection(r, triangles[objects[j].index]);
						break;
					case OBJ_METABALL:
						t = FindRayMetaballIntersection(r, metaballs[objects[j].index]);
						break;
				}
				if (t > EPSILON && t < FAR_CLIP && abs(nearest_t - t) < EPSILON)
				{
					nearest_index2 = nearest_index;
					nearest_t2 = nearest_t;
				}
				if (t < nearest_t+EPSILON && t > EPSILON && t < FAR_CLIP)
				{
					nearest_t = t;
					nearest_index = j;
				}
			}

			if (nearest_t < FAR_CLIP)
			{
				vec3 Intersection;
				vec4 c1;
				if (nearest_index2 >= 0)
				{
					Intersection = r.origin + min(nearest_t, nearest_t2)*r.dir;
					c1 = ComputeBasicMaterialColor(nearest_index, r.origin, Intersection);
					vec4 c2 = ComputeBasicMaterialColor(nearest_index2, r.origin, Intersection);
					c1 = mix(c1, c2, 0.5f*abs(nearest_t-nearest_t2)/EPSILON);
				}
				else
				{
					Intersection = r.origin + nearest_t*r.dir;
					c1 = ComputeBasicMaterialColor(nearest_index, r.origin, Intersection);
				}
				OverallColor += reflectance * c1;

				if (i == NumReflections) break;

				vec3 n = normal(nearest_index, Intersection);
			
				BasicMaterial b = basicmaterials[objects[nearest_index].materialindex];
				float NI = dot(n, normalize(r.origin - Intersection));
				float Fresnel;
				if (b.Dielectric > EPSILON) Fresnel = 0.3f*FresnelDielectricDielectric(b.RefractiveIndex, max(NI, 0.0f));
				else Fresnel = FresnelDieletricConductorApprox(b.RefractiveIndex, b.AbsorptionCoefficient, max(NI, 0.0f));
				reflectance *= 0.5f*Fresnel;

				r.dir = normalize(reflect(Intersection - r.origin, n));
				r.origin = Intersection;
			}
			else break;
		}
		AliasedColor += OverallColor;
	}
	imageStore(destTex, storePos, AliasedColor/(AntiAlias*AntiAlias));
}
