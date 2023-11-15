#include "rt_helper.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

#include<iostream>
#include<fstream>


int main() {

	// Opening the filestream
	std::ofstream file_out{ "C:/Users/invik/Downloads/image.ppm" };


	Hittable_list world;

	world.add(make_shared<Sphere>(Point3{ 0, 0, -1 }, 0.5));
	world.add(make_shared<Sphere>(Point3{ 0, -100.5, -1 }, 100));

	Camera cam{};

	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 400;

	cam.render(world, file_out);

	return 0;
}