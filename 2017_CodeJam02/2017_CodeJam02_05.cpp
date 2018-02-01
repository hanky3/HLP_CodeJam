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

	int shelter;
	double distance;

	PolygonInfo(int s, int e) : start(s), end(e), shelter(s), distance(0)
	{}

	bool operator ==(PolygonInfo polygon)
	{
		if (polygon.start == start && polygon.end == end)
			return true;

		if (polygon.start == end && polygon.end == start)
			return true;

		return false;
	}

} PolygonInfo;

vector<pair<int, int>> watchers;
map<pair<int, int>, int> filterWatchers;
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
			filterWatchers.insert(make_pair(curr, index));
		}

		index++;
		itr++;
	}

}

void updateShelterInfo(vector<PolygonInfo> &polyLines)
{
	int index = 0;

	for (auto polygon : polyLines)
	{
		vector2 aVec(watchers[polygon.start].first, watchers[polygon.start].second);
		vector2 bVec(watchers[polygon.end].first, watchers[polygon.end].second);

		// extract target index
		vector<int> candidateShelters;
		int target = (index + polygonLines.size() / 2) % polyLines.size();
		candidateShelters.push_back(polyLines[target].start);
		candidateShelters.push_back(polyLines[target].end);

		target = (target+1) % polyLines.size();
		candidateShelters.push_back(polyLines[target].end);

		int watcherIdx = 0;
		bool updated = false;
		for (auto watcher : polyLines)
		{
			vector<vector2> vectorList;
			if (watcherIdx == 0)
				vectorList.push_back(vector2(watchers[watcher.start].first, watchers[watcher.start].second));
			vectorList.push_back(vector2(watchers[watcher.end].first, watchers[watcher.end].second));

			for (auto p : vectorList)
			{
				double dist = pointToLine(p, aVec, bVec);
				bool isLongerDist = false;
				if (fabs(dist - polyLines[index].distance) < 1e-10)
				{
					if (watchers[watcher.start].first < watchers[polyLines[index].shelter].first)
						isLongerDist = true;
					else if (watchers[watcher.start].first == watchers[polyLines[index].shelter].first)
						isLongerDist = watchers[watcher.start].second < watchers[polyLines[index].shelter].second;
				}
				else if (dist > polyLines[index].distance)
				{
					isLongerDist = true;
				}

				if (isLongerDist)
				{
					polyLines[index].distance = dist;
					polyLines[index].shelter = watcherIdx;
				}
				else
				{
					updated = true;
					break;
				}
			}
			if (updated)	break;
			watcherIdx++;
		}

		index++;
	}
}

typedef vector<vector2> polygon;
polygon generateSinkArea()
{
	int n = filterWatchers.size();
	polygon hull;

	if (n <= 2)
		return polygon();

	auto itr = filterWatchers.begin();
	vector2 pivot(itr->first.first, itr->first.second);
	hull.push_back(pivot);

	polygonLines.clear();

	int pivotIndex = itr->second;
	while (true) {
		vector2 ph = hull.back();
		vector2 next(itr->first.first, itr->first.second);

		int nextPoly = itr->second;
		if (ph == next)
		{
			itr++;
			next = vector2(itr->first.first, itr->first.second);
			nextPoly = itr->second;
		}

		for (auto watcher : filterWatchers)
		{
			pair<int, int> coords = watcher.first;
			int index = watcher.second;

			vector2 newLine(coords.first, coords.second);

			if (ph == next || ph == newLine)
				continue;

			double cross = ccw(ph, next, newLine);
			if (cross > 0)
			{
				nextPoly = index;
				next = newLine;
			}
		}

		// Polygon Line
		PolygonInfo polygon(pivotIndex, nextPoly);
		polygonLines.push_back(polygon);

		if (next == pivot)
			break;

		hull.push_back(next);
		pivotIndex = nextPoly;
	}
	updateShelterInfo(polygonLines);

	return hull;
}


bool nextWatcher(vector<int> &prevWatcher, vector<int> &currWatcher, int &next)
{
	int count = 0;
	next = -1;

	if (prevWatcher.size() == 0)
		return true;

	next = currWatcher[0];

	int duplicate = 0;
	for (auto curr : currWatcher)
	{
		auto findItr = std::find(prevWatcher.begin(), prevWatcher.end(), curr);
		if (findItr != prevWatcher.end())
		{
			duplicate = curr;
			count++;
		}
	}

	if (count == 1)
		return true;

	return false;
}

/*
bool IsValidWatcher(vector<int> &prevWatcher, vector<int> &currWatcher)
{
	// 둘중의 하나만 일치하는지를 찾는다.
	int next;
	if (!nextWatcher(prevWatcher, currWatcher, next))
		return false;

	int startIndex = 0;
	vector2 originCoords(watchers[currWatcher[0]].first, watchers[currWatcher[0]].second);
	vector2 nextCoords(watchers[currWatcher[1]].first, watchers[currWatcher[1]].second);

	int nextIndex = startIndex;
	double prevCross = 0.0f;

	for (auto watcher : filterWatchers)
	{
		vector2 newLine(watcher.second.first, watcher.second.second);
		double cross = ccw(originCoords, nextCoords, newLine);
		if (prevCross * cross < 0)
			return false;

		prevCross = cross;

	}
	return true;
}

int findNextShelter(int a, int b)
{
	int ret = 0;

	vector<int> watcher;
	watcher.push_back(a);
	watcher.push_back(b);

	sort(watcher.begin(), watcher.end());

	double x, y;
	x = (double)(filterWatchers[a].first + filterWatchers[b].first) * (double)0.5;
	y = (double)(filterWatchers[a].second + filterWatchers[b].second) * (double)0.5;

	vector2 aVec = vector2(filterWatchers[a].first, filterWatchers[a].second);
	vector2 bVec = vector2(filterWatchers[b].first, filterWatchers[b].second);

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

int findShelter(int currPoly, int &nextPoly, int watcher1, int watcher2)
{
	if (watcher1 > watcher2)
		swap(watcher1, watcher2);

	if (currPoly < 0)
	{
		auto polygon = find(polygonLines.begin(), polygonLines.end(), PolygonInfo(watcher1, watcher2));
		if (polygon == polygonLines.end())
		{
			nextPoly = currPoly;
			return 0;
		}

		nextPoly = polygon - polygonLines.begin();
		return (polygon->shelter+1);
	}

	int adjPoly = (currPoly - 1) % polygonLines.size();
	if (polygonLines[adjPoly] == PolygonInfo(watcher1, watcher2))
	{
		nextPoly = adjPoly;
		return polygonLines[adjPoly].shelter + 1;
	}

	adjPoly = (currPoly + 1) % polygonLines.size();
	if (polygonLines[adjPoly] == PolygonInfo(watcher1, watcher2))
	{
		nextPoly = adjPoly;
		return polygonLines[adjPoly].shelter + 1;
	}

	nextPoly = currPoly;
	return 0;
}


int handleSinkInfo(int m)
{
	int ret = 0;

	int currPoly = -1;
	for (int index = 0; index < m; index++)
	{
		int x, y;
		int nextPoly;

		fscanf(fpInput, "%d %d", &x, &y);
		x--;
		y--;

		ret += findShelter(currPoly, nextPoly, x, y);
		ret %= MOD;

		currPoly = nextPoly;
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

		polygon hull = generateSinkArea();
		//printf("%d\n", hull.size());

		int ret = handleSinkInfo(m);
		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			fprintf(stdout, "%d\n", ret);
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