#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

long long gameCount;
vector<char> sonnyRPSList;

const char RPS[3] = { 'R', 'P', 'S'};
map<char, int> RPSPosition;


void readInputData()
{
	fscanf(fpInput, "%lld", &gameCount);
}

char winningRPS(char in)
{
	return RPS[(RPSPosition[in]+1)%3];
}

long long skipRPS(long long games)
{
	long long skipCount = 0;

	int factor = 1;
	while (true) {
		long long powN = pow(3, factor);
		if (powN > games)
			break;
		
		skipCount += (games / powN);
		factor++;
	}

	return skipCount;
}

char findSonnyRPS(long long games)
{
	char ret = 'R';
	long long pos = (games + skipRPS(games)) % 3;
	ret = RPS[pos];
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

	RPSPosition.insert(make_pair('R', 0));
	RPSPosition.insert(make_pair('P', 1));
	RPSPosition.insert(make_pair('S', 2));

	while (true)
	{
 		readInputData();
		if (!gameCount)
			break;

		char ret = findSonnyRPS(gameCount-1);
		if (isFile)
			printf("%c\n", winningRPS(ret));
		fprintf(fpOutput, "%c\n", winningRPS(ret));
	}

	fclose(fpInput);
	fclose(fpOutput);
}

int main(int argc, char* argv[])
{
#ifdef _FILE
	solveProblem(argv[1], true);
#else
	solveProblem("", false);
#endif


	return 0;
}