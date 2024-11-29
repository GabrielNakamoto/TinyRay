#include <iostream>

#include "shading.h"
#include "scene.h"
#include "objects.h"

// dot product http://sites.science.oregonstate.edu/math/home/programs/undergrad/CalculusQuestStudyGuides/vcalc/dotprod/dotprod.html

const int samples_per_pixel = 4;
const int fov       = M_PI / 2;
const int width     = 1024;
const int height    = 768;

int main(){
    Camera camera(vec3f(0,0,0), width, height, fov);
    std::vector<Sphere> spheres;
    std::vector<Light> lights;

    lights.push_back(Light(vec3f(-20, 20,  20), 1.5));

    Material      ivory(vec3f(0.4, 0.4, 0.3));
    Material red_rubber(vec3f(0.3, 0.1, 0.1));

    spheres.push_back(Sphere(vec3f(-3,    0,   -16), 2,      ivory));
    spheres.push_back(Sphere(vec3f(-1.0, -1.5, -12), 2, red_rubber));
    spheres.push_back(Sphere(vec3f( 1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(vec3f( 7,    5,   -18), 4,      ivory));

    Scene scene(camera, spheres, lights, samples_per_pixel);

    scene.render();

    return 0;
}
