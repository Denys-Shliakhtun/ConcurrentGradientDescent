#include "utils.h"

std::vector<std::vector<Point>> generateInitialPoints(Interval interval, int numOfPointsInVector, int numOfVectors) {
	srand(17);
	std::vector<std::vector<Point>> result(numOfVectors);
	for (int i = 0; i < numOfVectors; i++)
	{
		for (int j = 0; j < numOfPointsInVector; j++)
		{
			Point p;
			for (int k = 0; k < interval.start.pos.size(); k++)			
				p.pos.push_back(rand()%(int)(interval.end.pos[k]-interval.start.pos[k])+interval.start.pos[k]);
			result[i].push_back(p);
		}
	}

	return result;
}

bool testResult(std::vector<Point> res, Point answer, long double tolerance)
{
	for (int i = 0; i < res.size(); i++)
	{
		int countRightPos = 0;
		for (int j = 0; j < answer.pos.size(); j++)		
			if (answer.pos[j] - tolerance < res[i].pos[j] && res[i].pos[j] < answer.pos[j] + tolerance)
				countRightPos++;
		if (countRightPos == answer.pos.size())
			return true;
	}
	return false;
}