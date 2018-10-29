#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace std;
#define MAX_BUF_LENGTH	1001

FILE *fpInput;
FILE *fpOutput;

string quadTreeReverse(string::iterator &itr)
{
	string ret;
	if (*itr != 'x') {
		ret = *itr++;
		return ret;
	}

	// go to next
	itr++;

	string upperLeft = quadTreeReverse(itr);
	string upperRight = quadTreeReverse(itr);
	string downLeft = quadTreeReverse(itr);
	string downRight = quadTreeReverse(itr);

	ret = "x" + downLeft + downRight + upperLeft + upperRight;

	return ret;
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
	char buf[MAX_BUF_LENGTH];
	while (testCase > 0)
	{
		fscanf(fpInput, "%s", buf);

		string inputStr = string(buf);
		auto itr = inputStr.begin();
		auto ret = quadTreeReverse(itr);

		if (isFile)
			printf("%s\n", ret.c_str());
		fprintf(fpOutput, "%s\n", ret.c_str());

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