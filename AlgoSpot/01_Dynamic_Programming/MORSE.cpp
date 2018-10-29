#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <functional>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_SIGNAL_SIZE	200
#define MAX_SKIP_SIZE		1000000010
int n, m;
long long k;
long long cache[MAX_SIGNAL_SIZE + 1][MAX_SIGNAL_SIZE + 1];

void readInputData()
{
	fscanf(fpInput, "%d %d %lld", &n, &m, &k);
}

void preCalc(int k)
{
	for (int i = 1; i <= k; i++) {
		for (int j = 0; j <= k; j++) {
			if (j == 0 || i == j) {
				cache[i][j] = 1;
				continue;
			}

			cache[i][j] = min((long long)MAX_SKIP_SIZE, cache[i - 1][j - 1] + cache[i - 1][j]);
		}
	}
}

string morseOutput(int x, int y, long long &remain, string signal)
{
	string output;

	if (remain <= 0)
		return "";

	if (x == 0 && y == 0) {
		remain--;
		if (remain == 0) {
			output = signal;
		}
		return output;
	}

	if (cache[x + y][x] < remain) {
		remain -= cache[x + y][x];
		return "";
	}

	string next;
	if (x > 0) {
		output = morseOutput(x - 1, y, remain, signal + "-");
	}

	if (y > 0 && remain > 0) {
		output = morseOutput(x, y - 1, remain, signal + "o");
	}

	return output;
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
	preCalc(MAX_SIGNAL_SIZE);
	while (testCase > 0)
	{
		readInputData();

		string ret = morseOutput(n, m, k, "");
		if (isFile)
			printf("%s\n", ret.c_str());
		fprintf(fpOutput, "%s\n", ret.c_str());

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