#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define MOD 1000000007
#define MAX_CITY 50
#define MAX_DAYS 100

#if 0
vector<vector<int>> cityLinkedInfo;
int totalCityNum;
double cacheProbability[MAX_DAYS][MAX_CITY];

FILE *fpInput = NULL;
FILE *fpOutput = NULL;



double escapeProbability(int days, int startCity, int endCity)
{
	if (days == 0)
	{
		if (startCity == endCity)
			return 1.0f;
		else
			return 0.0f;
	}

	double &ret = cacheProbability[days][endCity];

	if (ret >= 0.0)
		return ret;

	ret = 0;
	int adjCityNum = cityLinkedInfo[endCity].size();
	for (int i = 0; i < adjCityNum; i++)
	{
		int adj = cityLinkedInfo[endCity][i];
		ret += escapeProbability(days - 1, startCity, adj) / (double)cityLinkedInfo[adj].size();
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

	for (int i = 0; i < testcase; i++)
	{
		int days, startCity;
		fscanf(fpInput, "%d %d %d", &totalCityNum, &days, &startCity);
		memset(cacheProbability, -1, sizeof(double)*(MAX_DAYS)*(MAX_CITY));

		cityLinkedInfo.clear();
		cityLinkedInfo = vector<vector<int>>(totalCityNum);
		for (int j = 0; j < totalCityNum; j++)
		{
			for (int k = 0; k < totalCityNum; k++)
			{
				int isLinked = 0;
				fscanf(fpInput, "%d ", &isLinked);
				if (isLinked)
					cityLinkedInfo[j].push_back(k);
			}
		}

		int endCityNum = 0;
		fscanf(fpInput, "%d", &endCityNum);
		vector<int> endCityList = vector<int>(endCityNum);
		for (int j = 0; j < endCityNum; j++)
		{
			int endCity;
			fscanf(fpInput, "%d ", &endCity);
			double result = escapeProbability(days, startCity, endCity);

			fprintf(fpOutput, "%.8f ", result);
			if (isFile)
				printf("%.8f ", result);
		}
		fprintf(fpOutput, "\n");
		if (isFile)
			printf("\n");
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