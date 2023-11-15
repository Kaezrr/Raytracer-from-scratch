#pragma once

#include "rt_helper.h"

class Interval {
public:
	double min{};
	double max{};

	Interval() : min(+infinity), max(-infinity) {}
	Interval(double _min, double _max) : min{ _min }, max{ _max } {}

	bool contains(double x) const {
		return min <= x && x <= max;
	}

	bool surrounds(double x) const {
		return min < x && x < max;
	}

	static const Interval empty;
	static const Interval universe;
};


static inline const Interval empty		{ +infinity, -infinity };
static inline const Interval universe	{ -infinity, +infinity };