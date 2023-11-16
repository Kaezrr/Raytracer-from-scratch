#include "rt_helper.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include "timer.h"

#include<fstream>
#include<iostream>


int main() {
	// Opening the filestream
	std::ofstream file_out{ "C:/Users/invik/Downloads/image.ppm" };

	Hittable_list world;

	auto material_ground{ make_shared<Lambertian>(Color{0.8, 0.8, 0.0}) };
	auto material_center{ make_shared<Lambertian>(Color{0.7, 0.3, 0.3}) };
	auto material_left	{ make_shared<Metal>(Color{0.8, 0.8, 0.8}, 0.3) };
	auto material_right	{ make_shared<Metal>(Color{0.8, 0.6, 0.2}, 1.0) };

	world.add(make_shared<Sphere>(Point3{ 0.0, -100.5, -1.0 }, 100.0, material_ground));
	world.add(make_shared<Sphere>(Point3{ 0.0,    0.0, -1.0 },   0.5, material_center));
	world.add(make_shared<Sphere>(Point3{-1.0,    0.0, -1.0 },   0.5, material_left));
	world.add(make_shared<Sphere>(Point3{ 1.0,	  0.0, -1.0 },   0.5, material_right));


	Camera cam{};

	cam.aspect_ratio		= 16.0 / 9.0;
	cam.image_width			= 1280;
	cam.samples_per_pixel	= 100;
	cam.max_depth			= 50;

	// Calculating the render time.

	Timer t;

	cam.render(world, file_out);

	std::cout << "\nTime taken: " << t.elapsed() << " seconds\n";

	return 0;
}