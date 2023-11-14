#include <iostream>
#include <fstream>
#include <cmath>

#include "Vec3.h"
#include "Color.h"
#include "Ray.h"


double hit_sphere(const Point3& center, double radius, const Ray& r)
{
	Vec3 oc{ r.origin() - center };
	auto a{ r.direction().length_squared() };
	auto half_b{ dot(r.direction(), oc) };
	auto c{ oc.length_squared() - radius*radius };

	auto discriminant{ half_b*half_b - a*c };

	if (discriminant < 0)
		return -1;
	else
		return  (-half_b - std::sqrt(discriminant)) / a;
}


Color ray_color(const Ray& r)
{
	auto t{ hit_sphere(Point3{0, 0, -1}, 0.5, r) };
	if (t > 0.0) {
		Vec3 n{ unit_vector(r.at(t) - Point3{0,0,-1}) };
		return 0.5 * Color{ n.x() + 1, n.y() + 1, n.z() + 1 };
	}

	Vec3 unit_direction{ unit_vector(r.direction()) };
	auto a{ 0.5 * (unit_direction.y() + 1) };

	return (1.0 - a) * Color { 1.0, 1.0, 1.0 } + a * Color{ 0.5, 0.7, 1.0 };
}


int main() {

	// Opening the filestream
	std::ofstream file_out{ "C:/Users/invik/Downloads/image.ppm" };

	// Image

	auto aspect_ratio{ 16.0 / 9.0 };
	int image_width{ 400 };

	// Calculate Image height and ensure it's at least 1
	int image_height{ static_cast<int>(image_width / aspect_ratio) };
	image_height = (image_height < 1) ? 1 : image_height;

	// Camera

	auto focal_length{ 1.0 };
	auto viewport_height{ 2.0 };
	auto viewport_width{ viewport_height * (static_cast<double>(image_width) / image_height) };
	Point3 camera_center{ 0, 0, 0 };

	// Calculate the vectors across the viewport vertical and horizontal edges
	Vec3 viewport_horizontal{ viewport_width, 0, 0 };
	Vec3 viewport_vertical{ 0, -viewport_height, 0 };

	// Calculate the horizontal and vertical delta vectors from pixel to pixel
	auto pixel_delta_hor{ viewport_horizontal / image_width };
	auto pixel_delta_ver{ viewport_vertical / image_height };

	// Calculate location of upper left pixel
	auto viewport_upper_left{ camera_center - Vec3{0,0,focal_length} - viewport_horizontal/2 - viewport_vertical/2 };
	auto pixel00_loc{ viewport_upper_left + 0.5 * (pixel_delta_hor + pixel_delta_ver) };

	// Render

	file_out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i{ 0 }; i < image_height; ++i) {
		std::clog << "\rScanlines remaining: " << image_height - i << ' ' << std::flush;

		for (int j{ 0 }; j < image_width; ++j) {
			auto pixel_center{ pixel00_loc + (i * pixel_delta_ver) + (j * pixel_delta_hor) };
			auto ray_direction{ pixel_center - camera_center };
			Ray ray{ camera_center, ray_direction };

			Color pixel_color{ ray_color(ray) };
			write_color(file_out, pixel_color);
		}
	}

	std::clog << "\rDone.                 \n";

	return 0;
}