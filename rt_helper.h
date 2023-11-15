#pragma once

#include <cmath>
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

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"