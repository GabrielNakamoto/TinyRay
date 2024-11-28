#include <iostream>
#include <cmath>

struct vec3f {
    float x, y, z;

    vec3f() : x(), y(), z() {};
    vec3f(float x, float y, float z) : x(x), y(y), z(z) {};

    vec3f operator+(const vec3f &v){
        return vec3f(x+v.x, y+v.y, z+v.z);
    }

    vec3f operator-(const vec3f &v){
        return vec3f(x-v.x, y-v.y, z-v.z);
    }

    vec3f operator*(const float &s){
        return vec3f(x*s, y*s, z*s);
    }

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
