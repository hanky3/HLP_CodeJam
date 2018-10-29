#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_HEIGHT 99999
int N;
vector<pair<int,int>> priceRanges;
long long cacheScoreInterval;
int maxInterval;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	priceRanges = vector<pair<int, int>>(N);
	cacheScoreInterval = 0;
	maxInterval = 0;

	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d %d", &priceRanges[index].first, &priceRanges[index].second);
		maxInterval = max(maxInterval, abs(priceRanges[index].first - priceRanges[index].second));
	}
}

pair<int, int> maxOverlap(pair<int, int> a, pair<int, int> b)
{
	if (a.first > b.first)
		swap(a, b);

	if (b.first >= a.second)
		return make_pair(-1, -1);

	return make_pair(max(a.first, b.first), min(a.second, b.second));
}

int calcInterval(pair<int, int> priceInterval)
{
	return abs(priceInterval.first - priceInterval.second);
}

long long midScoreInterval(int start, int end)
{
	long long midScore = 0;

	int currHeight = 0;
	int mid = (start + end) / 2;
	int left = mid;
	int right = mid + 1;

	pair<int, int> minOverlap = maxOverlap(priceRanges[left], priceRanges[right]);

	if (minOverlap.first < 0)
		return 0;

	int minInterval = abs(minOverlap.first - minOverlap.second);
	int nextIndex = left;
	midScore = minInterval * 2;

	bool leftEnd = false;
	bool rightEnd = false;

	while (true)
	{
		// area
		pair<int, int> nextOverlap = maxOverlap(minOverlap, priceRanges[nextIndex]);

		if (nextOverlap.first >= 0) {
			minInterval = abs(nextOverlap.first - nextOverlap.second);
			midScore = max(midScore, ((long long)minInterval)*(right - left + 1));
			minOverlap = nextOverlap;
		}
		else {
			if (nextIndex == left)
				leftEnd = true;
			else
				rightEnd = true;
		}

		if (left <= start)		leftEnd = true;
		if (right >= end)	rightEnd = true;

		if (leftEnd && rightEnd)
			break;

		int nextLeft = left - 1;
		int nextRight = right + 1;

		pair<int, int> nextLeftOverlap(-1, -1);
		pair<int, int> nextRightOverlap(-1, -1);
		if (!leftEnd)
			nextLeftOverlap = maxOverlap(minOverlap, priceRanges[nextLeft]);
		if (!rightEnd)
			nextRightOverlap = maxOverlap(minOverlap, priceRanges[nextRight]);

		if (leftEnd ||
			((nextRight <= end) && (calcInterval(nextLeftOverlap) < calcInterval(nextRightOverlap)) )) {
			right++;
			nextIndex = right;
		}
		else {
			left--;
			nextIndex = left;
		}
	}

	return midScore;
}


long long maxScoreInterval(int start, int end)
{
	if (start == end)
		return abs(priceRanges[start].second - priceRanges[start].first);

	//int maxHeuristicSize = (end - start + 1) * maxHeight;
	//if (maxHeuristicSize < cacheRectSize)
	//	return 0;

	long long ret = 0;
	int mid = (start + end) / 2;

	long long leftArea = maxScoreInterval(start, mid);
	long long rightArea = maxScoreInterval(mid + 1, end);
	ret = max(leftArea, rightArea);
	ret = max(ret, midScoreInterval(start, end));
	cacheScoreInterval = max(cacheScoreInterval, ret);

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
		long long ret = maxScoreInterval(0, N - 1);
		if (isFile)
			printf("%lld\n", ret);
		fprintf(fpOutput, "%lld\n", ret);
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
