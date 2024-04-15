#pragma once
#include <vector>

template<class T>
long double sum(std::vector<T>& vector)
{
	int size = vector.size();

	if (size == 0)
		return 0;

	T sum = 0;
	for (int i = 0; i < size; i++)
		sum += vector[i];

	return sum;
}

template<class T>
long double average(std::vector<T>& vector)
{
	int size = vector.size();

	if (size == 0)
		return 0;

	return sum(vector) / size;
}

struct Point
{
	std::vector<long double> pos;
	Point* grad;


	Point()
	{
		grad = nullptr;
	}

	Point(const Point &p)
	{
		for (int i = 0; i < p.pos.size(); i++)
			pos.push_back(p.pos[i]);
		grad = p.grad;
	}

	Point(const std::vector<long double>& p)
	{
		for (int i = 0; i < p.size(); i++)
			pos.push_back(p[i]);
		grad = nullptr;
	}

	static Point addVectors(Point p1, Point p2)
	{
		Point result;
		for (int i = 0; i < p1.pos.size(); i++)
			result.pos.push_back(p1.pos[i] + p2.pos[i]);
		return result;
	}

	Point operator+(const Point p1)
	{
		return addVectors(*this, p1);
	}

	static Point getHalfwayPoint(Point p1, Point p2)
	{
		Point result;
		for (int i = 0; i < p1.pos.size(); i++)
			result.pos.push_back((p1.pos[i] + p2.pos[i]) / 2);
		return result;
	}
};

struct Interval
{
	Point start;
	Point end;

	Interval()
	{

	}

	Interval(const Point& start, const Point& end)
	{
		this->start = start;
		this->end = end;
	}

	Point getHalfwayPoint()
	{
		return Point::getHalfwayPoint(start, end);
	}
};

std::vector<std::vector<Point>> generateInitialPoints(Interval interval, int numOfPointsInVector, int numOfVectors);
bool testResult(std::vector<Point> res, Point answer, long double tolerance = 1e-5);