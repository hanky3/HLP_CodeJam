#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#if 0
using namespace std;

#define MAX_LENGTH 100
int n, m, k;
int skip;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

const int M = 1000000000 + 100;
int lengthCache[201][201];
void generateSkipSize(int n, int m)
{
	memset(lengthCache, 0, sizeof(lengthCache));
	for (int i = 0; i <= n; ++i)
	{
		lengthCache[i][0] = lengthCache[i][i] = 1;
		for (int j = 1; j < i; ++j)
		{
			lengthCache[i][j] = min(M, lengthCache[i - 1][j - 1] + lengthCache[i-1][j]);
		}
	}
}


bool traverseMorse(int remainN, int remainM, int k, string currentMorse, string &output)
{
	if (remainN < 0 || remainM < 0)
		return false;

	if (lengthCache[remainN + remainM][remainM] + skip < k)
	{
		skip += lengthCache[remainN + remainM][remainM];
		return false;
	}

	if (remainN == 0 && remainM == 0)
	{
		if (skip == k)
		{
			output = currentMorse;
			return true;
		}

		skip++;
		return false;
	}


	if (traverseMorse(remainN - 1, remainM, k, currentMorse + "-", output))
		return true;

	if (traverseMorse(remainN, remainM - 1, k, currentMorse + "o", output))
		return true;

	return false;
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

	generateSkipSize(200, 200);


	int testcase;
	fscanf(fpInput, "%d", &testcase);

	char buf[21];
	for (int i = 0; i < testcase; i++)
	{
		skip = 1;
		fscanf(fpInput, "%d %d %d", &n, &m, &k);
		string result;
		traverseMorse(n, m, k, "", result);

		fprintf(fpOutput, "%s\n", result.c_str());
		if (isFile)
			printf("%s\n", result.c_str());
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