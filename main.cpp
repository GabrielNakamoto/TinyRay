#include <iostream>

#include "shading.h"
#include "scene.h"
#include "objects.h"

// dot product http://sites.science.oregonstate.edu/math/home/programs/undergrad/CalculusQuestStudyGuides/vcalc/dotprod/dotprod.html

const int samples_per_pixel = 4;
const int reflect_depth = 4;
const int fov       = M_PI / 2;
const int width     = 1024;
const int height    = 768;

int main(){
    Camera camera(vec3f(0,0,0), width, height, fov);

    std::vector<Sphere> spheres;
    std::vector<Light> lights;

    lights.push_back(Light(vec3f(-20, 20,  20), 1.5));
    lights.push_back(Light(vec3f( 30, 50, -25), 1.8));
    lights.push_back(Light(vec3f( 30, 20,  30), 1.7));

    Material      ivory(vec3f(0.4, 0.4, 0.3), 0.6, 0.3, 0.1,  50);
    Material red_rubber(vec3f(0.3, 0.1, 0.1), 0.9, 0.1, 0.0, 10);
    Material     mirror(vec3f(1.0), 0.0, 10.0, 0.8, 1425.);

    spheres.push_back(Sphere(vec3f(-3,    0,   -16), 2,      ivory));
    spheres.push_back(Sphere(vec3f(-1.0, -1.5, -12), 2, mirror));
    spheres.push_back(Sphere(vec3f( 1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(vec3f( 7,    5,   -18), 4,      mirror));

    Scene scene(camera, spheres, lights, samples_per_pixel, reflect_depth);

    scene.render();

    return 0;
}
