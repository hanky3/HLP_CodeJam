#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#if 0
using namespace std;

#define MOD 1000000007
#define MAX_WIDTH 101

vector<long long> dragonCurveLength;
int cache[MAX_WIDTH];

FILE *fpInput = NULL;
FILE *fpOutput = NULL;


int getAllRectangleCount(int width)
{
	int &ret = cache[width];
	if (ret > -1)
		return ret;

	if (width <= 1)
	{
		ret = 1;
		return ret;
	}

	ret = 0;
	ret = (getAllRectangleCount(width - 1) + getAllRectangleCount(width - 2)) % MOD;

	return ret;
}

int getAsymRectangleCount(int width)
{
	int ret = 0;

	if (width % 2 == 0)
	{
		ret = getAllRectangleCount(width);
		ret = (ret - getAllRectangleCount(width / 2) + MOD) % MOD;
		ret = (ret - getAllRectangleCount(width / 2 - 1) + MOD) % MOD;

	}
	else
		ret = (getAllRectangleCount(width) - getAllRectangleCount(width / 2) + MOD) % MOD;

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
		int width;
		fscanf(fpInput, "%d", &width);

		memset(cache, -1, MAX_WIDTH*sizeof(int));

		int result = getAllRectangleCount(width);
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