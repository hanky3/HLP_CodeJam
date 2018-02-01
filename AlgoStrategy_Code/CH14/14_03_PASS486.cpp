#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#if 0
using namespace std;

//#define MOD 1000000007
#define MAX_NUM 10000001

int factorCount[MAX_NUM];

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

void preCalculateFactorCount()
{
	memset(factorCount, 0x00, sizeof(int)*MAX_NUM);

	for (int i = 1; i < MAX_NUM; i++)
	{
		for (int j = i; j < MAX_NUM; j += i)
		{
			factorCount[j]++;
		}
	}
}

int getMatchingFactorNum(int factorN, int lo, int hi)
{
	int result = 0;
	for (int i = lo; i <= hi; i++)
	{
		if (factorCount[i] == factorN)
			result++;
	}
	return result;
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

	preCalculateFactorCount();
	for (int i = 0; i < testcase; i++)
	{
		int factorN, lo, hi;
		fscanf(fpInput, "%d %d %d", &factorN, &lo, &hi);

		int result = getMatchingFactorNum(factorN, lo, hi);
		fprintf(fpOutput, "%d\n", result);
		if (isFile)
			printf("%d\n", result);
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