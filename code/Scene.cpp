#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>

float getRndFloat()
{
    static default_random_engine engine;
    static uniform_real_distribution<float> distrib(0, 1);
    return distrib(engine);
}

Ray getCameraRay(Camera* camera, float width, float height, int i, int j, int s, int t,int numPixelRays)
{
    // (i,j) coordonées du pixel sur l'image
    // (s,t) coordonées de la case
    float x = (float)j + ((float)t + getRndFloat()) / numPixelRays - 0.5f*width;
    float y = (float)i + ((float)s + getRndFloat()) / numPixelRays - 0.5f*height;

    Vector w = normalize(camera->eye - camera->look_at);
    Vector v = normalize(camera->up);
    Vector u = v.prodVect(w);

    Ray result;
    result.dir = normalize(x*u + y*v - (height*0.5f / tan(camera->fov*0.5f))*w);
    result.orig = camera->eye;

    return result;
}

Ray::Ray(Vector oo, Vector dd)
{
    orig = oo;
    dir = dd;
}

Sphere::Sphere() {

}

Scene::Scene()
{
    num_lights = 0;
}


Scene::~Scene()
{
}

Vector getRandDirOnHemisphere(Vector n)
{
    Vector w = n;
    Vector u = normalize(w.prodVect(Vector(0.002f, 1, 0.33f)));
    Vector v = w.prodVect(u);

    
    float r1 = getRndFloat(), r2 = getRndFloat();
    float x = sqrt(1 - r2)*cosf(2 * PI*r1), y=sqrt(1 - r2)*sinf(2 * PI*r1), z= sqrt(r2);
    return u*x+ y*v + z*w;
}

