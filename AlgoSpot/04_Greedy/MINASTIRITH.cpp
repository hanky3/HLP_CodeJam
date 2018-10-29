#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <functional>


using namespace std;
#define MINAS_RADIUS	8.0f
#define INF 987654321

FILE *fpInput;
FILE *fpOutput;

const double PI = acos(0.0) * 2;

struct CastlePointInfo {
	double x;
	double y;
	double radius;
};
int castlePointCnt;
vector<CastlePointInfo> castlePoints;

void readInputData()
{
	fscanf(fpInput, "%d", &castlePointCnt);

	castlePoints = vector<CastlePointInfo>(castlePointCnt);
	for (int i = 0; i < castlePointCnt; i++) {
		CastlePointInfo p;
		fscanf(fpInput, "%lf %lf %lf", &p.x, &p.y, &p.radius);
		castlePoints[i] = p;
	}
}

bool pointToRange(vector<pair<double, double>> &ranges, vector<pair<double, double>> &startRanges)
{
	for (auto p : castlePoints) {
		double mid = fmod(atan2(p.y, p.x) + 2*PI, 2*PI);
		double range = asin(p.radius / (2.0f * MINAS_RADIUS)) * 2.0f;
		if (range < 0)
			return false;

		pair<double, double> rangeInfo = make_pair(mid - range, mid + range);
		if (rangeInfo.first < 0 || rangeInfo.second > 2 * PI)
			startRanges.push_back(
					make_pair(fmod(rangeInfo.first + 2 * PI, 2 * PI), fmod(rangeInfo.second + 2 * PI, 2 * PI)));

		ranges.push_back(rangeInfo);
	}
	sort(ranges.begin(), ranges.end());

	return true;
}

int solveLinear(double start, double end, vector<pair<double, double>> &ranges)
{
	int count = 0;
	int index = 0;

	double nextStart = start;
	while (start < end) {
		nextStart = -1.0f;
		while (index < ranges.size()) {
			if (ranges[index].first > start)
				break;

			if (ranges[index].second > start)
				nextStart = max(nextStart, ranges[index].second);
			index++;
		}

		if (nextStart < 0.0) {
			return INF;
		}
		start = nextStart;
		count++;
	}

	return count;
}

int minCastleWatchers()
{
	int ret = INF;
	vector<pair<double, double>> ranges;
	vector<pair<double, double>> startRanges;

 	if (!pointToRange(ranges, startRanges))
		return 1;

	for (auto p : startRanges) {
		ret = min(ret, 1+solveLinear(p.second, p.first, ranges));
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

		int ret = minCastleWatchers();
		if (isFile) {
			if (ret >= INF)	printf("IMPOSSIBLE\n");
			else	printf("%d\n", ret);
		}
		if (ret >= INF)	fprintf(fpOutput, "IMPOSSIBLE\n");
		else fprintf(fpOutput, "%d\n", ret);

		testCase--;
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}