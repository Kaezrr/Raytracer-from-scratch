#pragma once

#include "rt_helper.h"

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3 {
public:
	double coords[3];

	Vec3() : coords{ 0, 0, 0 } {}
	Vec3(double x, double y, double z) : coords{ x, y, z } {}

	double x() const { return coords[0]; }
	double y() const { return coords[1]; }
	double z() const { return coords[2]; }

	Vec3 operator-() const { return { -coords[0], -coords[1], -coords[2] }; }
	double operator[](int i) const { return coords[i]; }
	double& operator[](int i) { return coords[i]; }

	Vec3& operator+=(const Vec3& v) {
		coords[0] += v.coords[0];
		coords[1] += v.coords[1];
		coords[2] += v.coords[2];
		return *this;
	}

	Vec3& operator*=(double n) {
		coords[0] *= n;
		coords[1] *= n;
		coords[2] *= n;
		return *this;
	}

	Vec3& operator/=(double n) {
		return *this *= 1 / n;
	}

	double length_squared() const {
		return coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2];
	}

	bool near_zero() const {
		auto s{ 1e-8 };
		return (fabs(coords[0]) < s) && (fabs(coords[1]) < s) && (fabs(coords[2]) < s);
	}

	double length() const {
		return sqrt(length_squared());
	}

	static Vec3 random() {
		return Vec3{ random_double(), random_double(), random_double() };
	}

	static Vec3 random(double min, double max) {
		return Vec3{ random_double(min, max), random_double(min, max), random_double(min, max) };
	}
};

// Type aliasing for distinction between 3D Point and Color

using Point3 = Vec3;


// Vector Utility Functions	

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.coords[0] << ' ' << v.coords[1] << ' ' << v.coords[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
	return { u.coords[0] + v.coords[0], u.coords[1] + v.coords[1], u.coords[2] + v.coords[2] };
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
	return { u.coords[0] - v.coords[0], u.coords[1] - v.coords[1], u.coords[2] - v.coords[2] };
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
	return { u.coords[0] * v.coords[0], u.coords[1] * v.coords[1], u.coords[2] * v.coords[2] };
}

inline Vec3 operator*(const Vec3& u, double t) {
	return { u.coords[0] * t, u.coords[1] * t, u.coords[2] * t };
}

inline Vec3 operator*(double t, const Vec3& u) {
	return u * t;
}

inline Vec3 operator/(const Vec3& u, double t) {
	return (1/t) * u;
}

inline double dot(const Vec3& u, const Vec3& v) {
	return u.coords[0] * v.coords[0]
		 + u.coords[1] * v.coords[1]
		 + u.coords[2] * v.coords[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
	return { u.coords[1] * v.coords[2] - u.coords[2] * v.coords[1]
			,u.coords[2] * v.coords[0] - u.coords[0] * v.coords[2]
			,u.coords[0] * v.coords[1] - u.coords[1] * v.coords[0] };
}

inline Vec3 unit_vector(const Vec3& v) {
	return v / v.length();
}

inline Vec3 random_on_unit_sphere() {
	while (true) {
		auto p{ Vec3::random(-1, 1) };
		if (p.length_squared() < 1)
			return p;
	}
}

inline Vec3 random_unit_vector() {
	return unit_vector(random_on_unit_sphere());
}

inline Vec3 random_on_hemisphere(const Vec3& normal) {
	Vec3 on_unit_sphere{ random_on_unit_sphere() };
	if (dot(on_unit_sphere, normal) > 0.0) 
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2*dot(v, n)*n;
}

inline Vec3 refract(const Vec3& v, const Vec3& n, double refraction_ratio) {
	auto cos_theta{ fmin(dot(-v, n), 1.0) };
	Vec3 r_out_perp{ refraction_ratio * (v + cos_theta * n) };
	Vec3 r_out_parallel{ -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n };

	return r_out_perp + r_out_parallel;
}