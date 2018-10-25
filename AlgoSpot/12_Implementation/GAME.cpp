#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <map>

using namespace std;

#define MAX_TEXT_SIZE	101

FILE *fpInput;
FILE *fpOutput;

int N, K;
vector<vector<int>> waitTimes;

void readInputData()
{
	fscanf(fpInput, "%d %d\n", &N, &K);
	waitTimes = vector<vector<int>>(N, vector<int>(K, 0));

	for (int h = 0; h < N; h++) {
		for (int w = 0; w < K; w++) {
			fscanf(fpInput, "%d", &waitTimes[h][w]);
		}
	}
}

vector<int> gameLoss()
{
	vector<bool> visited(N, false);
	for (int k = 0; k < K; k++) {
		vector<int> firstList;

		int minWaitTime = numeric_limits<int>::max();
		for (int n = 0; n < N; n++) {
			if (visited[n])
				continue;
			if (waitTimes[n][k] < minWaitTime) {
				minWaitTime = waitTimes[n][k];
				firstList.clear();
				firstList.push_back(n+1);
			}
			else if(waitTimes[n][k] == minWaitTime)
				firstList.push_back(n+1);
		}

		if (firstList.size() >= 2) {
			return firstList;
		}
		visited[firstList[0]-1] = true;
	}

	vector<int> ret;
	for (int n = 0; n < N; n++) {
		if (visited[n])	continue;
		ret.push_back(n+1);
	}
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
	while (testCase > 0) {
		readInputData();
		vector<int> ret = gameLoss();
		if (isFile) {
			for (auto n : ret) printf("%d ", n);
			printf("\n");
		}
		for (auto n : ret) fprintf(fpOutput, "%d ", n);
		fprintf(fpOutput, "\n");
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