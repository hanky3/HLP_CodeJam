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

#define MAX_N		500

int N;
vector<vector<int>> diamondNums;

int cache[MAX_N][MAX_N];


void readInputData()
{
	fscanf(fpInput, "%d", &N);
	memset(cache, -1, sizeof(cache));

	diamondNums = vector<vector<int>>(N*2 - 1, vector<int>());

	int sgn = 1;
	int wSize = 1;
	for (int h = 0; h < (N*2-1); h++) {
		for (int w = 0; w < wSize; w++) {
			int num;
			fscanf(fpInput, "%d", &num);
			diamondNums[h].push_back(num);
		}

		if (h >= (N - 1))
			sgn = -1;
		wSize += (sgn);
	}
}

int maxDiamondSum(int h, int w)
{
	if (h >= diamondNums.size() || w >= diamondNums[h].size())
		return 0;

	int &ret = cache[h][w];
	if (ret != -1)
		return ret;

	ret = 0;
	ret = max(ret, diamondNums[h][w] + maxDiamondSum(h + 1, w));
	if (h < (N - 1))
		ret = max(ret, diamondNums[h][w] + maxDiamondSum(h + 1, w + 1));
	else
		ret = max(ret, diamondNums[h][w] + maxDiamondSum(h + 1, w - 1));

	return ret;
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

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);

	while (testCase > 0)
	{
		readInputData();
		int ret = maxDiamondSum(0, 0);
		fprintf(fpOutput, "%d\n", ret);
		if (isFile) {
			printf("%d\n", ret);
		}

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