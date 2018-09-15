#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

int N;
vector<pair<int, int>> points;

void readInputData()
{
	points = vector<pair<int, int>>(3);
	for (int i = 0; i < points.size(); i++)
		fscanf(fpInput, "%d %d", &points[i].first, &points[i].second);
}

double ccw(pair<int, int> a, pair<int, int> b, pair<int, int> c)
{
	double op = a.first*b.second + b.first*c.second + c.first*a.second;
	op -= (a.second*b.first + b.second*c.first + c.second*a.first);
	return op;
}

bool isParallelLine(vector<pair<int, int>> &point)
{
	double ab = ccw(points[0], points[1], point[2]);
	return (ab == 0);
}

double getPointDist(pair<int, int> p1, pair<int, int> p2)
{
	return sqrt((p1.first - p2.first)*(p1.first - p2.first) + (p1.second - p2.second)*(p1.second - p2.second));
}

double diffRectLength(vector<pair<int, int>> &point) {
	double minLength, maxLength;
	if (isParallelLine(point))
		return -1;

	minLength = numeric_limits<double>::max();
	maxLength = 0;
	for (int index = 0; index < point.size(); index++) {
		double length = getPointDist(point[index], point[(index + 1) % point.size()]);
		length += getPointDist(point[index], point[(index + 2) % point.size()]);
		length *= 2;

		minLength = min(minLength, length);
		maxLength = max(maxLength, length);
	}
	return abs(minLength - maxLength);
}

int main(int argc, char* argv[])
{
	fpInput = stdin;
	fpOutput = stdout;
#ifdef _FILE_
	fpInput = fopen(argv[1], "r");
	fpOutput = fopen("output.txt", "w");
#endif

	int testCase = 1;
	//fscanf(fpInput, "%d", &testCase);
	while (testCase-- > 0) {
		readInputData();
		auto ret = diffRectLength(points);
#ifdef _FILE_
		printf("%.10f\n", ret);
#endif
		fprintf(fpOutput, "%.10f\n", ret);
	}
	fclose(fpInput);
	fclose(fpOutput);
	return 0;
}
