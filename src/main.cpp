#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <memory>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // check ray bounce limit
    if( depth <= 0 ) {
        return color(0, 0, 0);
    }
    if( world.hit(r, 0.001, infinity, rec) ) {
        ray scattered;
        color attenuation;
        if( rec.mat_ptr->scatter(r, rec, attenuation, scattered) ) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    shared_ptr<material> ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for( int a = -11; a < 11; a++) {
        for( int b = -11; b < 11; b++) {
            float choose_mat = random_float();
            point3 center(a + 0.9 * random_float(), 0.2, b + 0.9 * random_float());

            if( (center - point3(4, 0.2, 0)).length() > 0.9 ) {
                shared_ptr<material> sphere_material;

                if( choose_mat < 0.8 ) {
                    color albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    point3 center2 = center + vec3(0, random_float(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if( choose_mat < 0.95 ) {
                    color albedo = color::random(0.5, 1);
                    float fuzz = random_float(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    shared_ptr<material> material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    shared_ptr<material> material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    shared_ptr<material> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {
    // img
    const float aspect_ratio = 16.0 / 9.0;
    const int img_w = 400;
    const int img_h = static_cast<int>(img_w / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // world
    hittable_list world = random_scene();

    // camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20.0, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // render
    std::cout << "P3\n" << img_w << ' ' << img_h << "\n255\n";

    for( int j = img_h - 1; j >= 0; --j ) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for( int i = 0; i < img_w; ++i ) {
            color pixel_color(0, 0, 0);
            for( int s = 0; s < samples_per_pixel; ++s ) {
                float u = (i + random_float()) / (img_w - 1);
                float v = (j + random_float()) / (img_h - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}
