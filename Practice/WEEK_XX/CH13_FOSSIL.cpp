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

double ccw(HullPoint p, HullPoint a, HullPoint b)
{
	double ret = (b.x - p.x) * (p.y - a.y) + (b.y - p.y) * (a.x - p.x);
	return ret;
}

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


inline double Det(double a, double b, double c, double d)
{
	return a*d - b*c;
}

///Calculate intersection of two lines.
///\return true if found, false if not found or error
//bool LineLineIntersect(double x1, double y1, //Line 1 start
//	double x2, double y2, //Line 1 end
//	double x3, double y3, //Line 2 start
//	double x4, double y4, //Line 2 end
//	double &ixOut, double &iyOut) //Output 
bool lineIntersected(pair<HullPoint, HullPoint> lineA, pair<HullPoint, HullPoint> lineB, HullPoint &interP)
{
	double detL1 = Det(lineA.first.x, lineA.first.y, lineA.second.x, lineA.second.y);
	double detL2 = Det(lineB.first.x, lineB.first.y, lineB.second.x, lineB.second.y);
	double x1mx2 = lineA.first.x - lineA.second.x;
	double x3mx4 = lineB.first.x - lineB.second.x;
	double y1my2 = lineA.first.y - lineA.second.y;
	double y3my4 = lineB.first.y - lineB.second.y;

	double xnom = Det(detL1, x1mx2, detL2, x3mx4);
	double ynom = Det(detL1, y1my2, detL2, y3my4);
	double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
	if (denom == 0.0)//Lines don't seem to cross
	{
		interP.x = NAN;
		interP.y = NAN;
		return false;
	}

	interP.x = xnom / denom;
	interP.y = ynom / denom;
	if (!isfinite(interP.x) || !isfinite(interP.y)) //Probably a numerical issue
		return false;

	return true; //All OK
}

#if 0
bool lineIntersected(pair<HullPoint, HullPoint> lineA, pair<HullPoint, HullPoint> lineB, HullPoint &interP)
{
	bool checked = false;
	double x, y;

	// 流急老 版快 眉农
	if (lineA.first.x == lineA.second.x)
		x = lineA.first.x;

	if (!checked) {
		double a1 = (lineA.first.y - lineA.second.y) / (lineA.first.x - lineA.second.x);
		double b1 = lineA.first.y - (a1 * lineA.first.x);
		double a2 = (lineB.first.y - lineB.second.y) / (lineB.first.x - lineB.second.x);
		double b2 = lineB.first.y - (a2 * lineB.first.x);

		x = (b2 - b1) / (a1 - a2);
		y = a1 * x + b1;
	}


	interP.x = x; interP.y = y;
	// range check for A
	if (x < min(lineA.first.x, lineA.second.x) || x > max(lineA.first.x, lineA.second.x))
		return false;
	if (y < min(lineA.first.y, lineA.second.y) || y > max(lineA.first.y, lineA.second.y))
		return false;

	// range check for B
	if (x < min(lineB.first.x, lineB.second.x) || x > max(lineB.first.x, lineB.second.x))
		return false;
	if (y < min(lineB.first.y, lineB.second.y) || y > max(lineB.first.y, lineB.second.y))
		return false;


	return true;
}
#endif

bool isHullInside(vector<HullPoint> &a, vector<HullPoint> &b)
{
	for (auto pointA : a) {
		for (int curr = 0; curr < b.size(); curr++) {
			int next = (curr + 1) % b.size();
			if (ccw(b[curr], b[next], pointA) < 0)
				return false;
		}
	}

	return true;
}

bool findIntersections(HullPoint &min, HullPoint &max)
{
	bool ret = false;
	min.x = 101; min.y = 101;
	max.x = -1; max.y = -1;
	for (int a = 0; a < pointA.size(); a++) {
		pair<HullPoint, HullPoint> lineA;
		lineA.first = pointA[a];
		lineA.second = pointA[(a+1)%pointA.size()];

		for (int b = 0; b < pointB.size(); b++) {
			pair<HullPoint, HullPoint> lineB;
			lineB.first = pointB[b];
			lineB.second = pointB[(b + 1) % pointB.size()];
		
			HullPoint p;
			if (lineIntersected(lineA, lineB, p)) {
				min = (p.x < min.x) ? p : min;
				max = (p.x > max.x) ? p : max;
				ret = true;
			}
		}
	}
	return ret;
}

double findY(bool minFlag, double x, HullLines &hulls)
{
	double ret = 0;

	if (minFlag)
		ret = numeric_limits<double>::max();

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
	}

	return ret;
}

double maxOverlapVertical()
{
	double ret = 0.0f;
	HullPoint minP, maxP;

	if (findIntersections(minP, maxP)) {
		makeHull(pointA, hullUpper, hullDown);
		makeHull(pointB, hullUpper, hullDown);
	}
	else {
		if (isHullInside(pointA, pointB)) {
			makeHull(pointB, hullUpper, hullDown);
		}
		else if (isHullInside(pointB, pointA)) {
			makeHull(pointB, hullUpper, hullDown);
		}
		else
			return 0.0f;

		minP = hullUpper[0].first;
		maxP = hullUpper[hullUpper.size() - 1].second;
	}

	double loX = minP.x;
	double hiX = maxP.x;
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
