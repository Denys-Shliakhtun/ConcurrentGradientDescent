#include "PointGradientDescent.h"
#include <oneapi/tbb/task_arena.h>
#include <oneapi/tbb/task_group.h>

std::vector<Point> linearGradientDescentPoint(
	Point(*derivative)(Point),
	std::vector<Point>& start_points,
	long double learn_rate,
	int n_iter,
	long double tolerance
)
{
	std::vector<Point> results(start_points.size());
	for (int i = 0; i < start_points.size(); i++)
		results[i] = gradientDescentPoint(derivative, start_points[i], learn_rate, n_iter, tolerance);
	return results;
}

std::vector<Point> concurrentGradientDescentPoint(
	int numOfThreads,
	Point(*derivative)(Point),
	std::vector<Point>& start_points,
	long double learn_rate,
	int n_iter,
	long double tolerance
)
{
	tbb::task_group task_group;

	tbb::task_arena::constraints constraints{};
	constraints.set_max_concurrency(numOfThreads);
	tbb::task_arena arena(constraints);
	
	std::vector<Point> results(start_points.size());

	for (int i = 0; i < start_points.size(); i++)
	{
		arena.execute([&, i] {
			task_group.run([&, i] {
				results[i] = gradientDescentPoint(derivative, start_points[i], learn_rate, n_iter, tolerance);
			});
		});
	}
	
	task_group.wait();
	return results;
}

Point gradientDescentPoint(
	Point(*derivative)(Point),
	Point start,
	long double learn_rate, 
	int n_iter,
	long double tolerance
)
{
	Point point = start;
	for (int i = 0; i < n_iter; i++)
	{
		if (point.grad != nullptr)
			delete point.grad;
		point.grad = new Point(derivative(point));
		int toleranceCount = 0;

		for (int i = 0; i < point.pos.size(); i++)
		{
			long double diff = -1 * learn_rate * point.grad->pos[i];
			
			if (abs(diff) <= tolerance)
				toleranceCount++;
			
			point.pos[i] += diff;
		}
		if (toleranceCount == point.pos.size())
			break;
	}
	return point;
}