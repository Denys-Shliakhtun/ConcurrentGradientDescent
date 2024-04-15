#pragma once
#include <vector>
#include "utils.h"

Point gradientDescentInterval(
	Point(*derivative)(Point),
	Interval interval,
	Point start,
	long double learn_rate,
	int n_iter,
	long double tolerance = 1e-06
);
std::vector<Point> linearGradientDescentInterval(
	Point(*derivative)(Point),
	Interval interval,
	int num_of_intervals,
	long double learn_rate,
	int n_iter,
	long double tolerance = 1e-06
);

std::vector<Point> concurrentGradientDescentInterval(
	int numOfThreads,
	Point(*derivative)(Point),
	Interval interval,
	int num_of_intervals,
	long double learn_rate,
	int n_iter,
	long double tolerance = 1e-06
);

