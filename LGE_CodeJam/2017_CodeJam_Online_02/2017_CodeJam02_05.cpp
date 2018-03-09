#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <time.h>
using namespace std;

#if 0
#define MOD	1000000007

FILE *fpInput = NULL;
FILE *fpOutput = NULL;


const double PI = 2.0 * acos(0.0);

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
		return sqrt(x*x + y*y);
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

vector2 perpendicularFoot(vector2 p, vector2 a, vector2 b) {
	return a + (p - a).project(b - a);
}

double pointToLine(vector2 p, vector2 a, vector2 b) {
	return (p - perpendicularFoot(p, a, b)).norm();
}

//-------------------------------------------------------
// 전역변수 선언
//-------------------------------------------------------
typedef struct PolygonInfo
{
	int start;
	int end;
	int nextShelter;
	double distance;

	PolygonInfo(int s, int e) : start(s), end(e), nextShelter(s), distance(0.0f) {}

	bool operator == (PolygonInfo poly1)
	{
		if (poly1.start == start && poly1.end == end)
			return true;

		if (poly1.start == end && poly1.end == start)
			return true;
		return false;
	}
} PolygonInfo;

vector<pair<int, int>> watchers;
vector<pair<int, pair<int, int>>> filterWatchers;
vector<vector2> limitRectArea;
vector<PolygonInfo> polygonLines;


void initLimitArea(int n)
{
	limitRectArea.clear();
	limitRectArea = vector<vector2>(8);

	limitRectArea[0] = vector2(n, n);
	limitRectArea[1] = vector2(n / 2, n);
	limitRectArea[2] = vector2(0, n);
	limitRectArea[3] = vector2(0, n / 2);
	limitRectArea[4] = vector2(0, 0);
	limitRectArea[5] = vector2(n / 2, 0);
	limitRectArea[6] = vector2(n, 0);
	limitRectArea[7] = vector2(n, n / 2);

}

void extractLimitArea(pair<int, int> pos, int n)
{
	vector<vector2> mapArea(8);
	mapArea[0] = vector2(0, 0);
	mapArea[1] = vector2(n / 2, 0);
	mapArea[2] = vector2(n, 0);
	mapArea[3] = vector2(n, n / 2);
	mapArea[4] = vector2(n, n);
	mapArea[5] = vector2(n / 2, n);
	mapArea[6] = vector2(0, n);
	mapArea[7] = vector2(0, n / 2);


	vector2 currV = vector2(pos.first, pos.second);

	for (int i = 0; i < mapArea.size(); i++)
	{
		double limitDist = (mapArea[i] - limitRectArea[i]).norm();
		double currDist = (mapArea[i] - currV).norm();

		if (i == 1 || i == 5) 
		{
			limitDist = fabs(mapArea[i].y - limitRectArea[i].y);
			currDist = fabs(mapArea[i].y - currV.y);
		}
		else if (i == 3 || i == 7) 
		{
			limitDist = fabs(mapArea[i].x - limitRectArea[i].x);
			currDist = fabs(mapArea[i].x - currV.x);
		}

		if (currDist < limitDist)
			limitRectArea[i] = currV;
	}
}

bool IsLimitedArea(pair<int, int> currPos)
{
	vector2 currV(currPos.first, currPos.second);
	for (int index = 0; index < limitRectArea.size(); index++)
	{
		int next = (index + 1) % limitRectArea.size();
		double innerAngle = ccw(limitRectArea[index], limitRectArea[next], currV);

		if (innerAngle <= 0)
			return false;
	}

	return true;
}

void makeFiltering()
{
	vector<pair<int, int>>::iterator itr = watchers.begin();
	filterWatchers.clear();

	int index = 0;
	while (itr != watchers.end())
	{
		pair<int, int> curr = *itr;
		if (!IsLimitedArea(curr))
		{
			filterWatchers.push_back(make_pair(index, curr));
		}

		index++;
		itr++;
	}
}

/*
int findNextShelter(int a, int b)
{
	int ret = 0;

	vector<int> watcher;
	watcher.push_back(a);
	watcher.push_back(b);

	sort(watcher.begin(), watcher.end());

	vector2 aVec = vector2(watchers[a].first, watchers[a].second);
	vector2 bVec = vector2(watchers[b].first, watchers[b].second);

	vector<int> shelter;
	double bestDist = -1;

	for (auto watcher : filterWatchers)
	{
		//		vector2 originVec(x, y);

		vector2 nextVec(watcher.second.first, watcher.second.second);
		double dist = pointToLine(nextVec, aVec, bVec);
		if (fabs(dist - bestDist) < 1e-10)
		{
			shelter.push_back(watcher.first);
		}
		else if (dist > bestDist)
		{
			shelter.clear();
			shelter.push_back(watcher.first);
			bestDist = dist;
		}
	}

	if (shelter.size() == 0)
		return 0;

	int best = shelter[0];
	for (int i = 1; i < shelter.size(); i++)
	{
		int next = shelter[i];
		if (watchers[next].first < watchers[best].first)
			best = next;
		else if (watchers[next].first == watchers[best].first)
			best = watchers[next].second < watchers[best].second ? next : best;
	}

	ret = best + 1;
	return ret;
}
*/

double getVerticalDistance(PolygonInfo &polygon, int target)
{
	vector2 aVec = vector2(watchers[polygon.start].first, watchers[polygon.start].second);
	vector2 bVec = vector2(watchers[polygon.end].first, watchers[polygon.end].second);
	vector2 newLine(watchers[target].first, watchers[target].second);
	return pointToLine(newLine, aVec, bVec);
}

