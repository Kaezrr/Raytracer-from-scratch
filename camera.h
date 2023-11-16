#pragma once

#include "rt_helper.h"
#include "hittable.h"
#include "color.h"
#include "material.h"

#include <iostream>


class Camera {
public:
	double	aspect_ratio		{ 1.0 };	// Ratio of image width over image height
	int		image_width			{ 100 };	// Rendered image width in pixel count
	int		samples_per_pixel	{ 10 };		// Count of random samples for each pixel
	int		max_depth			{ 10 };		// Maximum number of ray bounces in the scene

	double	vfov	{ 90 };			// Vertical view angle (field of view).
	Point3	lookfrom{ 0, 0, -1 };	// Point camera is looking from.
	Point3	lookat	{ 0, 0, 0 };	// Point camera is looking at.
	Vec3	vup		{ 0, 1 ,0 };	// Relative "up" position of the camera.

	void render(const Hittable& world, std::ostream& file_out=std::cout) {
		initialize();
		file_out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j{ 0 }; j < image_height; ++j) {
			std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;

			for (int i{ 0 }; i < image_width; ++i) {
				Color pixel_color{ 0, 0, 0 };

				for (int sample{ 0 }; sample < samples_per_pixel; ++sample) {
					Ray r{ get_ray(i, j) };
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(file_out, pixel_color, samples_per_pixel);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	int		image_height	{};		// Rendered image height.
	Point3	camera_center	{};		// Camera center.
	Point3	pixel00_loc		{};		// Location of pixel 0, 0.
	Vec3	pixel_delta_hor	{};		// Offset to pixel to the right.
	Vec3	pixel_delta_ver	{};		// Offset to pixel below.
	Vec3	u, v, w;				// Camera basis vectors.

	void initialize() {
		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		camera_center = lookfrom;

		// Determine viewport dimensions.
		auto focal_length{ (lookfrom - lookat).length() };
		auto theta{ degrees_to_radians(vfov) };
		auto h{ tan(theta / 2) };
		auto viewport_height{ 2 * h * focal_length };
		auto viewport_width{ viewport_height * (static_cast<double>(image_width) / image_height) };

		// Calculate u, v, w basis vectors for the camera coordinate frame.
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = unit_vector(cross(w, u));

		// Calculate the vectors across the viewport vertical and horizontal edges
		Vec3 viewport_horizontal{ u * viewport_width };
		Vec3 viewport_vertical{ -v * viewport_height };

		// Calculate the horizontal and vertical delta vectors from pixel to pixel
		pixel_delta_hor = viewport_horizontal / image_width;
		pixel_delta_ver = viewport_vertical / image_height;

		// Calculate location of upper left pixel
		auto viewport_upper_left{ camera_center - (w * focal_length) - viewport_horizontal / 2 - viewport_vertical / 2 };
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_hor + pixel_delta_ver);
	}

	Ray get_ray(int i, int j) const {
		// Get a randomly sampled camera ray for the pixel at location i, j.
		auto pixel_center{ pixel00_loc + (i * pixel_delta_hor) + (j * pixel_delta_ver) };
		auto pixel_sample{ pixel_center + pixel_sample_square() };

		auto ray_origin{ camera_center };
		auto ray_direction{ pixel_sample - ray_origin };

		return Ray{ ray_origin, ray_direction };
			
	}

	Point3 pixel_sample_square() const {
		// Returns a random point in the square surrounding the pixel at the origin.
		auto px{ -0.5 + random_double() };
		auto py{ -0.5 + random_double() };

		return (px * pixel_delta_hor) + (py * pixel_delta_ver);
	}

	Color ray_color(const Ray& r, int depth, const Hittable& world) const {
		Hit_record rec{};

		// We have reached maximum ray bounce limit, not more light gathered.
		if (depth <= 0)
			return Color{ 0,0,0 };

		// Minimum time set to 0.001 to prevent shadow acne.
		if (world.hit(r, Interval{ 0.001, infinity }, rec)) {
			Ray scattered{};
			Color attenuetion{};
			if (rec.mat->scatter(r, rec, attenuetion, scattered))
				return attenuetion * ray_color(scattered, depth - 1, world);

			return Color{ 0, 0, 0 };
		}

		Vec3 unit_direction{ unit_vector(r.direction()) };
		auto a{ 0.5 * (unit_direction.y() + 1) };
		return (1.0 - a) * Color { 1.0, 1.0, 1.0 } + a * Color{ 0.5, 0.7, 1.0 };
	}
};