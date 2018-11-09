#include"CImg.h"
#include "Vector.h"
#include "Scene.h"

void save_image(Vector* image, int width, int height, char* filename)
{

    unsigned char* outImg = new unsigned char[3 * height*width];
    for (int i = 0; i<height; i++)
    {
        for (int j = 0; j<width; j++)
        {
            Vector v = image[(height - i - 1)*width + j];
        
            v.x = fmin(1, v.x);
            v.y = fmin(1, v.y);
            v.z = fmin(1, v.z);

            outImg[i*width + j] = (unsigned char)(v.x*255.f);
            outImg[i*width + j + height*width] = (unsigned char)(v.y*255.f);
            outImg[i*width + j + 2 * height*width] = (unsigned char)(v.z*255.f);
        }
    }
    cimg_library::CImg<unsigned char> cimg(outImg, width, height, 1, 3);
    cimg.save(filename);
}

int main()
{
    int width = 500;
    int height = 500;

    Vector* image = new Vector[width*height];

    Scene scene;

    Material mat0;
    mat0.type = Material::LAMBERT;
    mat0.kd = Vector(1, 0, 0);

    Material mat1;
    mat1.type = Material::LAMBERT;
    mat1.kd = Vector(1, 1, 1);
    
    Material mat2;
    mat2.type = Material::LAMBERT;
    mat2.kd = Vector(0, 0.6, 0.9);

    Material mat3;
    mat3.type = Material::LAMBERT;
    mat3.kd = Vector(0.6, 0.9, 0);

    Material mat4;
    mat4.type = Material::MIRROIR;
    mat4.ks = Vector(1,1,1);

    Material mat5;
    mat5.type = Material::LAMBERT;
    mat5.kd = Vector(1, 1, 1);
    mat5.ks = Vector(1, 1, 1);
    mat5.theta_cone = 4;
    mat5.checker = 1;

    Material mat6;
    mat6.type = Material::GLOSSY;
    mat6.kd = Vector(1, 1, 1);
    mat6.ks = Vector(0.9f, 0.6f, 0.f);
    mat6.theta_cone = 20;

#define num_spheres 7
    Sphere sph[num_spheres];

    sph[0].O = Vector(0, 0, 0);
    sph[0].R = 3.2f;
    sph[0].mat = &mat5;

    sph[1].O = Vector(0, -10000 - 2.f, 0);
    sph[1].R = 10000.f;
    sph[1].mat = &mat1;

    sph[2].O = Vector(+10000 + 10, 0, 0);
    sph[2].R = 10000.f;
    sph[2].mat = &mat3;

    sph[3].O = Vector(0,0, -10000 - 15.f);
    sph[3].R = 10000.f;
    sph[3].mat = &mat1;

    sph[4].O = Vector(0, 10000 + 25, 0);
    sph[4].R = 10000.f;
    sph[4].mat = &mat1;

    sph[5].O = Vector(-10000 - 10, 0, 0);
    sph[5].R = 10000.f;
    sph[5].mat = &mat2;
    
    sph[6].O = Vector(0, 25, -21);
    sph[6].R = 10.2f;
    sph[6].mat = &mat3;

    scene.add_object(sph + 0);
    scene.add_object(sph + 1);
    scene.add_object(sph + 2);
    scene.add_object(sph + 3);
    scene.add_object(sph + 4);
    scene.add_object(sph + 5);
    //scene.add_object(sph + 6);

    Mesh monkey;
    monkey.load_from_file("models/cube.obj");
    monkey.mat = &mat5;
//    scene.add_object(&monkey);

    LumierePonct lpt;
    lpt.I = 100*Vector(1,1,1);
    lpt.Pos = Vector(-5, 15, 0);
    scene.add_light(&lpt);

    LumierePonct lpt1;
    lpt1.I = 150 * Vector(1, 1, 0.5f);
    lpt1.Pos = Vector(4, 15, 0);
    scene.add_light(&lpt1);

    Camera main_camera;
    main_camera.eye = Vector(0, 12, 15);
    main_camera.look_at = Vector(-15, -5, -100);
    main_camera.up = Vector(0, 1, 0);
    main_camera.fov = 90.f*PI/180.f;

    scene.renderScene(&main_camera, width, height, image, 2);

    save_image(image, width, height, "rendered_image.bmp");
}

