#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include "vec.h"
#include <chrono>

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

const int fov       = M_PI / 2;
const int width     = 1024;
const int height    = 768;
const vec3f background_color(0.2, 0.7, 0.8);


vec3f build_camera_ray(int px, int py){
    const float aspect_ratio = (float) width / height;
    float x = (2*((px+0.5)/(float)width) - 1) * tan(fov/2.) * (float)aspect_ratio;
    float y = -(2*((py+0.5)/(float)height) - 1) * tan(fov/2.);
    return vec3f(x,y,-1).normalize();
}

vec3f phongify(std::vector<Light> &lights, vec3f &hit_point, vec3f &normal, vec3f &object_color){
    float diffuse_strength = 0;
    for(auto &light : lights){
        vec3f light_ray = (light.pos - hit_point).normalize();
        diffuse_strength += light.intensity * std::max(0.f, dot(light_ray, normal));
    }
    return object_color * diffuse_strength;
}

vec3f scene_intersect(int &intersect_count, const vec3f& orig, const vec3f& dir, std::vector<Light> &lights, std::vector<Sphere> &spheres, vec3f& normal, vec3f& hit){
    float closest_hit = std::numeric_limits<float>::max();
    bool intersect = false;
    Material material;
    for(size_t i = 0; i < spheres.size(); ++i){
        float hit_distance;
        if(spheres[i].ray_intersect(orig, dir, hit_distance) && hit_distance < closest_hit){
            intersect_count++;
            intersect = true;
            closest_hit = hit_distance;
            hit = orig + (dir * hit_distance);
            normal = (hit - spheres[i].center).normalize();
            material = spheres[i].material;
        }
    }
    if(intersect) return phongify(lights, hit, normal, material.diffuse_color);
    else return background_color;
}

void render(std::vector<Sphere> &spheres, std::vector<Light> &lights){
    std::vector<vec3f> framebuffer(width*height);


    int intersections=0;
    for (size_t j = 0; j<height; j++) {
        for (size_t i = 0; i<width; i++) {
            float t;
            vec3f eye(0,0,0);
            vec3f dir = build_camera_ray(i,j);
            vec3f point, normal;
            framebuffer[i+j*width] = scene_intersect(intersections, eye, dir, lights, spheres, normal, point);
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
    std::clog << intersections << " intersections\n";
}

int main(){
    std::vector<Sphere> spheres;
    std::vector<Light> lights;

    lights.push_back(Light(vec3f(20, 20,  20), 1.5));

    Material      ivory(vec3f(0.4, 0.4, 0.3));
    Material red_rubber(vec3f(0.3, 0.1, 0.1));

    spheres.push_back(Sphere(vec3f(5,-2,-16), 2, ivory));
    spheres.push_back(Sphere(vec3f(-3,4,-10), 3, red_rubber));
    render(spheres, lights);
    return 0;
}
