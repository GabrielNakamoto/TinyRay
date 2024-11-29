#pragma once

#include <iostream>
#include "geometry.h"

struct Material {
    Material() : diffuse_color() {};
    Material(const vec3f &diffuse) : diffuse_color(diffuse) {};

    vec3f diffuse_color;
};

struct Light {
    Light(const vec3f &p, const float &i) : pos(p), intensity(i) {};
    vec3f pos;
    float intensity;
};
