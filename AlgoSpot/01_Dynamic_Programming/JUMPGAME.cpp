#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>


using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_GRID_LENGTH 101

int gridLength;
vector<vector<int>> gridInfo;
int cache[MAX_GRID_LENGTH][MAX_GRID_LENGTH];

void readInputData()
{
	fscanf(fpInput, "%d", &gridLength);

	gridInfo = vector<vector<int>>(gridLength, vector<int>(gridLength, 0));
	for (int y = 0; y < gridLength; y++)
		for (int x = 0; x < gridLength; x++)
			fscanf(fpInput, "%d", &gridInfo[y][x]);

	memset(cache, -1, sizeof(cache));
}

int goDestination(int x, int y)
{
	if (x >= gridLength || y >= gridLength)
		return 0;

	if (x == gridLength - 1 && y == gridLength - 1)
		return 1;

	int &ret = cache[y][x];

	if (ret != -1)
		return ret;

	ret = 0;
	int currStep = gridInfo[y][x];
	if (goDestination(x + currStep, y) || goDestination(x, y + currStep)) {
		ret = 1;
	}

	return ret;
}


void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");

		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}
	else
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();

		int ret = goDestination(0, 0);
		if (isFile)
			printf("%s\n", ret ? "YES": "NO");
		fprintf(fpOutput, "%s\n", ret ? "YES" : "NO");

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