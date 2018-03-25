#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

class HullPoint {
public:
	HullPoint(double x_=0, double y_=0) : x(x_), y(y_)
	{}

	bool operator == (const HullPoint& p) const
	{
		return x == p.x && y == p.y;
	}

	bool operator < (const HullPoint& p) const
	{
		return x != p.x ? x < p.x : y < p.y;
	}
	double x;
	double y;
};


vector<HullPoint> pointA;
vector<HullPoint> pointB;

typedef vector<pair<HullPoint, HullPoint>> HullLines;
HullLines hullUpper;
HullLines hullDown;

void readInputData()
{
	int pointACnt, pointBCnt;
	fscanf(fpInput, "%d %d", &pointACnt, &pointBCnt);
	pointA = vector<HullPoint>(pointACnt);
	pointB = vector<HullPoint>(pointBCnt);

	for (int index = 0; index < pointACnt; index++) {
		fscanf(fpInput, "%lf %lf", &(pointA[index].x), &(pointA[index].y));
	}

	for (int index = 0; index < pointBCnt; index++) {
		fscanf(fpInput, "%lf %lf", &pointB[index].x, &pointB[index].y);
	}

	hullUpper.clear();
	hullDown.clear();
}

void makeHull(vector<HullPoint> &points, HullLines &upper, HullLines &down)
{
	int start = 0;
	for (int index = 1; index < points.size(); index++) {
		if (points[index].x < points[start].x)
			start = index;
		else if ((points[index].x == points[start].x)
			&& (points[index].y < points[start].y))
			start = index;
	}
	
	int count = points.size() + 1;
	bool upperSide = true;
	bool downSide = false;

	int curr = start, next = start;
	while (count-- > 0) {
		curr = start;
		next = (start - 1 + points.size()) % points.size();

		if (points[next].x < points[curr].x)
			upperSide = false;

		if (points[next].x <= points[curr].x)
			downSide = true;
		else
			downSide = false;

		if (upperSide)
			upper.push_back(make_pair(points[curr], points[next]));
		if (downSide)
			down.insert(down.begin(), make_pair(points[next], points[curr]));

		start = next;
	}
}


double findY(bool minFlag, double x, HullLines &hulls)
{
	double ret = 0;

	if (minFlag)
		ret = numeric_limits<double>::max();

	bool intersected = false;
	for (auto hullLine : hulls) {
		if (x < hullLine.first.x || x > hullLine.second.x)
			continue;

		double a1 = (hullLine.first.y - hullLine.second.y) / (hullLine.first.x - hullLine.second.x);
		double b1 = hullLine.first.y - (a1 * hullLine.first.x);
		double y = a1 * x + b1;

		if (minFlag) 
			ret = min(ret, y);
		else
			ret = max(ret, y);
		
		intersected = true;
	}
	if (!intersected || ret < 0)
		ret = 0.0f;

	return ret;
}

double maxOverlapVertical()
{
	double ret = 0.0f;
	HullPoint minP, maxP;

	makeHull(pointA, hullUpper, hullDown);
	makeHull(pointB, hullUpper, hullDown);
	
	double loX = max(min_element(pointA.begin(), pointA.end())->x, min_element(pointB.begin(), pointB.end())->x);
	double hiX = min(max_element(pointA.begin(), pointA.end())->x, max_element(pointB.begin(), pointB.end())->x);

	for (int iter = 0; iter < 100; iter++) {
		double x1 = (2*loX + hiX)  / 3;
		double x2 = (loX + 2*hiX) / 3;

		double hiY1 = findY(true, x1, hullUpper);
		double loY1 = findY(false, x1, hullDown);

		double hiY2 = findY(true, x2, hullUpper);
		double loY2 = findY(false, x2, hullDown);
		ret = max<double>((hiY1 - loY1), (hiY2 - loY2));

		if ((hiY1 - loY1) < (hiY2 - loY2))
			loX = x1;
		else
			hiX = x2;
	}
	if (ret < 0)
		ret = 0.0f;

	return ret;
}

void solveProblem(const char *fileName, bool isFile)
{
	fpInput = stdin;
	fpOutput = stdout;
	if (isFile) {
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}

	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		double ret = maxOverlapVertical();
		if (isFile)
			printf("%.10f\n", ret);
		fprintf(fpOutput, "%.10f\n", ret);

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}
