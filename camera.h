#pragma once

#include "rt_helper.h"
#include "hittable.h"
#include "color.h"

#include <iostream>


class Camera {
public:
	double	aspect_ratio{ 1.0 };
	int		image_width{ 100 };

	void render(const Hittable& world, std::ostream& file_out=std::cout) {
		initialize();

		file_out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int i{ 0 }; i < image_height; ++i) {
			std::clog << "\rScanlines remaining: " << image_height - i << ' ' << std::flush;

			for (int j{ 0 }; j < image_width; ++j) {
				auto pixel_center{ pixel00_loc + (i * pixel_delta_ver) + (j * pixel_delta_hor) };
				auto ray_direction{ pixel_center - camera_center };
				Ray ray{ camera_center, ray_direction };

				Color pixel_color{ ray_color(ray, world) };
				write_color(file_out, pixel_color);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	int		image_height{};		// Rendered image height
	Point3	camera_center{};	// Camera center
	Point3	pixel00_loc{};		// Location of pixel 0, 0
	Vec3	pixel_delta_hor{};	// Offset to pixel to the right
	Vec3	pixel_delta_ver{};	// Offset to pixel below

	void initialize() {

		// Calculate Image height and ensure it's at least 1

		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		camera_center = Point3{ 0, 0, 0 };

		// Determine viewport dimensions.
		auto focal_length{ 1.0 };
		auto viewport_height{ 2.0 };
		auto viewport_width{ viewport_height * (static_cast<double>(image_width) / image_height) };

		// Calculate the vectors across the viewport vertical and horizontal edges
		Vec3 viewport_horizontal{ viewport_width, 0, 0 };
		Vec3 viewport_vertical{ 0, -viewport_height, 0 };

		// Calculate the horizontal and vertical delta vectors from pixel to pixel
		pixel_delta_hor = viewport_horizontal / image_width;
		pixel_delta_ver = viewport_vertical / image_height;

		// Calculate location of upper left pixel
		auto viewport_upper_left{ camera_center - Vec3{0,0,focal_length} - viewport_horizontal / 2 - viewport_vertical / 2 };
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_hor + pixel_delta_ver);
	}

	Color ray_color(const Ray& r, const Hittable& world) const {
		Hit_record rec{};

		if (world.hit(r, Interval{ 0, infinity }, rec)) {
			return 0.5 * (rec.normal + Color{ 1, 1, 1 });
		}

		Vec3 unit_direction{ unit_vector(r.direction()) };
		auto a{ 0.5 * (unit_direction.y() + 1) };
		return (1.0 - a) * Color { 1.0, 1.0, 1.0 } + a * Color{ 0.5, 0.7, 1.0 };
	}
};