#include "PointGradientDescent.h"
#include "IntervalGradientDescent.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include "TargetFunctions.h"
using namespace std::chrono;

void testPointGradientDescent(
    int numOfThreads, 
    Point(*grad)(Point), 
    int N_ITER, 
    long double LEARN_RATE, 
    long double TOLERANCE, 
    Interval interval, 
    int numOfPoints, 
    Point resultPoint)
{
    std::vector<std::vector<Point>> initialPoints = generateInitialPoints(interval, numOfPoints, 20);

    std::vector<Point> res;
    steady_clock::time_point start, stop;
    std::vector<int> linearTime, concurrentTime;
    
    int linearPassedTest = 0;
    int concurrentPassedTest = 0;
    
    std::cout << "\n---- Linear ----\n";
    
    // Preheat linear
    for (int i = 0; i < 20; i++)
    {
        linearGradientDescentPoint(grad, initialPoints[i], LEARN_RATE, N_ITER, TOLERANCE);
        std::cout << "Preheat: " << i + 1 << "/20" << std::endl;
    }

    // Measure linear
    for (int i = 0; i < 20; i++)
    {
        start = high_resolution_clock::now();
        res = linearGradientDescentPoint(grad, initialPoints[i], LEARN_RATE, N_ITER, TOLERANCE);
        stop = high_resolution_clock::now();
        std::cout << "\nMeasure: " << i + 1 << "/20" << std::endl;
        if (testResult(res, resultPoint, 1e-2))
        {
            std::cout << "Test passed!\n";
            linearPassedTest++;
        }
        else
            std::cout << "Test failed!\n";
        linearTime.push_back(duration_cast<microseconds>(stop - start).count());
    }

    std::cout << "\n---- Concurrent ----\n";
    std::cout << "Threads: " << numOfThreads << std::endl;

    // Preheat concurrent
    for (int i = 0; i < 20; i++)
    {
        concurrentGradientDescentPoint(numOfThreads, grad, initialPoints[i], LEARN_RATE, N_ITER, TOLERANCE);
        std::cout << "Preheat: " << i + 1 << "/20" << std::endl;
    }

    // Measure concurrent
    for (int i = 0; i < 20; i++)
    {
        start = high_resolution_clock::now();
        res = concurrentGradientDescentPoint(numOfThreads, grad, initialPoints[i], LEARN_RATE, N_ITER, TOLERANCE);
        stop = high_resolution_clock::now();
        std::cout << "\nMeasure: " << i + 1 << "/20" << std::endl;
        if (testResult(res, resultPoint, 1e-2))
        {
            std::cout << "Test passed!\n";
            concurrentPassedTest++;
        }
        else
            std::cout << "Test failed!\n";
        concurrentTime.push_back(duration_cast<microseconds>(stop - start).count());
    }

    std::cout << "\n---- Result ----\n"
        << "Linear time: " << average(linearTime) << std::endl
        << "Linear passed tests: " << linearPassedTest << std::endl
        << "Concurrent time: " << average(concurrentTime) << std::endl
        << "Concurrent passed tests: " << concurrentPassedTest << std::endl
        << "Linear time / Concurrent time: " << average(linearTime) / average(concurrentTime) << std::endl;
}

