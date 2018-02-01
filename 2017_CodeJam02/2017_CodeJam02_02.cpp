#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <list>
#include <queue>
#include <deque>

#if 0
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

#define MAX_DISTANCE	1000000000

vector<int> lineCenters;
vector<pair<int, int>> lineSorting;

int meanDistance(int center1, int center2)
{
	int ret = abs(center1 - center2) / 2;
	ret += (abs(center1 + center2) % 2);

	return ret;
}

int minRadiusToTour()
{
	if (lineCenters.size() == 1)
		return 0;

	if (lineSorting.size() == 2)
		return meanDistance(lineSorting[0].first, lineSorting[1].first);

	int radius = 0;
	for (int index = 0; index < lineSorting.size() - 2; index++)
	{
		int frontCenter = lineSorting[index].first;
		int backCenter = lineSorting[index+2].first;

		int dist = meanDistance(frontCenter, backCenter);
		radius = max(radius, dist);
	}

	return radius;
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
		int lineCount;
		fscanf(fpInput, "%d", &lineCount);

		lineCenters = vector<int>(lineCount);
		lineSorting = vector<pair<int, int>>(lineCount);

		for (int line = 0; line < lineCount; line++)
		{
			fscanf(fpInput, "%d", &lineCenters[line]);
			lineSorting[line].first = lineCenters[line];
			lineSorting[line].second = line;
		}
		
		sort(lineSorting.begin(), lineSorting.end());
		int ret = minRadiusToTour();
		fprintf(fpOutput, "%d\n", ret);
		if (isFile)
			printf("%d\n", ret);
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