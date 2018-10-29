#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_DISTANCE 8030000

int N;
unsigned long long K;
struct CityDistInfo {
	int distance;
	int startDisplay;
	int interval;
};
//priority_queue<pair<int, int>, greater<pair<int, int>>> nerdInfo;
vector<CityDistInfo> cityDistances;


void readInputData()
{
	fscanf(fpInput, "%d %llu", &N, &K);

	cityDistances = vector<CityDistInfo>(N);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d %d %d", &cityDistances[index].distance, &cityDistances[index].startDisplay,
											&cityDistances[index].interval);
	}
}

unsigned long long totalCityNoticeCount(int distance, int &signCount)
{
	unsigned long long total = 0;
	signCount = 0;
	for (auto info : cityDistances) {
		if ((info.distance - info.startDisplay) > distance)
			continue;


		int moved = distance - (info.distance - info.startDisplay);
		if (moved <= info.startDisplay && (moved % info.interval == 0))
			signCount++;

		moved = min(moved, info.startDisplay);
		unsigned long long displayCnt = moved / info.interval + 1ull;
		total += displayCnt;

	}
	return total;
}


int findDisplayPos()
{
	int lo = 0;
	int hi = MAX_DISTANCE + 10;
	int mid = (lo + hi) / 2;
	while (lo + 1 < hi) {
		mid = (lo + hi) / 2;

		int duplicate;
		unsigned long long midCount = totalCityNoticeCount(mid, duplicate);

		if (duplicate > 0 && ((midCount-duplicate) < K && K <= midCount) )
			break;

		if (midCount < K)
			lo = mid;
		else
			hi = mid;
	}
	return mid;
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
		int ret = findDisplayPos();
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);

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