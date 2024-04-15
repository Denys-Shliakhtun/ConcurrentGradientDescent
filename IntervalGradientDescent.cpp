#include "IntervalGradientDescent.h"
#include <oneapi/tbb/task_arena.h>
#include <oneapi/tbb/task_group.h>

std::vector<Interval> breakIntervalIntoEqualParts(Interval interval, int num_of_intervals)
{
	std::vector<Interval> intervals;
	Point step;
	for (int i = 0; i < interval.start.pos.size(); i++)
		step.pos.push_back((interval.end.pos[i] - interval.start.pos[i]) / num_of_intervals);

	intervals.push_back(Interval(interval.start, interval.start + step));
	for (int i = 1; i < num_of_intervals - 1; i++)
		intervals.push_back(Interval(intervals[i - 1].end, intervals[i - 1].end + step));
	intervals.push_back(Interval(intervals[num_of_intervals - 2].end, interval.end));

	return intervals;
}

std::vector<Point> linearGradientDescentInterval(
	Point(*derivative)(Point),
	Interval interval,
	int num_of_intervals,
	long double learn_rate,
	int n_iter,
	long double tolerance
)
{
	std::vector<Point> results(num_of_intervals);
	std::vector<Interval> intervals = breakIntervalIntoEqualParts(interval, num_of_intervals);

	for (int i = 0; i < num_of_intervals; i++)
		results[i] = gradientDescentInterval(
			derivative, intervals[i], 
			intervals[i].getHalfwayPoint(),
			learn_rate, n_iter, tolerance);
	
	return results;
}

std::vector<Point> concurrentGradientDescentInterval(
	int numOfThreads,
	Point(*derivative)(Point),
	Interval interval,
	int num_of_intervals,
	long double learn_rate,
	int n_iter,
	long double tolerance
)
{
	std::vector<Point> results(num_of_intervals);
	std::vector<Interval> intervals = breakIntervalIntoEqualParts(interval, num_of_intervals);

	tbb::task_group task_group;

	tbb::task_arena::constraints constraints{};
	constraints.set_max_concurrency(numOfThreads); 
	tbb::task_arena arena(constraints);
	
	for (int i = 0; i < num_of_intervals; i++)
	{
		arena.execute([&, i] {
			task_group.run([&, i] {
				results[i] = gradientDescentInterval(
					derivative, intervals[i],
					intervals[i].getHalfwayPoint(),
					learn_rate, n_iter, tolerance);
				});
			});
	}
	task_group.wait();

	return results;
}

Point gradientDescentInterval(
	Point(*derivative)(Point),
	Interval interval,
	Point start,
	long double learn_rate,
	int n_iter,
	long double tolerance
)
{
	Point point = start;
	for (int i = 0; i < n_iter; i++)
	{
		if (point.grad)
			delete point.grad;
		point.grad = new Point(derivative(point));
		int toleranceCount = 0; 
		int outOfRangeCount = 0;

		for (int i = 0; i < point.pos.size(); i++)
		{
			long double diff = -1 * learn_rate * point.grad->pos[i];

			if (abs(diff) <= tolerance)
				toleranceCount++;
			
			if (point.pos[i] + diff < interval.start.pos[i])
			{
				if (point.pos[i] < interval.start.pos[i])
					outOfRangeCount++;
				else
					point.pos[i] = interval.start.pos[i] - tolerance;
			}				
			else if (point.pos[i] + diff > interval.end.pos[i])
			{
				if (point.pos[i] > interval.end.pos[i])
					outOfRangeCount++;
				else
					point.pos[i] = interval.end.pos[i] + tolerance;
			}				
			else
				point.pos[i] += diff;
		}
			
		if (toleranceCount == point.pos.size() || outOfRangeCount == point.pos.size())
			break;
	}
	return point;
}