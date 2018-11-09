#include "Vector.h"
#define num_Sph_Max 20
#define SHADOW_EPS 0.02f
#define PI 3.14159265
#define EPSILON 0.000001
#include <random>
#include <cassert>

#define MAX_LIGHTS 256

float getRndFloat();


class Camera
{
public:
	Vector eye;
	Vector look_at;
	Vector up;

	float fov;
};

class Material
{
public:
	enum {MIRROIR,LAMBERT,TRANSPARANT,PHONG,GLOSSY};
	Vector kd, ks;
	float kr;
	float theta_cone;
	int type;
	int checker = 0;
};

class IntersectInfo
{
public:
	Vector pt, n;
	float u, v;
	float t;
	Material* mat;
	IntersectInfo()
	{
		t = -1;
	};
};

class Ray
{
public:
	Vector orig, dir;
	Ray(Vector oo, Vector dd);
	Ray() {}
};

class Object
{
public:
	int id;
	Material* mat;

	virtual bool intersect(Ray ray, float tmin, float tmax, IntersectInfo* result) = 0;
};

class Sphere:public Object
{
public:
	Sphere();
	bool intersect(Ray ray, float tmin, float tmax, IntersectInfo* result);

	Vector O;
	float R;
};

class Mesh :public Object
{
public:
	class FaceElement
	{
	public:
		int vid, nid, tid;
	};
	Vector* vertices;
	Vector* normals;
	Vector* uvs;
	FaceElement* faces;
	int nv, nt, nn, nf;

	void load_from_file(char* filename);
	bool intersect(Ray ray, float tmin, float tmax, IntersectInfo* result);
	bool intersect_tri(int fid, Ray ray, float tmin, float tmax, IntersectInfo* result);
};

class Scene;

class Lumiere
{
public:
	Vector I;
	virtual Vector L(Scene* scene, IntersectInfo inter, Ray incident) = 0;
};

class LumierePonct :public Lumiere
{
public:
	Vector Pos;
	Vector L(Scene* scene, IntersectInfo inter, Ray incident);
};

class Scene
{
public:

	Object* objects[MAX_LIGHTS];
	Lumiere* lights[MAX_LIGHTS];
	int num_lights = 0;
	int num_objects = 0;
	void add_light(Lumiere* l) {
		assert(num_lights < MAX_LIGHTS); lights[num_lights++] = l;
	}
	void add_object(Object* o) {
		assert(num_objects < MAX_LIGHTS); objects[num_objects++] = o;
	}

	void renderScene(Camera *camera, int width, int height, Vector* image, int nbrRebond);
	Vector getColor(Ray incident, int nbrRebond);

	Scene();
	~Scene();
};



