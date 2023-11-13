#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3 {
public:
	double m_coords[3];

	Vec3() : m_coords{ 0, 0, 0 } {}
	Vec3(double x, double y, double z) : m_coords{ x, y, z } {}

	double x() const { return m_coords[0]; }
	double y() const { return m_coords[1]; }
	double z() const { return m_coords[2]; }

	Vec3 operator-() const { return { -m_coords[0], -m_coords[1], -m_coords[2] }; }
	double operator[](int i) const { return m_coords[i]; }
	double& operator[](int i) { return m_coords[i]; }
};