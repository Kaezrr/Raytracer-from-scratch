#pragma once

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3 {
private:
	double m_coords[3];

public:
	Vec3() : m_coords{ 0, 0, 0 } {}
	Vec3(double x, double y, double z) : m_coords{ x, y, z } {}

	double x() const { return m_coords[0]; }
	double y() const { return m_coords[1]; }
	double z() const { return m_coords[2]; }

	Vec3 operator-() const { return { -m_coords[0], -m_coords[1], -m_coords[2] }; }
	double operator[](int i) const { return m_coords[i]; }
	double& operator[](int i) { return m_coords[i]; }

	Vec3& operator+=(const Vec3& v) {
		m_coords[0] += v.m_coords[0];
		m_coords[1] += v.m_coords[1];
		m_coords[2] += v.m_coords[2];
		return *this;
	}

	Vec3& operator*=(double n) {
		m_coords[0] *= n;
		m_coords[1] *= n;
		m_coords[2] *= n;
		return *this;
	}

	Vec3& operator/=(double n) {
		return *this *= 1 / n;
	}

	double length_squared() const {
		return m_coords[0] * m_coords[0] + m_coords[1] * m_coords[1] + m_coords[2] * m_coords[2];
	}

	double length() const {
		return sqrt(length_squared());
	}
};


using Point3 = Vec3;


// Vector Utility Functions	

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

//continue here...