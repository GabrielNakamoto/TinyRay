#pragma once

#include <iostream>
#include <cmath>

struct vec3f {
    float x, y, z;

    vec3f() : x(), y(), z() {};
    vec3f(float s) : x(s), y(s), z(s) {};
    vec3f(float x, float y, float z) : x(x), y(y), z(z) {};
    vec3f(const vec3f &v) : x(v.x), y(v.y), z(v.z) {};


    float dot(const vec3f &v){
        return x*v.x + y*v.y + z*v.z;
    }

    float length(){
        return sqrt(dot(*this));
    }

    vec3f normalize(){
        float L = length();
        if(L > 0){
            float il = 1/L;
            x *= il, y *= il, z *= il;
        }
        return *this;
    }
};

inline float dot(const vec3f &ls, const vec3f &rs){
    return ls.x*rs.x + ls.y*rs.y + ls.z*rs.z;
}

inline vec3f operator+(const vec3f &ls, const vec3f &rs){
    return vec3f(ls.x+rs.x, ls.y+rs.y, ls.z+rs.z);
}

inline vec3f operator-(const vec3f &ls, const vec3f &rs){
    return vec3f(ls.x-rs.x, ls.y-rs.y, ls.z-rs.z);
}

inline vec3f operator*(const vec3f &v, float s){
    return vec3f(v.x*s, v.y*s, v.z*s);
}