void testIntervalGradientDescent(
    int numOfThreads, 
    Point(*grad)(Point), 
    int N_ITER, 
    long double LEARN_RATE, 
    long double TOLERANCE, 
    Interval interval, 
    int numOfIntervals, 
    Point step, 
    Point resultPoint)
{
    std::vector<Point> res;
    steady_clock::time_point start, stop;
    std::vector<int> linearTime, concurrentTime;
    std::vector<Interval> intervals;
    intervals.push_back(interval);
    for (int i = 1; i < 20; i++)
    {
        Interval currInterval = intervals[i-1];
        currInterval.end = currInterval.end + step;
        intervals.push_back(currInterval);
    }

    int linearPassedTest = 0;
    int concurrentPassedTest = 0;
    
    std::cout << "\n---- Linear ----\n";

    // Preheat linear
    for (int i = 0; i < 20; i++)
    {
        linearGradientDescentInterval(grad, intervals[i], numOfIntervals, LEARN_RATE, N_ITER, TOLERANCE);
        std::cout << "Preheat: " << i + 1 << "/20" << std::endl;
    }

    // Measure linear
    for (int i = 0; i < 20; i++)
    {
        start = high_resolution_clock::now();
        res = linearGradientDescentInterval(grad, intervals[i], numOfIntervals, LEARN_RATE, N_ITER, TOLERANCE);
        stop = high_resolution_clock::now();
        std::cout << "\nMeasure: " << i + 1 << "/20" << std::endl;
        if (testResult(res, resultPoint, 1e-2))
        {
            std::cout << "Test passed!\n";
            linearPassedTest++;
        }
        else
            std::cout << "Test failed!\n";
        linearTime.push_back(duration_cast<microseconds>(stop - start).count());
    }
    
    std::cout << "\n---- Concurrent ----\n";
    std::cout << "Threads: " << numOfThreads << std::endl;

    // Preheat concurrent
    for (int i = 0; i < 20; i++)
    {
        concurrentGradientDescentInterval(numOfThreads, grad, intervals[i], numOfIntervals, LEARN_RATE, N_ITER, TOLERANCE);
        std::cout << "Preheat: " << i + 1 << "/20" << std::endl;
    }

    // Measure concurrent
    for (int i = 0; i < 20; i++)
    {
        start = high_resolution_clock::now();
        res = concurrentGradientDescentInterval(numOfThreads, grad, intervals[i], numOfIntervals, LEARN_RATE, N_ITER, TOLERANCE);
        stop = high_resolution_clock::now();
        std::cout << "\nMeasure: " << i + 1 << "/20" << std::endl;
        if (testResult(res, resultPoint, 1e-2))
        {
            std::cout << "Test passed!\n";
            concurrentPassedTest++;
        }
        else
            std::cout << "Test failed!\n";
        for (int i = 0; i < res.size(); i++)
            std::cout << res[i].pos[0] << ' ';
        std::cout << "\nTime: " << duration_cast<microseconds>(stop - start).count() << '\n';
        concurrentTime.push_back(duration_cast<microseconds>(stop - start).count());
    }

    std::cout << "\n---- Result ----\n"
        << "Linear time: " << average(linearTime) << std::endl
        << "Linear passed tests: " << linearPassedTest << std::endl
        << "Concurrent time: " << average(concurrentTime) << std::endl
        << "Concurrent passed tests: " << concurrentPassedTest << std::endl
        << "Linear time / Concurrent time: " << average(linearTime) / average(concurrentTime) << std::endl;
}

int main() {    
    const int N_ITER = 10000;
    const long double LEARN_RATE = 0.01;
    const long double TOLERANCE = 1e-200;
    const int N_THREAD = 3;
    Point(*GRAD_1VAR)(Point) = f2d;
    Point(*GRAD_2VAR)(Point) = f3d;
    const Interval INTERVAL_1VAR(Point({ -10 }), Point({ 20 }));
    const Interval INTERVAL_2VAR(Point({ -10, -20 }), Point({ 20, 10 }));

    // test algorithm with 1 variable
    testPointGradientDescent(N_THREAD, GRAD_1VAR, N_ITER, LEARN_RATE, TOLERANCE, INTERVAL_1VAR, 20, Point({ 1.65383 }));

    // test algorithm with 2 variables
    //testPointGradientDescent(N_THREAD, GRAD_2VAR, N_ITER, LEARN_RATE, TOLERANCE, INTERVAL_2VAR, 20, Point({ 1.65383, -1.65383 }));
    
    // test interval algorithm
    //testIntervalGradientDescent(N_THREAD, GRAD_1VAR, N_ITER, LEARN_RATE, TOLERANCE, INTERVAL_1VAR, 20, Point({0.5}), Point({ 1.65383 }));
    
    std::cout << std::endl;
    system("pause");
    return 0;
}
