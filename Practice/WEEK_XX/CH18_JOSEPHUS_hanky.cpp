#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <math.h>


using namespace std;

FILE *fpInput;
FILE *fpOutput;

#define MAX_HEIGHT 99999
int N, K;
list<int> soldiers;

void readInputData()
{
	fscanf(fpInput, "%d %d", &N, &K);

	soldiers.clear();
	for (int index = 1; index <= N; index++) {
		soldiers.push_back(index);
	}
}

pair<int, int> surviveSoldiers(list<int> &s)
{
	auto itr = s.begin();
	while (s.size() > 2)
	{
		itr = s.erase(itr);
		if (itr == s.end()) itr = s.begin();

		for (int i = 0; i < (K-1); i++)
		{
			itr++;
			if (itr == s.end()) itr = s.begin();
		}
	}

	return make_pair(s.front(), s.back());
}

void solveProblem(const char *fileName, bool isFile)
{
	if (isFile)
	{
		fpInput = fopen(fileName, "r");
		string outputFileName = string(fileName);
		outputFileName = outputFileName.substr(0, outputFileName.length() - 2) + "out";
		fpOutput = fopen(outputFileName.c_str(), "w");
	}
	else
	{
		fpInput = stdin;
		fpOutput = stdout;
	}
	int testCase = 0;
	fscanf(fpInput, "%d", &testCase);
	while (testCase > 0)
	{
		readInputData();
		pair<int, int> ret = surviveSoldiers(soldiers);
		if (isFile)
			printf("%d %d\n", ret.first, ret.second);
		fprintf(fpOutput, "%d %d\n", ret.first, ret.second);
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