void updatePolygonShelter(int watcher)
{
	if (polygonLines.size() == 0)
		return;

	PolygonInfo currPolyLine = polygonLines.back();

	bool isBest = false;
	double dist = getVerticalDistance(currPolyLine, watcher);
	if (fabs(dist - currPolyLine.distance) < 1e-10)
	{
		if (watchers[watcher].first < watchers[currPolyLine.nextShelter].first)
			isBest = true;
		else if (watchers[watcher].first == watchers[currPolyLine.nextShelter].first)
			isBest = watchers[watcher].second < watchers[currPolyLine.nextShelter].second;
	}
	else if (dist > currPolyLine.distance)
	{
		isBest = true;
	}

	if (isBest)
	{
		polygonLines.back().nextShelter = watcher;
		polygonLines.back().distance = dist;
	}
}

typedef vector<pair<int, vector2>> polygon;
polygon generateSinkArea(vector<pair<int, int>> &coords)
{
	int n = coords.size();
	polygon hull;

	pair<int, vector2> pivot;
	pivot.first = filterWatchers[0].first;
	pivot.second = vector2(filterWatchers[0].second.first, filterWatchers[0].second.second);
	hull.push_back(pivot);
	polygonLines.clear();

	while (true) {
		vector2 ph = hull.back().second;

		int nextIdx = (hull.back().first + 1) % coords.size();
		vector2 next(coords[nextIdx].first, coords[nextIdx].second);

		int nextShelter = 0;
		for (int index = 0; index < coords.size(); index++)
		{
			vector2 newLine(coords[index].first, coords[index].second);
			updatePolygonShelter(index);

			if (ph == next || ph == newLine)
				continue;

			double cross = ccw(ph, next, newLine);
			if (cross > 0) {
				nextIdx = index;
				next = newLine;
			}
		}

		PolygonInfo polygonInfo(hull.back().first, nextIdx);
		polygonLines.push_back(polygonInfo);
		if (nextIdx == pivot.first)
			break;

		hull.push_back(make_pair(nextIdx, next));
	}

	for (int index = 0; index < coords.size(); index++)
		updatePolygonShelter(index);

	return hull;
}

vector<int> getWatchersFromPolygon(int polygon)
{
	vector<int> watchers;
	watchers.push_back(polygonLines[polygon].start);
	watchers.push_back(polygonLines[polygon].end);
	sort(watchers.begin(), watchers.end());

	return watchers;
}

vector<vector<int>> cacheNextShelter;
vector<vector<int>> cacheCurrLines;

bool isValidWatchers(int currPolygon, vector<int> currWatcher, int &next)
{
	if (currPolygon >= 0)
	{
		vector<int> linkedPolygons;
		linkedPolygons.push_back((currPolygon + polygonLines.size() - 1) % polygonLines.size());
		linkedPolygons.push_back((currPolygon + 1) % polygonLines.size());
		for (auto polyIndex : linkedPolygons)
		{
			vector<int> nextWatcher = getWatchersFromPolygon(polyIndex);
			if (currWatcher == nextWatcher)
			{
				next = polyIndex;
				return true;
			}
		}

		next = currPolygon;
		return false;
	}

	PolygonInfo poly(currWatcher[0], currWatcher[1]);
	auto itr = find(polygonLines.begin(), polygonLines.end(), poly);
	if (itr != polygonLines.end())
	{
		next = itr - polygonLines.begin();
		return true;
	}

	next = currPolygon;
	return false;
}

int findShelter(vector<int> currWatcher, int curr, int &next)
{
	// 둘중의 하나만 일치하는지를 찾는다
	if (!isValidWatchers(curr, currWatcher, next))
		return 0;
	return polygonLines[next].nextShelter + 1;
}


int handleSinkInfo(int m)
{
	int ret = 0;

	int currPolyLine = -1;
	vector<int> prevWatcher;
	for (int index = 0; index < m; index++)
	{
		int x, y;
		fscanf(fpInput, "%d %d", &x, &y);
	
		x--;
		y--;

		vector<int> currWatcher;
		currWatcher.push_back(x);
		currWatcher.push_back(y);
		sort(currWatcher.begin(), currWatcher.end());

		int nextPolyLine = currPolyLine;
		/*
		int nextshelter = findShelter(currWatcher, currPolyLine, nextPolyLine);
		if (nextshelter > 0)
		{
			printf("%d %d - %d\n", x, y, nextshelter);
			fprintf(fpOutput, "%d %d - %d\n", x, y, nextshelter);
		}
		*/
		ret += findShelter(currWatcher, currPolyLine, nextPolyLine);
		ret %= MOD;

		currPolyLine = nextPolyLine;
	}

	return ret;
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
		// 전역변수 초기화
		watchers.clear();
		filterWatchers.clear();

		int n, k, m;
		fscanf(fpInput, "%d %d %d", &n, &k, &m);

		initLimitArea(n);

		watchers = vector<pair<int, int>>(k);
		for (int index = 0; index < k; index++)
		{
			fscanf(fpInput, "%d %d", &watchers[index].first, &watchers[index].second);
			extractLimitArea(watchers[index], n);
		}
		makeFiltering();
		sort(filterWatchers.begin(), filterWatchers.end(), [](pair<int, pair<int, int>> a, pair<int, pair<int, int>> b) {
			return (a.second < b.second);
		});
		polygon hull = generateSinkArea(watchers);
		cacheNextShelter = vector<vector<int>>(k, vector<int>(k, -1));


		int ret = handleSinkInfo(m);
		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			fprintf(stdout, "[%d]%d\n", i, ret);
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
#if 1
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif