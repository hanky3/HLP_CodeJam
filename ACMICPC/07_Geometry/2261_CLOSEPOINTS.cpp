#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <limits>
#include <math.h>
#include <algorithm>
#include <set>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

struct Point {
	int x;
	int y;

	Point() {}
	Point(int x, int y) : x(x), y(y) {}

	bool operator<(const Point &p) const {
		if (y == p.y) return x < p.x;
		return y < p.y;
	}
};

int N;
vector<Point> points;

void readInputData()
{
	fscanf(fpInput, "%d\n", &N);
	points = vector<Point>(N);
	for (int n = 0; n < N; n++) {
		fscanf(fpInput, "%d %d\n", &points[n].x, &points[n].y);
	}
	sort(points.begin(), points.end(), [](Point &p1, Point &p2) {
		return p1.x < p2.x;
	});
}

int getDistance(Point p1, Point p2)
{
	return pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2);
}

int findClosePoints()
{
	int dist = 0;

	set<Point> candidates;
	candidates.insert(points[0]);
	candidates.insert(points[1]);
	dist = getDistance(points[0], points[1]);

	int start = 0;
	for (int pos = 2; pos < N; pos++) {
		auto currPoints = points[pos];
		auto itr = candidates.begin();
		while (start < pos) {
			Point temp = points[start];
			int distX = abs(currPoints.x - temp.x);
			if ((distX*distX) > dist) {
				candidates.erase(temp);
				start++;
			}
			else
				break;
		}

		int d = (int)sqrt((double)dist) + 1;
		Point lowerPoint(-10001, currPoints.y - d);
		Point upperPoint(10001, currPoints.y + d);
		auto lowerY = candidates.lower_bound(lowerPoint);
		auto upperY = candidates.upper_bound(upperPoint);
		for (; lowerY != candidates.end(); lowerY++) {
			int distY = lowerY->y - currPoints.y;
			if (distY > 0 && distY*distY > dist)
				break;
			dist = min(getDistance(currPoints, *lowerY), dist);
		}
		candidates.insert(currPoints);
	}
		 
	return dist;
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
	while (testCase > 0) {
		readInputData();
		auto ret = findClosePoints();
#ifdef _FILE_
		printf("%d\n", ret);
#endif
		fprintf(fpOutput, "%d\n", ret);
		testCase--;
	}

#ifdef _FILE_
	fclose(fpInput);
	fclose(fpOutput);
#endif
	return 0;
}

