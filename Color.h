#pragma once

#include "vec3.h"
#include <iostream>

// Type aliasing for distinction between 3D Point and Color

using Color = Vec3;

inline double linear_to_gamma(double linear_component)
{
	return sqrt(linear_component);
}

void write_color(std::ostream& out, const Color& pixel_color, int samples_per_pixel) {
	auto r{ pixel_color.x() };
	auto g{ pixel_color.y() };
	auto b{ pixel_color.z() };

	// Divide color by number of samples
	auto scale{ 1.0 / samples_per_pixel };
	r *= scale;
	g *= scale;
	b *= scale;

	// Apply linear to gamma transform.
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const Interval intensity{ 0.000, 0.999 };
	out << static_cast<int>(256 * intensity.clamp(r)) << ' '
		<< static_cast<int>(256 * intensity.clamp(g)) << ' '
		<< static_cast<int>(256 * intensity.clamp(b)) << '\n';
}
