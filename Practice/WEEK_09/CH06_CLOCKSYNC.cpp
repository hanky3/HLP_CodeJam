#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;
FILE *fpInput;
FILE *fpOutput;

#define CLOCK_COUNT			16
#define MAX_SWITCH_NUMBERS		10
#define MAX_CLOCK_RELATION			6
int clocks[CLOCK_COUNT];
int clockSwitchList[MAX_SWITCH_NUMBERS][MAX_CLOCK_RELATION] = {
	{ 0, 1, 2, -1},
	{ 3, 7, 9, 11, -1},
	{ 4, 10, 14, 15, -1 },
	{ 0, 4, 5, 6, 7, -1},
	{ 6, 7, 8, 10, 12, -1 },
	{ 0, 2, 14, 15, -1 },
	{ 3, 14, 15, -1 },
	{ 4, 5, 7, 14, 15, -1 },
	{ 1, 2, 3, 4, 5, -1 },
	{ 3, 4, 5, 9, 13, -1}
};

bool clockSync()
{
	for (int i = 0; i < CLOCK_COUNT; i++)
	{
		if (clocks[i] != 0)
			return false;
	}
	return true;
}

void moveClock(int switchNum, int count)
{
	if (count < 0)
		count += 12;

	for (int index = 0; index < MAX_CLOCK_RELATION; index++)
	{
		int clockNum = clockSwitchList[switchNum][index];
		if (clockNum == -1)
			break;

		clocks[clockNum] += (3 * count);
		clocks[clockNum] %= 12;
	}

}

int minClockSync(int steps, int totalSwitchCount)
{
	int minSwitchCount = 0;
	if (clockSync())
		return totalSwitchCount;

	if (steps > 9)
		return -1;

	for (int switchCnt = 0; switchCnt < 4; switchCnt++)
	{
		moveClock(steps, switchCnt);
		minSwitchCount = minClockSync(steps + 1, totalSwitchCount + switchCnt);
		if (minSwitchCount > -1)
			return minSwitchCount;
		moveClock(steps, -switchCnt);
	}

	return -1;
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
		for (int i = 0; i < CLOCK_COUNT; i++)
		{
			fscanf(fpInput, "%d", &clocks[i]);
			clocks[i] %= 12;
		}

		int ret = minClockSync(0, 0);
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
