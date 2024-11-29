#pragma once

#include <iostream>
#include <vector>

#include "shading.h"
#include "objects.h"

struct Intersection {
    Material material;
    vec3f hit_point;
    vec3f normal;
    float distance;
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
    int intersection_count;

    Scene(const Camera &camera, const std::vector<Sphere> &spheres, const std::vector<Light> &lights, int samples = 1, int reflect_depth = 4) : camera(camera), lights(lights), spheres(spheres), samples(samples), intersection_count(), reflect_depth(reflect_depth) {};
    void render();
    bool trace(const vec3f &origin, const vec3f &dir, Intersection &intersect);
    vec3f shade(const vec3f &origin, const vec3f& dir, size_t depth = 0);

private:

    int samples;
    int reflect_depth;
    Camera camera;

    std::vector<vec3f> framebuffer;
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
};
