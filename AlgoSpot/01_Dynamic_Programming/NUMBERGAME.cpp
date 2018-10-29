#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

const int MAX_NUMBERS = 50;
int cacheScore[MAX_NUMBERS][MAX_NUMBERS];
vector<int> numbers;

void readInputData()
{
	int numberCnt;
	memset(cacheScore, -50011, sizeof(cacheScore));

	fscanf(fpInput, "%d", &numberCnt);

	numbers = vector<int>(numberCnt);
	for (int index = 0; index < numberCnt; index++) {
		fscanf(fpInput, "%d", &numbers[index]);
	}
}

int maxScoreDiff(int start, int end)
{
	// 기저사례
	if (start == end)
		return numbers[start];

	if (start > end)
		return 0;

	int &ret = cacheScore[start][end];

	if (ret >= -50010)
		return ret;

	ret = -50001;
	if (end - start >= 1) {
		ret = max(ret, 0-maxScoreDiff(start, end - 2));
		ret = max(ret, 0-maxScoreDiff(start+2, end));
	}
	ret = max(ret, numbers[start] - maxScoreDiff(start + 1, end));
	ret = max(ret, numbers[end] - maxScoreDiff(start, end-1));

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

	int testCase = 0;

	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();

		int ret = maxScoreDiff(0, numbers.size()-1);
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