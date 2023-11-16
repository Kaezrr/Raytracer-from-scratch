#include "rt_helper.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "timer.h"

#include<fstream>
#include<iostream>


int main() {

	// Opening the filestream
	std::ofstream file_out{ "C:/Users/invik/Downloads/image.ppm" };


	Hittable_list world;

	world.add(make_shared<Sphere>(Point3{ 0, 0, -1 }, 0.5));
	world.add(make_shared<Sphere>(Point3{ 0, -100.5, -1 }, 100));

	Camera cam{};

	cam.aspect_ratio		= 16.0 / 9.0;
	cam.image_width			= 400;
	cam.samples_per_pixel	= 100;
	cam.max_depth			= 50;

	// Calculating the render time.

	Timer t;

	cam.render(world, file_out);

	std::cout << "\nTime taken: " << t.elapsed() << " seconds\n";

	return 0;
}