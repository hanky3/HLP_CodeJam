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

#define MAX_CLIMB_LENGTH	(1001)
#define MAX_RAIN_DAYS		(1001)

FILE *fpInput;
FILE *fpOutput;

int n, m;
vector<vector<int>> triangleNums;
double climbProb[MAX_CLIMB_LENGTH][MAX_RAIN_DAYS];

void readInputData()
{
	fscanf(fpInput, "%d %d", &n, &m);
	memset(climbProb, -1, sizeof(climbProb));
}

double calcClimbProbability(int day, int climb)
{
	if (day > m)
		return 0.0f;

	if (climb >= n)
		return 1.0f;


	double &ret = climbProb[climb][day];
	if (ret >= 0)
		return ret;

	ret = 0;
	ret = (0.25 * calcClimbProbability(day + 1, climb + 1)) +
			(0.75 * calcClimbProbability(day + 1, climb + 2));

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

		double ret = calcClimbProbability(0, 0);
		if (isFile)
			printf("%.10f\n", ret);
		fprintf(fpOutput, "%.10f\n", ret);

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
