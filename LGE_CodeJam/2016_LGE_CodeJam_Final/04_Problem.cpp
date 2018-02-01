#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
using namespace std;

#if 0
//########################
// Constant Variable
//########################
const double PI = 2.0 * acos(0.0);
const double INF = 987654321;

//########################
// Global Variable
//########################
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

//########################
// Common Classes
//########################
class vector2
{
public:
	vector2(double x_ = 0, double y_ = 0) : x(x_), y(y_)
	{}

	bool operator == (const vector2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	bool operator < (const vector2& rhs) const
	{
		return x != rhs.x ? x < rhs.x : y < rhs.y;
	}

	vector2 operator + (const vector2& rhs) const
	{
		return vector2(x + rhs.x, y + rhs.y);
	}

	vector2 operator - (const vector2& rhs) const
	{
		return vector2(x - rhs.x, y - rhs.y);
	}

	vector2 operator * (double rhs) const
	{
		return vector2(x * rhs, y * rhs);
	}

	double norm() const {
		return hypot(x, y);
	}

	vector2 normalize() const {
		return vector2(x / norm(), y / norm());
	}

	double polar() const
	{
		return fmod(atan2(y, x) + 2 * PI, 2 * PI);
	}

	double dot(const vector2 &rhs)
	{
		return x*rhs.x + y*rhs.y;
	}

	double cross(const vector2& rhs) const
	{
		return x*rhs.y - rhs.x*y;
	}

	vector2 project(const vector2& rhs) const
	{
		vector2 r = rhs.normalize();
		return r * r.dot(*this);
	}

	double x, y;
	//fmod
};

double ccw(vector2 origin, vector2 a, vector2 b)
{
	return (a - origin).cross(b - origin);
}

void swap(vector2 &a, vector2 &b)
{
	vector2 temp = a;
	a = b;
	b = temp;
}

bool segmentIntersects(vector2 a, vector2 b, vector2 c, vector2 d)
{
	double ab = ccw(a, b, c) * ccw(a, b, d);
	double cd = ccw(c, d, a) * ccw(c, d, b);

	if (ab == 0 && cd == 0)
	{
		if (b < a) swap(a, b);
		if (d < c) swap(c, d);
		return !(b < c || d < a);
	}
	return ab <= 0 && cd <= 0;
}
	
vector<pair<int, int>> coords;
typedef vector<vector2> Polygon;
float minRectSize = 0;

int compare(pair<int, int> a, pair<int, int> b)
{
	bool bLowerVal = false;

	if (a.first < b.first)
		bLowerVal = true;
	else if (a.first == b.first)
		bLowerVal = (a.second < b.second);

	return bLowerVal;
}


double getRectSize(Polygon rect)
{
	double size = 0.0f;
	for (int index = 0; index < rect.size(); index++)
	{
		int next = (index + 1) % rect.size();
		size += ( (rect[index].x + rect[next].x) * (rect[index].y - rect[next].y) );
	}
	size /= 2.0;
	size = abs(size);
	return size;
}

double generateMinPolygon(Polygon &rect)
{
	int startIndex = 0;

	if (rect.size() == 4)
		return getRectSize(rect);

	// find left most lower coordinate


	double result = INF;
	for (int index = 0; index < coords.size(); index++)
	{
		if (find(rect.begin(), rect.end(), vector2(coords[index].first, coords[index].second)) != rect.end()) continue;
		
		// 현재 선택된 점이 2개 이상인 경우 필터링 시킴
		if (rect.size() >= 2) {
			vector2 newLine(coords[index].first, coords[index].second);
			double cross1 = 1, cross2 = -1;
			cross1 = ccw(rect[rect.size() - 2], rect[rect.size() - 1], newLine);
			if (rect.size() == 3)
				cross2 = ccw(rect[rect.size() - 2], rect[rect.size() - 3], newLine);

			if (rect.size() == 2 && cross1 == 0)
				continue;
			if (rect.size() > 2 && (cross1 < 0 || cross2 > 0))
				continue;
		}
		// 현재 선택된 점이 3개인 경우 넓이를 계산하여 필터링

		rect.push_back(vector2(coords[index].first, coords[index].second));
		result = min(result, generateMinPolygon(rect));
		rect.pop_back();
	}

	return result;
}

void solveProblem(char *filename, bool isFile)
{
	// gFpOutput = fopen("Test.txt", "w");


	if (!isFile)
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	else
	{
		string inputFileName = string(filename);
		string outFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + ".out";
		fpInput = fopen(inputFileName.c_str(), "r");
		fpOutput = fopen(outFileName.c_str(), "w");
	}

	int testcase;
	fscanf(fpInput, "%d", &testcase);

	for (int i = 0; i < testcase; i++)
	{
		int coordsCnt = 0;
		fscanf(fpInput, "%d", &coordsCnt);
		coords = vector<pair<int, int>>(coordsCnt);

		for (auto index = 0; index < coordsCnt; index++)
			fscanf(fpInput, "%d %d\n", &coords[index].first, &coords[index].second);
		sort(coords.begin(), coords.end(), compare);
		Polygon polygon;
		double ret = generateMinPolygon(polygon);
		string output;

		fprintf(fpOutput, "%.1f", ret);
		if (isFile)
			printf("%.1f", ret);


		fprintf(fpOutput, "\n");
		if (isFile)
			printf("\n");
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
	// fclose(gFpOutput);
}


int main(int argc, char **argv)
{
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif