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

#define MAX_ELAPSE_TIME	(200)

FILE *fpInput;
FILE *fpOutput;

int N, M;
vector<double> candidateLocs;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &M);

	candidateLocs = vector<double>(M, 0.0f);
	for (int i = 0; i < M; i++) {
		fscanf(fpInput, "%lf", &candidateLocs[i]);
	}
	sort(candidateLocs.begin(), candidateLocs.end());
}

bool locateCamera(double minInterval)
{
	double prevLoc;
	int remainCamera = N - 1;

	prevLoc = candidateLocs[0];
	for (int index = 1; index < candidateLocs.size(); index++) {
		double interval = candidateLocs[index] - prevLoc;

		if (interval >= minInterval) {
			remainCamera--;
			prevLoc = candidateLocs[index];
		}
		if (remainCamera <= 0)
			break;
	}

	return (remainCamera == 0);
}

double maxCameraInterval()
{
	double lo, hi;
	double mid;
	lo = 0.0f;
	hi = 240.0f;

	while (fabs(hi - lo) > 1e-10) {
		mid = (lo + hi) / 2;
		if (locateCamera(mid))
			lo = mid;
		else
			hi = mid;

	}

	return mid;
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

		double ret = maxCameraInterval();
		if (isFile)
			printf("%.2f\n", ret);
		fprintf(fpOutput, "%.2f\n", ret);

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