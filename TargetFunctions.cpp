#include "TargetFunctions.h"
#include <cmath>

Point f1d(Point x) { // derivative of x^2
	return Point({ (2 * x.pos[0]) });
}

Point f2d(Point x) { // derivative of (x-1)^2+cos(x^2)
	return Point({ (2 * (x.pos[0] - 1) - 2 * x.pos[0] * sin(x.pos[0] * x.pos[0])) });
}
Point f3d(Point x) { // derivative of (x-1)^2+cos(x^2)
	return Point({ 
		(2 * (x.pos[0] - 1) - 2 * x.pos[0] * sin(x.pos[0] * x.pos[0])),
		(2 * (x.pos[1] + 1) - 2 * x.pos[1] * sin(x.pos[1] * x.pos[1]))
		});
}