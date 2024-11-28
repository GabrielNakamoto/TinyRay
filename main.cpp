#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include "vec.h"

struct Sphere {
    vec3f center;
    float radius;

    Sphere(const vec3f& center, const float &r) : center(center), radius(r) {};

    bool ray_intersect(const vec3f &orig, const vec3f &dir, float &t){
        // find line from origin (eye) to center of circle
        vec3f L = center - orig;
        // project the line from center of circle onto ray
        float pv = L.dot(dir);
        // calculate closest distance from projection to center of circle (squared)
        float d = L.dot(L) - pv*pv;
        if(d > radius*radius) return false;
        return true;
    }
};

// always normalize directions
const int fov      = M_PI / 2;
const int width = 1024;
const int height = 768;

vec3f build_camera_ray(int px, int py){
    const float aspect_ratio = (float) width / height;
    float x = (2*((px+0.5)/(float)width) - 1) * tan(fov/2.) * (float)aspect_ratio;
    float y = -(2*((py+0.5)/(float)height) - 1) * tan(fov/2.);
    return vec3f(x,y,-1).normalize();
}

void render(){
    std::vector<vec3f> framebuffer(width*height);

    Sphere sphere(vec3f(5,-2,-16), 2);

    int intersect_count=0;
    for (size_t j = 0; j<height; j++) {
        for (size_t i = 0; i<width; i++) {
            float t;
            vec3f eye(0,0,0);
            vec3f dir = build_camera_ray(i,j);
            if(sphere.ray_intersect(eye, dir, t)){
                intersect_count++;
                framebuffer[i+j*width] = vec3f(0.4, 0.4, 0.3);
            } else {
                framebuffer[i+j*width] = vec3f(0.2, 0.7, 0.8);
            }
        }
    }

    std::ofstream ofs; // save the framebuffer to file
    ofs.open("./out.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height*width; ++i) {
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].x)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].y)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].z)));
    }
    ofs.close();

    std::clog << "Intersections: " << intersect_count;
}

int main(){
    render();
    return 0;
}
