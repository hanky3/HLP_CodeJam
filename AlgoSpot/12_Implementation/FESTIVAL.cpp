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
#define MAX_FESTIVAL_DAYS		1000
#define MAX_FESTIVAL_TEAMS	1000


int festivalDays;
int festivalTeams;
vector<int> festivalCosts;

int cacheCost[MAX_FESTIVAL_DAYS][MAX_FESTIVAL_DAYS];
int totalCost(int start, int end)
{
	int &ret = cacheCost[start][end];

	if (ret >= 0)
		return ret;

	if (end == start)
		return festivalCosts[start];

	ret = festivalCosts[end] + totalCost(start, end - 1);
	return ret;
}

double minFestivalCost()
{
	double averageCost = numeric_limits<double>::max();
	int days = festivalTeams;
	for (int days = festivalTeams; days <= festivalDays; days++)
	{
		for (int start = 0; start <= festivalDays - days; start++)
		{
			double cost = (double)totalCost(start, start+days - 1) / (double)days;
			averageCost = min(cost, averageCost);
		}
	}
	return averageCost;
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
		festivalCosts.clear();
		memset(cacheCost, -1, sizeof(int) * MAX_FESTIVAL_DAYS * MAX_FESTIVAL_DAYS);
		fscanf(fpInput, "%d %d", &festivalDays, &festivalTeams);
		festivalCosts = vector<int>(festivalDays);
		for (int index = 0; index < festivalDays; index++)
			fscanf(fpInput, "%d", &festivalCosts[index]);


		double ret = minFestivalCost();
		fprintf(fpOutput, "%.8f\n", ret);
		if (isFile)
			printf("%.8f\n", ret);
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