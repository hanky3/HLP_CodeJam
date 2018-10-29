#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define MAX_WIN_NUM 10
double prevAverageWins[MAX_WIN_NUM][MAX_WIN_NUM];
double nextAverageWins[MAX_WIN_NUM][MAX_WIN_NUM];
FILE *gFpOutput = NULL;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

double getMaxNumber(double a, double b)
{
	if (a > b)
		return a;
	else
		return b;
}

double getAverageWins(int step, int winNumber)
{
	double averageWinCases = 0.0f;
	double diff = 0.0f;
	for (int index = 1; index <= step; index++)
	{
		for (int w = 0; w < winNumber; w++)
		{
			for (int l = 0; l < winNumber; l++)
			{
				nextAverageWins[w][l] = getMaxNumber(prevAverageWins[w][l], prevAverageWins[0][0]);

				if (w == (winNumber - 1))
					nextAverageWins[w][l] += prevAverageWins[0][0] + 1;
				else
					nextAverageWins[w][l] += getMaxNumber(prevAverageWins[w + 1][l], prevAverageWins[0][0]);

				if (l == (winNumber - 1))
					nextAverageWins[w][l] += prevAverageWins[0][0];
				else
					nextAverageWins[w][l] += getMaxNumber(prevAverageWins[w][l+1], prevAverageWins[0][0]);

				nextAverageWins[w][l] /= 3.0;
			}
		}

		if (index >= 10000)
			diff = (step - 10000) * (nextAverageWins[0][0] - prevAverageWins[0][0]);

		for (int w = 0; w < winNumber; w++)
		{
			for (int l = 0; l < winNumber; l++)
			{
				prevAverageWins[w][l] = nextAverageWins[w][l];
			}
		}

		if (index >= 10000)
			break;
	}

	averageWinCases = prevAverageWins[0][0] + diff;
	return averageWinCases;
}

void solveProblem(char *filename, bool isFile)
{
//	gFpOutput = fopen("Test.txt", "w");

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

	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);

	for (int i = 0; i < testCase; i++)
	{
		int step, n;

		memset(prevAverageWins, 0, sizeof(double)*MAX_WIN_NUM*MAX_WIN_NUM);
		memset(nextAverageWins, 0, sizeof(double)*MAX_WIN_NUM*MAX_WIN_NUM);

		fscanf(fpInput, "%d %d", &step, &n);
		double result = getAverageWins(step, n);
		fprintf(fpOutput, "%.16lf\n", result);
		if (isFile)
			printf("%lf\n", result);
	}

	if (isFile)
	{
		fclose(fpInput);
		fclose(fpOutput);
	}
//	fclose(gFpOutput);
}

int main(int argc, char **argv)
{
	solveProblem("", false);

	return 0;
}