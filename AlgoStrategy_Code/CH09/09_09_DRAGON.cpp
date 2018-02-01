#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#if 0
using namespace std;

#define MAX_BOARD_SIZE 21
#define MAX_INF 999999999
vector<long long> dragonCurveLength;

FILE *fpInput = NULL;
FILE *fpOutput = NULL;

void preCalculateDragonCurveLength(int maxGeneration)
{
	for (int i = 0; i <= maxGeneration; i++)
	{
		if (i == 0)
			dragonCurveLength.push_back(1);
		else
		{
			dragonCurveLength.push_back(dragonCurveLength[i - 1] * 2 + 2);
		}
	}
}

long long skip;
void findDragonCurveWords(string appendWord, int generation, int start, int length, string &keyword)
{
	if (generation == 0)
	{
		for (int i = 0; i < appendWord.length(); i++)
		{
			if (skip >= start && skip < start + length)
				keyword.append(appendWord.substr(i, 1));
			skip++;
		}
		return;
	}

	if (skip >= start + length)
		return;


	int currLength = keyword.length();
	for (int i = 0; i < appendWord.length(); i++)
	{
		if (appendWord[i] == 'X')
		{
			if (dragonCurveLength[generation] + skip < start)
				skip += dragonCurveLength[generation];
			else
				findDragonCurveWords("X+YF", generation - 1, start, length, keyword);
		}
		else if (appendWord[i] == 'Y')
		{
			if (dragonCurveLength[generation] + skip < start)
				skip += dragonCurveLength[generation];
			else
				findDragonCurveWords("FX-Y", generation - 1, start, length, keyword);
		}
		else
		{
			if (skip >= start && skip < start + length)
				keyword.append(appendWord.substr(i, 1));
			skip++;
		}
	}

	return;
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
		int generation, p, l;
		fscanf(fpInput, "%d %d %d", &generation, &p, &l);
		
		dragonCurveLength.clear();
		skip = 1;

		preCalculateDragonCurveLength(50);
		string result;
		findDragonCurveWords("FX", generation, p, l, result);
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