Vector Scene::getColor(Ray incident, int nbrRebond)
{
    if (nbrRebond == 0) return Vector(0, 0, 0); // nbre maximale de rebonds dépassé
    
    // collecte d'information d'intersection 

    float tmax = 1e9;
    IntersectInfo inter, closestInter;
    bool intersected = false;
    for (int k = 0; k < num_objects; k++)
    {
        if (objects[k]->intersect(incident, SHADOW_EPS, tmax, &inter))
        {
            closestInter = inter;
            tmax = inter.t;
            intersected = true;
        }
    }

    if (!intersected) return Vector(0, 0, 0); // aucun objet n'est detecté

    Vector LoDirect;
    Vector BRDF;// bidirectionnal reflectance distribution function 
    Vector Li = 0;// radiance incidente
    Vector Lo;
    float T = (fmod(inter.u * 5, 1)>0.5f) ^ (fmod(inter.v * 5, 1)>0.5f);
    
    // test d'dentification du materiau 
    if (closestInter.mat->type == Material::LAMBERT)
    {
        BRDF = closestInter.mat->kd / PI;
        for (int i = 0; i < num_lights;i++)
        {
            Li = Li + lights[i]->L(this,closestInter,incident);
        }

        LoDirect.x = BRDF.x*Li.x;
        LoDirect.z = BRDF.z*Li.z;
        LoDirect.y = BRDF.y*Li.y;
    }
    else if (closestInter.mat->type == Material::MIRROIR)
    {
        Vector  normale=closestInter.n;
        Ray newRay(closestInter.pt + SHADOW_EPS*closestInter.n, incident.dir - 2 * incident.dir.dot(normale)*normale);
        Li=getColor(newRay,nbrRebond-1);
        BRDF = closestInter.mat->ks;
        
        LoDirect.x = BRDF.x*Li.x;
        LoDirect.z = BRDF.z*Li.z;
        LoDirect.y = BRDF.y*Li.y;

    }
    else if (closestInter.mat->type == Material::TRANSPARANT)
    {
        float nAir = 1, nSph = closestInter.mat->kr;
        if (closestInter.n.dot(incident.dir) > 0)
        {
            closestInter.n = -closestInter.n;
            swap(nAir, nSph);
        }
        Vector  normale = closestInter.n;
        
        float qn = 1 - (nAir / nSph)*(nAir / nSph)*(1 - (incident.dir.dot(normale))*(incident.dir.dot(normale)));

        if (qn >= 0)
        {
            Ray newRay(closestInter.pt - SHADOW_EPS*normale, (nAir / nSph)*incident.dir - ((nAir / nSph)*incident.dir.dot(normale) + sqrt(qn))*normale);
            Li = getColor(newRay, nbrRebond);
            BRDF = Vector(1, 1, 1);
        }
        else
        {
            Ray newRay(closestInter.pt + SHADOW_EPS*closestInter.n, incident.dir - 2 * incident.dir.dot(normale)*normale);
            Li = getColor(newRay, nbrRebond - 1);
            BRDF = closestInter.mat->ks;
        }
        
        LoDirect.x = BRDF.x*Li.x;
        LoDirect.z = BRDF.z*Li.z;
        LoDirect.y = BRDF.y*Li.y;
    }
    else if (closestInter.mat->type == Material::GLOSSY)
    {
        BRDF = closestInter.mat->kd / PI;
        for (int i = 0; i < num_lights; i++)
        {
            Li = Li + lights[i]->L(this, closestInter, incident);
        }

        LoDirect.x = BRDF.x*Li.x;
        LoDirect.z = BRDF.z*Li.z;
        LoDirect.y = BRDF.y*Li.y;


        Vector  normale = closestInter.n;
        Vector w = incident.dir - 2 * incident.dir.dot(normale)*normale;
        Vector u = normalize(-w.prodVect(Vector(0.005f, 1, 0.003f)));
        Vector v = -w.prodVect(u);

        float theta = 2 * PI*getRndFloat();

        Ray newRay;
        newRay.orig = closestInter.pt;// + SHADOW_EPS*cloesetInter->n;
        newRay.dir = w + sinf(closestInter.mat->theta_cone)*(cosf(theta)*u + sinf(theta)*v);

        Li = getColor(newRay, nbrRebond - 1);

        Lo.x += closestInter.mat->ks.x*Li.x;
        Lo.y += closestInter.mat->ks.y*Li.y;
        Lo.z += closestInter.mat->ks.z*Li.z;

        BRDF = closestInter.mat->ks;

        LoDirect.x = BRDF.x*Li.x;
        LoDirect.z = BRDF.z*Li.z;
        LoDirect.y = BRDF.y*Li.y;
    }

    Vector LoIndirect;
    Vector rayDir = getRandDirOnHemisphere(closestInter.n);
    Ray ray = Ray(closestInter.pt, rayDir);
    Vector LiIndirect = getColor(ray, nbrRebond - 1);
    if (closestInter.mat->type == Material::LAMBERT)
    {
        LoIndirect.x= closestInter.mat->kd.x*LiIndirect.x;
        LoIndirect.y= closestInter.mat->kd.y*LiIndirect.y;
        LoIndirect.z = closestInter.mat->kd.z*LiIndirect.z;
    }
    if (inter.mat->checker)
    {
        Vector Result;
        Result = (T<0.5f)?(LoIndirect + LoDirect) : (LoIndirect + LoDirect)*0.7f;
        return Result;
    }
    else
    {
        return LoIndirect + LoDirect;
    }

}
void Scene::renderScene(Camera* camera, int width, int height, Vector* image, int maxRebond)
{
    int numPixelRays = 2;
    for (int i = 0; i<height; i++)
    {
        for (int j = 0; j<width; j++)
        {
            // génération des pixelRays 
            for (int s = 0; s < numPixelRays; s++)
            {
                for (int t = 0; t < numPixelRays; t++)
                {
                    float tmax = 1e9;
                    Ray rayIJ = getCameraRay(camera, width, height, i, j, s, t,numPixelRays);
                    image[i*width + j] = image[i*width + j] + getColor(rayIJ, maxRebond);
                }

            }
            // moyenne 
            image[i*width + j] = image[i*width + j] / (numPixelRays*numPixelRays);
        }
    }

}



