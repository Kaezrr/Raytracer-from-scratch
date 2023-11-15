#pragma once

#include "hittable.h"

#include <memory>
#include <vector>


class Hittable_list : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> objects{};

	Hittable_list() = default;
	Hittable_list(std::shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(std::shared_ptr<Hittable> object) {
		objects.push_back(object);
	}

	bool hit(const Ray& r, Interval ray_t, Hit_record& rec) const override {
		Hit_record temp_rec{};
		bool hit_anything{ false };
		auto closest_so_far{ ray_t.max };

		for (const auto& object : objects) {
			if (!object->hit(r, Interval{ ray_t.min, closest_so_far }, temp_rec))
				continue;

			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}

		return hit_anything;
	}
};