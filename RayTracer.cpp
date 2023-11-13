#include <iostream>
#include <fstream>


int main() {

	// File

	std::ofstream file_out{ "C:/Users/invik/Downloads/image.ppm" };

	//Image 

	int image_width{ 256 };
	int image_height{ 256 };

	// Render

	file_out << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int i{ 0 }; i < image_height; ++i) {
		std::clog << "\rScanlines remaining: " << image_height - i << ' ' << std::flush;
		for (int j{ 0 }; j < image_width; ++j) {

			auto r{ double(j) / (image_width - 1) };
			auto g{ double(i) / (image_height - 1) };
			auto b{ 0 };

			int ir{ static_cast<int>(255.999 * r) };
			int ig{ static_cast<int>(255.999 * g) };
			int ib{ static_cast<int>(255.999 * b) };

			file_out << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}

	std::clog << "\rDone.                       \n";

	return 0;
}