#pragma once

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::sqrt;
using std::make_shared;
using std::shared_ptr;

// Constants

inline constexpr double infinity{ std::numeric_limits<double>::infinity() };
inline constexpr double pi{ 3.1415926535897932385 };

// Utlility functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	// Returns a random real number in [0, 1)
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	// Returns a random real number in [min, max)
	return min + (max - min) * random_double();
}

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"