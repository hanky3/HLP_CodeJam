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

#if 0
using namespace std;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#define CLOCK_CNT		16
#define SWITCH_CNT		10

const int int_max = numeric_limits<int>::max();
vector<int> clockInfo;
vector<vector<int>> clockSwitches;
void setClockSwitches()
{
	clockSwitches = vector<vector<int>>(SWITCH_CNT);
	clockSwitches[0].push_back(0);
	clockSwitches[0].push_back(1);
	clockSwitches[0].push_back(2);

	clockSwitches[1].push_back(3);
	clockSwitches[1].push_back(7);
	clockSwitches[1].push_back(9);
	clockSwitches[1].push_back(11);

	clockSwitches[2].push_back(4);
	clockSwitches[2].push_back(10);
	clockSwitches[2].push_back(14);
	clockSwitches[2].push_back(15);

//	0 4 5 6 7
	clockSwitches[3].push_back(0);
	clockSwitches[3].push_back(4);
	clockSwitches[3].push_back(5);
	clockSwitches[3].push_back(6);
	clockSwitches[3].push_back(7);

//	6, 7, 8, 10, 12
	clockSwitches[4].push_back(6);
	clockSwitches[4].push_back(7);
	clockSwitches[4].push_back(8);
	clockSwitches[4].push_back(10);
	clockSwitches[4].push_back(12);

//	0, 2, 14, 15
	clockSwitches[5].push_back(0);
	clockSwitches[5].push_back(2);
	clockSwitches[5].push_back(14);
	clockSwitches[5].push_back(15);

//	3, 14, 15
	clockSwitches[6].push_back(3);
	clockSwitches[6].push_back(14);
	clockSwitches[6].push_back(15);

//	4, 5, 7, 14, 15
	clockSwitches[7].push_back(4);
	clockSwitches[7].push_back(5);
	clockSwitches[7].push_back(7);
	clockSwitches[7].push_back(14);
	clockSwitches[7].push_back(15);

//	1, 2, 3, 4, 5
	clockSwitches[8].push_back(1);
	clockSwitches[8].push_back(2);
	clockSwitches[8].push_back(3);
	clockSwitches[8].push_back(4);
	clockSwitches[8].push_back(5);

//	3, 4, 5, 9, 13
	clockSwitches[9].push_back(3);
	clockSwitches[9].push_back(4);
	clockSwitches[9].push_back(5);
	clockSwitches[9].push_back(9);
	clockSwitches[9].push_back(13);

}

void readClockInfo()
{
	clockInfo = vector<int>(CLOCK_CNT);
	for (int index = 0; index < CLOCK_CNT; index++)
	{
		fscanf(fpInput, "%d", &clockInfo[index]);
		clockInfo[index] %= 12;
	}
}

void updateClock(int switchIdx, int switchCnt)
{
	for (int clockIdx = 0; clockIdx < clockSwitches[switchIdx].size(); clockIdx++)
	{
		int clock = clockSwitches[switchIdx][clockIdx];
		clockInfo[clock] += (3 * switchCnt);
		clockInfo[clock] %= 12;
	}
}

void undoClock(int switchIdx, int switchCnt)
{
	for (int clockIdx = 0; clockIdx < clockSwitches[switchIdx].size(); clockIdx++)
	{
		int clock = clockSwitches[switchIdx][clockIdx];
		clockInfo[clock] -= (3 * switchCnt);
		clockInfo[clock] = (clockInfo[clock] + 12) % 12;
	}
}

bool allClockSynchronized()
{
	for (int clock = 0; clock < CLOCK_CNT; clock++)
	{
		if (clockInfo[clock] != 0)
			return false;
	}
	return true;
}

bool findClockSync(int nextSwitch, int &minSwitchCnt)
{
	// 기저사례
	if (allClockSynchronized())
	{
		minSwitchCnt = 0;
		return true;
	}

	if (nextSwitch >= SWITCH_CNT)
		return false;

	bool ret = false;
	minSwitchCnt = int_max;
	for (int switchNum = 0; switchNum < 4; switchNum++)
	{
		int switchCnt = 0;

		updateClock(nextSwitch, switchNum);
		if (findClockSync(nextSwitch + 1, switchCnt))
		{
			switchCnt += switchNum;
			minSwitchCnt = min(minSwitchCnt, switchCnt);
			ret = true;
		}
		undoClock(nextSwitch, switchNum);
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
	setClockSwitches();

	for (int i = 0; i < testcase; i++)
	{
		// 전역변수 초기화
		readClockInfo();

		int ret = int_max;
		if (!findClockSync(0, ret))
			ret = -1;

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
#if _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem(NULL, false);
#endif

	return 0;
}
#endif