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

vector<int> stringList;

void readInputData()
{
	int strCount;
	fscanf(fpInput, "%d", &strCount);

	stringList = vector<int>(strCount, 0);

	for (int index = 0; index < strCount; index++) {
		fscanf(fpInput, "%d", &stringList[index]);
	}
	sort(stringList.begin(), stringList.end());
}

int minStrCopyCost()
{
	int ret = 0;
	int totalLength = 0;

	bool isFirst = true;
	while (stringList.size() > 1) {
		int mergeLen = stringList[0] + stringList[1];
		ret += mergeLen;

		stringList.erase(stringList.begin(), stringList.begin() + 2);
		stringList.push_back(mergeLen);
		sort(stringList.begin(), stringList.end());
	}
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
		int ret = minStrCopyCost();
		if (isFile)
			printf("%d\n", ret);
		fprintf(fpOutput, "%d\n", ret);

		testCase--;
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