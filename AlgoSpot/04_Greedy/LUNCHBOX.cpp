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

vector<pair<int, int>> lunchTimes;
int N;

void readInputData()
{
	fscanf(fpInput, "%d", &N);
	lunchTimes = vector<pair<int, int>>(N);

	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &lunchTimes[index].second);
	}

	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &lunchTimes[index].first);
	}

	sort(lunchTimes.begin(), lunchTimes.end(), [](pair<int, int> &a, pair<int, int> &b) {
		return (a.first > b.first);
	});
}

int minimumLunchTime()
{
	int ret = 0;

	int sumMicroTimes = 0;
	for (auto val : lunchTimes) {
		int eatTime = val.first;
		sumMicroTimes += val.second;
		ret = max(ret, (eatTime + sumMicroTimes));
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

		int ret = minimumLunchTime();
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
#ifdef _FILE_
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif

	return 0;
}