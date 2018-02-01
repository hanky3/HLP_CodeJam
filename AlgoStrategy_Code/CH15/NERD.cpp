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
const double PI = 2.0 * acos(0.0);
FILE *fpInput = NULL;
FILE *fpOutput = NULL;

vector<pair<int, int>> nerdPeopleList;
vector<pair<int, int>> nonNerdPeopleList;

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

int compare(pair<int, int> a, pair<int, int> b)
{
	bool bLowerVal = false;

	if (a.first < b.first)
		bLowerVal = true;
	else if (a.first == b.first)
		bLowerVal = (a.second < b.second);

	return bLowerVal;
}

double ccw(vector2 origin, vector2 a, vector2 b)
{
	return (a - origin).cross(b - origin);
}

typedef vector<pair<vector2, vector2>> Polygon;

Polygon generateOuterPolygon(vector<pair<int, int>> coords)
{
	Polygon polygon;
	int startIndex = 0;

	// find left most lower coordinate
	vector2 originCoords(coords[0].first, coords[0].second);
	vector2 nextCoords(coords[1].first, coords[1].second);

	while (true)
	{
		int nextIndex = startIndex;
		for (int i = 0; i < coords.size(); i++)
		{
			int next = (i + startIndex) % coords.size();
			vector2 newLine(coords[next].first, coords[next].second);

			if (newLine == originCoords)
				continue;

			double cross = ccw(originCoords, nextCoords, newLine);
			double dist = (nextCoords - originCoords).norm() - (newLine - originCoords).norm();

			if (cross > 0 || (cross == 0 && dist < 0))
			{
				nextCoords = newLine;
				nextIndex = next;
			}
		}
		polygon.push_back(make_pair(originCoords, nextCoords));
		originCoords = nextCoords;
		startIndex = nextIndex;
		if (originCoords == polygon[0].first)
			break;

		nextIndex = (nextIndex + 1) % coords.size();
		nextCoords = vector2(coords[nextIndex].first, coords[nextIndex].second);
	}

	return polygon;
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

bool isInside(vector2 q, const Polygon&  p)
{
	int crosses = 0;
	for (int i = 0; i < p.size(); i++)
	{
		int j = (i + 1) % p.size();
		if ((p[i].first.y > q.y) != (p[i].second.y > q.y)) {
			double atX = (p[i].second.x - p[i].first.x) * (q.y - p[i].first.y) / (p[i].second.y - p[i].first.y) + p[i].first.x;
			if (q.x < atX)
				crosses++;
		}
	}
	return (crosses % 2) > 0;
}

bool isPolygonIntersected(Polygon poly1, Polygon poly2)
{
	if (isInside(poly1[0].first, poly2) || isInside(poly2[0].first, poly1))
		return true;

	Polygon::iterator it = poly1.begin();
	while (it != poly1.end())
	{
		Polygon::iterator it2 = poly2.begin();
		while (it2 != poly2.end())
		{
			if (segmentIntersects(it->first, it->second, it2->first, it2->second))
				return true;
			it2++;
		}
		it++;
	}

	return false;
}

bool isNerdTheoryValid()
{
	Polygon nerdPoly = generateOuterPolygon(nerdPeopleList);
	Polygon nonNerdPoly = generateOuterPolygon(nonNerdPeopleList);

	if (!isPolygonIntersected(nerdPoly, nonNerdPoly))
		return true;
	else
		return false;
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
		int peopleCnt = 0;
		fscanf(fpInput, "%d", &peopleCnt);

		nerdPeopleList.clear();
		nonNerdPeopleList.clear();
		for (int j = 0; j < peopleCnt; j++)
		{
			int isNerd = 0;
			int shoeSize, typeSpeed;
			fscanf(fpInput, "%d %d %d", &isNerd, &shoeSize, &typeSpeed);
			if (isNerd)
				nerdPeopleList.push_back(make_pair(shoeSize, typeSpeed));
			else
				nonNerdPeopleList.push_back(make_pair(shoeSize, typeSpeed));
		}

		sort(nerdPeopleList.begin(), nerdPeopleList.end(), compare);
		sort(nonNerdPeopleList.begin(), nonNerdPeopleList.end(), compare);
	
		bool ret = isNerdTheoryValid();
		string output;
		if (ret)
			output = "THEORY HOLDS";
		else
			output = "THEORY IS INVALID";

		fprintf(fpOutput, "%s", output.c_str());
		if (isFile)
			printf("%s", output.c_str());


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