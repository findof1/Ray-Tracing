#ifndef MATERIAL_H
#define MATERIAL_H

class hitRecord;

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(
        const ray& r_in, const hitRecord& rec, color& attenuation, ray& scattered
    ) const {
        return false;
    }
};

class lambertian : public material {
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered)
        const override {
        auto scatter_direction = rec.normal + randomUnitVector();

        if (scatter_direction.nearZero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction, rIn.time());
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material {
public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered)
        const override {
        vec3 reflected = reflect(rIn.direction(), rec.normal);
        reflected = unitVector(reflected) + (fuzz * randomUnitVector());
        scattered = ray(rec.p, reflected, rIn.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

    bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered)
        const override {
        attenuation = color(1.0, 1.0, 1.0);
        double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

        vec3 unitDirection = unitVector(rIn.direction());
        

        double cosTheta = fmin(dot(-unitDirection, rec.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta * cosTheta);
        vec3 direction;
        if (ri * sinTheta > 1.0 || reflectance(cosTheta, ri) > randomDouble()) {
            direction = reflect(unitDirection, rec.normal);
        }
        else {
            direction = refract(unitDirection, rec.normal, ri);
        }

        scattered = ray(rec.p, direction, rIn.time());

        
        return true;
    }

private:
    double refractionIndex;

    static double reflectance(double cosTheta, double refractionIndex) {
        auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosTheta), 5);
    }
};

#endif
