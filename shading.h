#pragma once

#include <iostream>
#include "geometry.h"

struct Material {
    Material() : diffuse_color() {};
    Material(const vec3f &diffuse, float kd, float ks, float ka, float spec) : diffuse_color(diffuse), kd(kd), ks(ks), ka(ka), specular_exponent(spec) {};

    vec3f diffuse_color;
    float kd, ks, ka;
    // shininess
    float specular_exponent;
};

struct Light {
    Light(const vec3f &p, const float &i) : pos(p), intensity(i) {};
    vec3f pos;
    float intensity;
};
