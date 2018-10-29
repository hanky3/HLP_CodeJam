#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <queue>
#include <assert.h>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_SIZE	81
typedef unsigned long long uint64;

int removePos;
string strWord;

void readInputData()
{
	char word[MAX_SIZE];
	memset(word, 0, sizeof(word));
	fscanf(fpInput, "%d %s\n", &removePos, word);
	strWord = word;
}

string removeMispell(int pos)
{
	string ret = strWord;

	ret.erase(ret.begin() + (pos - 1));
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

	int testCase = 1;
	fscanf(fpInput, "%d", &testCase);

	int index = 1;
	while (testCase > 0)
	{
		readInputData();
		string ret = removeMispell(removePos);
		if (isFile)
			printf("%d %s\n", index, ret.c_str());
		fprintf(fpOutput, "%d %s\n", index, ret.c_str());

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