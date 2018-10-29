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

long long totalPlayCount, winCount;

void readInputData()
{
	fscanf(fpInput, "%lld %lld", &totalPlayCount, &winCount);
}

bool upperWinRatio(long long playCnt, long long currWins, long long newWins)
{
	long long currWinRatio, newWinRatio;
	
	currWinRatio = (currWins*100) / playCnt;
	newWinRatio = ((currWins+newWins)*100) / (playCnt+newWins);

	return (newWinRatio > currWinRatio);
}

long long minWinsToWin(long long playCnt, long long currWins)
{
	long long lo, hi, mid;
	lo = 0;
	hi = 2000000000L;

	if (!upperWinRatio(playCnt, currWins, 2000000000L))
		return -1;

	
	while(lo+1 < hi) {
		mid = (lo + hi) / 2;
	
		if (upperWinRatio(playCnt, currWins, mid))
			hi = mid;
		else
			lo = mid;
	}

	return hi;
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

		long long ret = minWinsToWin(totalPlayCount, winCount);
		if (isFile)
			printf("%lld\n", ret);
		fprintf(fpOutput, "%lld\n", ret);

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