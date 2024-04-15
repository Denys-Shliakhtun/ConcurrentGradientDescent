#pragma once
#include <vector>
#include "utils.h"

Point gradientDescentPoint(
	Point(*derivative)(Point),
	Point start,
	long double learn_rate,
	int n_iter,
	long double tolerance = 1e-06
);
std::vector<Point> linearGradientDescentPoint(
	Point(*derivative)(Point),
	std::vector<Point>& start_points,
	long double learn_rate,
	int n_iter,
	long double tolerance = 1e-06
);
std::vector<Point> concurrentGradientDescentPoint(
	int numOfThreads,
	Point(*derivative)(Point),
	std::vector<Point>& start_points,
	long double learn_rate,
	int n_iter,
	long double tolerance = 1e-06
);

