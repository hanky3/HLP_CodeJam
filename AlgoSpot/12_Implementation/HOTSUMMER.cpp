#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

FILE *fpInput;
FILE *fpOutput;

int W;
vector<int> elecPowers;

void readInputData()
{
	fscanf(fpInput, "%d", &W);
	elecPowers = vector<int>(9);

	for (int index = 0; index < 9; index++) {
		fscanf(fpInput, "%d", &elecPowers[index]);
	}
}


string elecPowerInRange()
{
	int total = 0;
	for (auto elec : elecPowers)
		total += elec;

	if (total <= W)
		return "YES";

	return "NO";
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
		auto ret = elecPowerInRange();
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