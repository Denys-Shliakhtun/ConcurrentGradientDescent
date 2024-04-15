#pragma once
#include <vector>
#include "utils.h"

Point f1d(Point x); // derivative of x^2, result is 0
Point f2d(Point x); // derivative of (x-1)^2+cos(x^2), result is 1.65383
Point f3d(Point x); // derivative of (x-1)^2+cos(x^2)+(y+1)^2+cos(y^2), result is (1.65383, -1.65383)
