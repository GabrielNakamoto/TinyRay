#pragma once

#include <iostream>
#include <vector>

#include "shading.h"
#include "objects.h"

struct Intersection {
    Material material;
    vec3f hit_point;
    vec3f normal;
};

struct Camera {
private:
    double aspect_ratio;
    int fov;

public:
    vec3f position;
    int image_width;
    int image_height;

    Camera(const vec3f &pos, const int &width, const int &height, const int &fov) : position(pos), image_width(width), image_height(height), fov(fov) {
        aspect_ratio = (double)width/height;
    };

    vec3f get_ray(double dx, double dy, int px, int py);
};

class Scene {
public:
    Scene(const Camera &camera, const std::vector<Sphere> &spheres, const std::vector<Light> &lights, int samples = 1) : camera(camera), lights(lights), spheres(spheres), samples(samples) {};
    void render();
    bool trace(const vec3f &origin, const vec3f &dir, Intersection &intersect);
    vec3f shade(const vec3f& dir);

private:

    int samples;
    std::vector<vec3f> framebuffer;
    Camera camera;
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
};
