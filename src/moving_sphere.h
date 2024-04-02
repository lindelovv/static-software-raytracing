#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"
#include "hittable.h"

class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(point3 cen0, point3 cen1, float _time0, float _time1, float r, shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
        {};

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        point3 center(float time) const;

    public:
        point3 center0, center1;
        float time0, time1;
        float radius;
        shared_ptr<material> mat_ptr;
};

point3 moving_sphere::center(float time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    float a = r.direction().length_squared();
    float half_b = dot(oc, r.direction());
    float c = oc.length_squared() - radius * radius;

    float discriminant = half_b * half_b - a * c;
    if( discriminant < 0 ) {
        return false;
    }
    float sqrtd = std::sqrt(discriminant);

    // find the nearest root that lies in the acceptable range
    float root = (-half_b - sqrtd) / a;
    if( root < t_min || t_max < root ) {
        root = (-half_b + sqrtd) / a;
        if( root < t_min || t_max < root ) {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
};

#endif
