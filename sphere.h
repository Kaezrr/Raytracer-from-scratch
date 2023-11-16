#pragma once

#include "hittable.h"
#include "vec3.h"


class Sphere : public Hittable {
private:
	double radius{};
	Point3 center{};
	shared_ptr<Material> mat{};

public:
	Sphere(Point3 c, double r, shared_ptr<Material> m) 
		: center{ c }, radius{ r }, mat{ m } {}

	bool hit(const Ray& r, Interval ray_t, Hit_record& rec) const override {
		Vec3 oc{ r.origin() - center };
		auto a{ r.direction().length_squared() };
		auto half_b{ dot(r.direction(), oc) };
		auto c{ oc.length_squared() - radius * radius };

		auto discriminant{ half_b * half_b - a * c };
		if (discriminant < 0) return 0;
		auto sqrtd{ std::sqrt(discriminant) };

		// Find the nearest root that lies in acceptable range.
		auto root{ (-half_b - sqrtd) / a };
		if (!ray_t.surrounds(root)) {
			root = (-half_b + sqrtd) / a;
			if (!ray_t.surrounds(root))
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		auto outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;

		return true;
	}
};
