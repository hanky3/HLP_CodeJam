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

FILE *fpInput = NULL;
FILE *fpOutput = NULL;
#define MAX_GRID_LENGTH			1000000000

vector<pair<int, int>> infectLocationList;
vector<bool> infectionChecked;

bool compare(pair<int, int> a, pair<int, int> b)
{
	if (a.first < b.first)
		return true;

	if (a.first == b.first)
		return (a.second < b.second);

	return false;
}

const pair<int, int> adjInfectedLocs[3] = {
	make_pair(0, 2),
	make_pair(1, 1),
	make_pair(2, 0)
};

int findInfectionTime()
{
	for (auto infectedLoc : infectLocationList)
	{
		for (int index = 0; index < 3; index++)
		{
			pair<int, int> adj = make_pair(infectedLoc.first + adjInfectedLocs[index].first, 
									infectedLoc.second + adjInfectedLocs[index].second);
		
			if (adj.first <= 0 || adj.second <= 0
				|| adj.second > MAX_GRID_LENGTH || adj.second > MAX_GRID_LENGTH)
				continue;


		}
	}

	return 0;
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
		int infectedCnt;
		fscanf(fpInput, "%d", &infectedCnt);

		infectLocationList = vector<pair<int, int>>(infectedCnt);
		for (int index = 0; index < infectedCnt; index++)
		{
			fscanf(fpInput, "%d %d", &infectLocationList[index].first, &infectLocationList[index].second);
		}
		sort(infectLocationList.begin(), infectLocationList.end(), compare); 

		int ret = findInfectionTime();
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
