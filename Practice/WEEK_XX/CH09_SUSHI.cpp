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

#define MAX_K			(100000 + 1)
#define MOD			(20091101)

int N;
unsigned long long M;
unsigned long long budgets;
vector<pair<int, int>> sushiInfo;
int cache[301];

void readInputData()
{
	fscanf(fpInput, "%d %llu", &N, &M);
	sushiInfo = vector<pair<int, int>>(N, make_pair(0, 0));
	budgets = (M / 100);

	for (int i = 0; i < N; i++)
	{
		fscanf(fpInput, "%d %d", &(sushiInfo[i].first), &(sushiInfo[i].second));
		sushiInfo[i].first /= 100;
	}

	memset(cache, 0x00, sizeof(cache));
}

int maxPreferSum()
{
	unsigned int totalPrices = 0;
	while (totalPrices <= budgets)
	{
		for (int dish = 0; dish < sushiInfo.size(); dish++)
		{
			int preferValue = sushiInfo[dish].second;
			int price = sushiInfo[dish].first;

			if (totalPrices < price)
				continue;

			int currPos = totalPrices % 300;
			cache[currPos] = max(cache[currPos],
				preferValue + cache[(totalPrices - price) % 300]);
		}
		totalPrices++;
	}

	return cache[budgets % 300];
}


void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");

		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 3) + "out";
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
		unsigned int ret = maxPreferSum();


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
