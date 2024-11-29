#pragma once

#include <iostream>

#include "geometry.h"
#include "shading.h"

struct Sphere {
    vec3f center;
    float radius;
    Material material;

    Sphere(const vec3f& center, const float &r, const Material &mat) : center(center), radius(r), material(mat) {};

    // always normalize directions
    bool ray_intersect(const vec3f &orig, const vec3f &dir, float &t0){
        // find line from origin (eye) to center of circle
        vec3f L = center - orig;
        // project the line from center of circle onto ray
        float pv = L.dot(dir);
        // calculate closest distance from projection to center of circle (squared)
        float d = L.dot(L) - pv*pv;
        if(d > radius*radius) return false;
        float q = sqrt(radius*radius - d);
        t0 = pv-q;
        float t1 = pv+q;
        if(t0 < 0) t0 = t1;
        if(t0 < 0) return false;
        return true;
    }
};

