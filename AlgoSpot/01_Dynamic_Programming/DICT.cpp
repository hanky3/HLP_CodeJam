#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;
typedef long long int64;
typedef unsigned long long uint64;

FILE *fpInput;
FILE *fpOutput;

int n, m, k;
int64 cacheLength[201][201];
int64 skip;

void readInputData()
{
	fscanf(fpInput, "%d %d %d", &n, &m, &k);
	skip = k;
}

void preCalcLength(int end)
{
	memset(cacheLength, 0, sizeof(cacheLength));

	for (int i = 0; i <= end; i++) {
		cacheLength[i][0] = 1;
		cacheLength[i][i] = 1;
	}

	for (int i = 1; i <= end; i++) {
		for (int j = 0; j <= end; j++) {
			if (i == j || j == 0) {
				cacheLength[i][j] = 1;
				continue;
			}
			cacheLength[i][j] = cacheLength[i - 1][j - 1] + cacheLength[i - 1][j];

			if (cacheLength[i][j] > 100000010ll)
				cacheLength[i][j] = 100000010ll;
		}
	}
}


bool findWord(int aSize, int bSize, string currWord, string &ret)
{
	if (aSize == 0 && bSize == 0) {
		skip--;
		if (skip == 0) {
			ret = currWord;
			return true;
		}
		return false;
	}

	if (skip > cacheLength[aSize + bSize][aSize]) {
		skip -= cacheLength[aSize + bSize][aSize];
		ret = "NONE";
		return false;
	}


	if (aSize > 0) {
		if (findWord(aSize - 1, bSize, currWord + "a", ret))
			return true;
	}
	if (findWord(aSize, bSize - 1, currWord + "b", ret))
		return true;

	ret = "NONE";
	return false;
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
	int index = 1;
	preCalcLength(200);
	while (testCase > 0)
	{
		string ret;
		readInputData();
		findWord(n, m, "", ret);
		if (isFile)
			printf("%s\n", ret.c_str());
		fprintf(fpOutput, "%s\n", ret.c_str());

		testCase--;
		index++;
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