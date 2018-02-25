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

#define MAX_DAY	101
#define MAX_TOWN 51

vector<vector<int>> adj;
vector<int> adjNumbers;
int numOfTown;
int days;
int prison;
vector<int> destTowns;
double cache[MAX_DAY][MAX_TOWN][MAX_TOWN];

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &numOfTown, &days, &prison);
	//cin >> numOfTown >> days >> prison;
	adj = vector<vector<int>>(numOfTown, vector<int>(numOfTown, 0));
	adjNumbers = vector<int>(numOfTown, 0);

	for (int i = 0; i < numOfTown; i++)
	{
		for (int j = 0; j < numOfTown; j++)
		{
			fscanf(fpInput, "%d", &adj[i][j]);
			adjNumbers[i] += adj[i][j];
		}
	}

	int destTownCnt;
	fscanf(fpInput, "%d", &destTownCnt);
	destTowns = vector<int>(destTownCnt, 0);
	for (int i = 0; i < destTownCnt; i++)
		fscanf(fpInput, "%d", &destTowns[i]);

	memset(cache, -1, sizeof(cache));
}

double goPrison(int remainDays, int start, int end)
{
	double &ret = cache[remainDays][start][end];
	if (ret >= 0.0)
		return ret;

	if (remainDays == 0)
		return (start == end) ? 1.0f : 0.0f;

	ret = 0.0f;
	int nextTownCount = adjNumbers[start];
	for (int nextTown = 0; nextTown < numOfTown; nextTown++)
	{
		if (adj[start][nextTown] > 0)
			ret += (goPrison(remainDays - 1, nextTown, end) / (double)nextTownCount);
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

		for (auto dest : destTowns)
		{
			double ret = goPrison(days, prison, dest);
			if (isFile)
				printf("%.8f ", ret);
			fprintf(fpOutput, "%.8f ", ret);
		}

		if (isFile)
			printf("\n");
		fprintf(fpOutput, "\n");

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