bool Sphere::intersect(Ray ray, float tmin, float tmax, IntersectInfo* result)
{
    
    float a = 1, b = 2 * (ray.dir).dot(ray.orig - O), c = (ray.orig - O).normsquare() - R*R;
    float delta = b*b - 4 * a*c;

    if (delta > 0)
    {
        float t2 = (-b + sqrt(delta)) / 2, t1 = (-b - sqrt(delta)) / 2;
        if (t1 > tmin && t1 < tmax)
        {
            result->pt = (ray.orig + (t1*ray.dir));
            result->t = t1;
            result->n = normalize((result->pt) - O);
            result->mat = mat;

            result->u = (1 + atan2(result->n.z, result->n.x) / PI) * 0.5;
            result->v = acosf(result->n.y) / PI;
            return true;
        }
        if (t2 > tmin && t2 < tmax)
        {
            result->pt = (ray.orig + (t2*ray.dir));
            result->t = t2;
            result->n = normalize((result->pt) - O);
            result->mat = mat;
            float phi = atan2(result->pt.z, result->pt.x);
            float theta = asin(result->pt.y);
            result->u = 1 - (phi + PI) / (2 * PI);
            result->v = (theta + PI / 2) / PI;
            return true;
        }
    }

    return false;
}

Vector LumierePonct::L(Scene* scene,IntersectInfo inter, Ray incident)
{
    Vector l = normalize(Pos - inter.pt);// direction lumière->pt intersect
    float  d2 = (Pos - inter.pt).normsquare();// d(lum , pt intersect)

    IntersectInfo sh_result;
    bool shadowed = false;
    for (int i = 0; i < scene->num_objects; i++) {
        if (scene->objects[i]->intersect(Ray(inter.pt + SHADOW_EPS*inter.n, l), 0, sqrtf(d2), &sh_result))
        {
            if (sh_result.mat->type != Material::TRANSPARANT)
            {
                shadowed = true;
                break;
            }
        }
    }

    if (shadowed)return Vector();

    return I * fmax(0,l.dot(inter.n))/ d2;
}


void Mesh::load_from_file(char* filename)
{
    FILE* file = fopen(filename, "rb");
    assert(file);

    fseek(file, 0, SEEK_END);
    int filesize = ftell(file);
    rewind(file);

    char* filecontent = (char*)malloc((filesize+1)*sizeof(char));
    int read = fread(filecontent, filesize,1, file);
    assert(read == 1);
    filecontent[filesize] = 0;

    int NV = 0;
    int NF = 0;
    int NT = 0;
    int NN = 0;

    char* At = filecontent;
    while ((At - filecontent) < filesize)
    {
        if (*At == 'v')
        {
            if ((At - filecontent + 1) < filesize)
            {
                if (*(At + 1) == ' ')
                    NV++;
                else if (*(At + 1) == 't')
                    NT++;
                else if (*(At + 1) == 'n')
                    NN++;
            }
        }
        else if (*At == 'f')
            NF++;

        while ((At - filecontent < filesize) && *At != '\n')
            At++;
        At++;
    }

    vertices = (Vector*)malloc(sizeof(Vector)* NV);
    normals = (Vector*)malloc(sizeof(Vector)* NN);
    uvs = (Vector*)malloc(sizeof(Vector)* NT);
    faces = (FaceElement*)malloc(3*sizeof(FaceElement)* NF);

    At = filecontent;
    while (At - filecontent < filesize)
    {
        if (*At == 'v')
        {
            At++;
            if (*At == ' ')
            {
                At++;
                Vector P;
                sscanf(At, "%f %f %f", &P.x, &P.y, &P.z);
                vertices[nv++] = P;
            }
            else if (*At == 't')
            {
                At++;
                Vector UV;
                sscanf(At, "%f %f", &UV.x, &UV.y);
                uvs[nt++] = UV;
            }
            if (*At == 'n')
            {
                At++;
                Vector N;
                sscanf(At, "%f %f %f", &N.x, &N.y, &N.z);
                normals[nn++] = N;
            }
        }
        else if (*At == 'f')
        {
            At++;
            if (nn == 0 && nt == 0)
            {
                int A, B, C;
                sscanf(At, "%d %d %d", &A, &B, &C);
                A--; B--; C--;

                faces[3 * nf] = { A,-1,-1 };
                faces[3 * nf + 1] = { B,-1,-1 };
                faces[3 * nf + 2] = { C,-1,-1 };
                nf++;
            }
            else if (nt != 0 && nn == 0)
            {
                int A, B, C;
                int AUV, BUV, CUV;
                sscanf(At, "%d/%d %d/%d %d/%d", &A, &AUV,
                    &B, &BUV, &C, &CUV);
                A--; B--; C--;
                AUV--; BUV--; CUV--;

                faces[3 * nf] = { A,-1,AUV };
                faces[3 * nf + 1] = { B,-1,BUV };
                faces[3 * nf + 2] = { C,-1,CUV };
                nf++;
            }
            else if (nt == 0 && nn != 0)
            {
                int A, B, C;
                int AN, BN, CN;

                sscanf(At, "%d//%d %d//%d %d//%d", &A, &AN,
                    &B, &BN, &C, &CN);
                A--; B--; C--;
                AN--; BN--; CN--;

                faces[3 * nf] = { A,AN,-1 };
                faces[3 * nf + 1] = { B,BN,-1 };
                faces[3 * nf + 2] = { C,CN,-1 };
                nf++;
            }
            else
            {
                int A, B, C;
                int AN, BN, CN;
                int AUV, BUV, CUV;
                sscanf(At, "%d/%d/%d %d/%d/%d %d/%d/%d",
                    &A, &AUV, &AN,
                    &B, &BUV, &BN,
                    &C, &CUV, &CN);
                A--; B--; C--;
                AN--; BN--; CN--;
                AUV--; BUV--; CUV--;

                faces[3 * nf] = { A,AN,AUV };
                faces[3 * nf + 1] = { B,BN,BUV };
                faces[3 * nf + 2] = { C,CN,CUV };
                nf++;
            }
        }
        else
        {
            while (At - filecontent < filesize && *At != '\n')
                At++;
            At++;
        }
    }

    free(filecontent);
}


