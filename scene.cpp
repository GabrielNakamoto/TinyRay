#include <random>
#include <fstream>

#include "scene.h"

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

vec3f Camera::get_ray(double dx, double dy, int px, int py){
    const float aspect_ratio = (float) image_width / image_height;
    float x = (2*((px+0.5+dx)/(float)image_width) - 1) * tan(fov/2.) * (float)aspect_ratio;
    float y = -(2*((py+0.5+dy)/(float)image_height) - 1) * tan(fov/2.);
    return vec3f(x,y,-1).normalize();
}

void Scene::render(){
    intersection_count = 0;

    framebuffer = std::vector<vec3f>(camera.image_width*camera.image_height);

    const double invert_sample_scale = 1.0 / samples;

    for(size_t y = 0; y < camera.image_height; y++){
        for(size_t x = 0; x < camera.image_width; x++){
            vec3f color(0);
            for(size_t s = 0; s < samples; s++){
                vec3f ray_dir = camera.get_ray(random_double() - 0.5, random_double() - 0.5, x, y);
                color = color + shade(camera.position, ray_dir);
            }
            framebuffer[x+y*camera.image_width] = color * invert_sample_scale;
        }
    }

    std::ofstream ofs; // save the framebuffer to file
    ofs.open("./out.ppm");
    ofs << "P6\n" << camera.image_width << " " << camera.image_height << "\n255\n";
    for (size_t i = 0; i < camera.image_height*camera.image_width; ++i) {
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].x)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].y)));
        ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i].z)));
    }
    ofs.close();

    std::cout << intersection_count << " intersections\n";
}

vec3f reflect(const vec3f &V, const vec3f &N){
    return V + N*(2*dot(-V,N));
}

vec3f Scene::shade(const vec3f &origin, const vec3f &dir, size_t depth){
    Intersection intersect;

    if(depth < reflect_depth && trace(camera.position, dir, intersect)){
        // calculate reflect color
        vec3f reflect_dir = reflect(dir, intersect.normal).normalize();
        vec3f reflect_orig = intersect.hit_point + intersect.normal*1e-3;
        /* vec3f reflect_orig = dot(reflect_dir, intersect.normal) < 0 ? intersect.hit_point - intersect.normal*1e-3 : intersect.hit_point + intersect.normal*1e-3; */
        vec3f reflect_color = shade(reflect_orig, reflect_dir, depth + 1);

        float diffuse_strength = 0, specular_strength = 0;
        for(auto &light : lights){
            // ray from hit point to light
            vec3f light_dir = (light.pos - intersect.hit_point).normalize();
            float light_dist = (light.pos - intersect.hit_point).length();

            vec3f shadow_orig = intersect.hit_point + intersect.normal*1e-3;
            Intersection shadow_hit;
            if(trace(shadow_orig, light_dir, shadow_hit) && shadow_hit.distance < light_dist)
                continue;

            vec3f view_dir = (camera.position - intersect.hit_point).normalize();
            vec3f reflect_ray = reflect(-light_dir, intersect.normal);
            diffuse_strength += light.intensity * std::max(0.f, dot(light_dir, intersect.normal));
            specular_strength += light.intensity * std::powf(std::max(0.f, dot(view_dir, reflect_ray.normalize())), intersect.material.specular_exponent);
        }
        return intersect.material.diffuse_color * diffuse_strength * intersect.material.kd + specular_strength * intersect.material.ks + reflect_color * intersect.material.ka;
    }
    else {
        return vec3f(0.2, 0.7, 0.8);
    }
}

bool Scene::trace(const vec3f &origin, const vec3f &dir, Intersection &intersect){
    float closest_hit = std::numeric_limits<float>::max();
    bool hit = false;
    Material material;

    for(auto &sphere : spheres){
        float hit_distance;
        if(sphere.ray_intersect(origin, dir, hit_distance) && hit_distance < closest_hit){
            hit = true;
            closest_hit = hit_distance;
            intersect.hit_point = origin + (dir * hit_distance);
            intersect.normal = (intersect.hit_point - sphere.center).normalize();
            intersect.material = sphere.material;
            intersect.distance = hit_distance;
        }
    }

    if(hit) intersection_count++;

    return hit;
}
