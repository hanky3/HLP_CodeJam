#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

#define MAX_FIRE_CNT				100000

FILE *fpInput;
FILE *fpOutput;

int N, P;
vector<int> fireCosts;
int cache[MAX_FIRE_CNT + 1];

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &P);
	memset(cache, -1, sizeof(cache));

	fireCosts = vector<int>(N, 0);
	for (int index = 0; index < N; index++) {
		fscanf(fpInput, "%d", &fireCosts[index]);
	}
}

int minFireInstallCost(int installed)
{
	if (installed + P > N)
		return 0;

	int &ret = cache[installed];
	if (ret != -1)
		return ret;

	int next = installed;
	int cost = fireCosts[next];
	for (int n = installed; n < (installed + P); n++) {
		if (fireCosts[n] <= cost) {
			cost = fireCosts[n];
			next = n;
		}
	}

	ret = numeric_limits<int>::max();
	for (int n = next; n < (installed + P); n++) {
		ret = min(ret, fireCosts[n] + minFireInstallCost(n + 1));
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
	while (testCase > 0)
	{
		readInputData();
		auto ret = minFireInstallCost(0);
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