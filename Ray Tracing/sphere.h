#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat)
        : center1(center), radius(fmax(0, radius)), mat(mat), is_moving(false) {}

    sphere(const point3& center1, const point3& center2, double radius,
        std::shared_ptr<material> mat)
        : center1(center1), radius(fmax(0, radius)), mat(mat), is_moving(true)
    {
        center_vec = center2 - center1;
    }

    bool hit(const ray& r, interval rayT, hitRecord& rec) const override {
        point3 center = is_moving ? sphereCenter(r.time()) : center1;
        vec3 oc = center - r.origin();
        auto a = r.direction().lengthSquared();
        auto h = dot(r.direction(), oc);
        auto c = oc.lengthSquared() - radius * radius;

        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = sqrt(discriminant);

        auto root = (h - sqrtd) / a;
        if (!rayT.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!rayT.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outwardNormal = (rec.p - center) / radius;
        rec.setFaceNormal(r, outwardNormal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center1;
    bool is_moving;
    vec3 center_vec;
    double radius;
    std::shared_ptr<material> mat;

    point3 sphereCenter(double time) const {
        return center1 + time * center_vec;
    }
};

#endif