bool Mesh::intersect(Ray ray, float tmin, float tmax, IntersectInfo* result)
{
    IntersectInfo inter;
    bool intersected = false;
    for (int i = 0; i < nf; i++)
    {
        if (intersect_tri(i, ray, SHADOW_EPS, tmax, &inter))
        {
            *result = inter;
            tmax = inter.t;
            intersected = true;
        }
    }
    return intersected;
}

bool Mesh::intersect_tri(int fid, Ray ray, float tmin, float tmax, IntersectInfo* result)
{
    Vector a = 4*vertices[faces[3*fid].vid];
    Vector b = 4*vertices[faces[3*fid + 1].vid];
    Vector c = 4*vertices[faces[3*fid + 2].vid];

    a.y += 10;
    b.y += 10;
    c.y += 10;

    Vector n = normalize((b - a).prodVect(c - a));
    
    //intersection with plane
    float D = n.dot(ray.dir);
    if (D>0)
    {
        n = -n;// for double-face objects
        D = -D;
    }
    if (fabs(D) > EPSILON)
    {
        float t = (a - ray.orig).dot(n) / D;
        if (t > tmin && t < tmax)
        {
            Vector inter_pt = ray.orig + t*ray.dir;
            {
                // Compute vectors        
                Vector v0 = c - a;
                Vector v1 = b - a;
                Vector v2 = inter_pt - a;

                // Compute dot products
                float dot00 = v0.dot(v0);
                float dot01 = v0.dot(v1);
                float dot02 = v0.dot(v2);
                float dot11 = v1.dot(v1);
                float dot12 = v1.dot(v2);

                // Compute barycentric coordinates
                float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
                float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
                float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

                // Check if point is in triangle
                if ((u >= 0) && (v >= 0) && (u + v < 1))
                {
                    result->pt = inter_pt;
                    result->n = n;// maybe interpolate between tri normals
                    result->t = t;
                    result->mat = mat;
                    result->u = u;
                    result->v = v;
                    return true;
                }
            }
        }
    }

    return false;
}
