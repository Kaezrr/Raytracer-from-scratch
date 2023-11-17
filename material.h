#pragma once

#include "rt_helper.h"
#include "hittable.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r_in, const Hit_record& rec, Color& attenuetion, Ray& scattered) const = 0;
};


class Lambertian : public Material {
private:
	Color albedo{};

public:
	Lambertian(const Color& a) : albedo{ a } {}

	bool scatter([[maybe_unused]] const Ray& r_in, const Hit_record& rec, Color& attenuetion, Ray& scattered) const override {
		auto scatter_direction{ rec.normal + random_unit_vector() };

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuetion = albedo;

		return true;
	}
};


class Metal : public Material {
private:
	Color albedo{};
	double fuzz{};

public:
	Metal(const Color& a, double f) : albedo{ a }, fuzz{ (f < 1) ? f : 1 } {}

	bool scatter(const Ray& r_in, const Hit_record& rec, Color& attenuetion, Ray& scattered) const override {
		auto reflection{ reflect(unit_vector(r_in.direction()), rec.normal) };
		scattered = Ray(rec.p, reflection + fuzz * random_unit_vector());
		attenuetion = albedo;

		return dot(scattered.direction(), rec.normal) > 0;
	}
};


class Dielectric : public Material {
private:
	double index_of_refraction{};

	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance
		auto r0{ (1 - ref_idx) / (1 + ref_idx) };
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

public:
	Dielectric(double ir) : index_of_refraction{ ir } {}

	bool scatter(const Ray& r_in, const Hit_record& rec, Color& attenuetion, Ray& scattered) const override {
		attenuetion = Color{ 1.0, 1.0, 1.0 };
		double refraction_ratio{ rec.front_face ? (1.0 / index_of_refraction) : index_of_refraction };

		Vec3 unit_direction{ unit_vector(r_in.direction()) };
		double cos_theta{ fmin(dot(-unit_direction, rec.normal), 1.0) };
		double sin_theta{ sqrt(1 - cos_theta * cos_theta) };
		
		bool cannot_refract{ refraction_ratio * sin_theta > 1.0 };
		Vec3 direction{};

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = Ray{ rec.p, direction };
		return true;
	}
};